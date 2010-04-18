/***************************************************************************

    Copyright 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tinyvoctrainersettings.h"
#include "tinyvoctrainer.h"

#include "qtvtvoclesson.h"
#include "qtvtvocexpression.h"
#include "qtvtvocdocument.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

// Constants
#define DICTIONARY_PATH "/.tinyvoctrainer/dictionaries"

TinyVocTrainerSettings* TinyVocTrainerSettings::m_Instance = 0;

TinyVocTrainerSettings* TinyVocTrainerSettings::instance()
{
    if(!m_Instance) {
        m_Instance = new TinyVocTrainerSettings(QCoreApplication::instance());
        m_Instance->init();
    }
    return m_Instance;
}

TinyVocTrainerSettings::TinyVocTrainerSettings(QObject *parent)
    : QObject(parent), m_CurrentlyOpenedFile(QString()), m_CurrentDocument(0)
{
}

void TinyVocTrainerSettings::init()
{
    bool pathExist;
    QString settingsPath = QDir::homePath() + QString(DICTIONARY_PATH);
    QDir dir(settingsPath);
    if(!QFileInfo(settingsPath).exists()) {
        pathExist = dir.mkpath(settingsPath);
    } else {
        pathExist = true;
    }

    if(pathExist) {
        QFileInfoList dictionaries = dir.entryInfoList(QDir::Files, QDir::Name);
        foreach(QFileInfo info, dictionaries) {
            m_Dictionaries.insert(info.fileName(),info.absoluteFilePath());
        }
    }
}

void TinyVocTrainerSettings::openDictionary()
{
    QString fileName = QFileDialog::getOpenFileName(0,
                                                    tr("Open dictionary"),
                                                    QDir::homePath());
    openDictionaryFile(fileName);
}

void TinyVocTrainerSettings::openDictionary(const QString& dictionaryName)
{
    if(m_Dictionaries.contains(dictionaryName)) {
        QString filename = m_Dictionaries.value(dictionaryName);
        openDictionaryFile(filename);
    } else {
        qDebug() << "Cannot find dictionary";
    }
}

QStringList TinyVocTrainerSettings::dictionaries() const
{
    return m_Dictionaries.uniqueKeys();
}

QStringList TinyVocTrainerSettings::languages() const
{
    return m_Languages;
}

QStringList TinyVocTrainerSettings::lessons() const
{
    QStringList lessonList;
    foreach(QTvtVocLesson* lession, m_Lessons) {
        if(lession) {
            lessonList.append(lession->name());
        }
    }
    return lessonList;
}

QTvtVocLesson* TinyVocTrainerSettings::lesson(int index) const
{
    QTvtVocLesson* lesson = 0;
    if(index < m_Lessons.count() && index >= 0) {
        lesson = m_Lessons.at(index);
    }
    return lesson;
}

int TinyVocTrainerSettings::openedDictionary() const
{
    return m_Dictionaries.values().indexOf(m_CurrentlyOpenedFile);
}

void TinyVocTrainerSettings::openDictionaryFile(const QString& fileName)
{
    if(fileName.isEmpty()
        || !QFileInfo(fileName).exists()
        || fileName == m_CurrentlyOpenedFile)
        return;

    QTvtVocDocument* document = new QTvtVocDocument(this);
    document->open(fileName);

    QList< QTvtVocContainer* > lessonContainers =
            document->lesson()->childContainers();

    // no lessons, delete document, return
    if(lessonContainers.isEmpty()) {
        delete document;
        return;
    }

    // Read lessons
    QList<QTvtVocLesson*> lessonList;
    foreach(QTvtVocContainer *c, lessonContainers) {
        if (c && c->containerType() == QTvtVocLesson::Lesson) {
            QTvtVocLesson* lesson = static_cast<QTvtVocLesson *>(c);
            lessonList.append(lesson);
        }
    }

    // if there are no languages, delete document, return
    if(document->identifierCount() <= 0) {
        delete document;
        return;
    }

    // Read languages
    QStringList languageList;
    for (int i = 0; i < document->identifierCount(); ++i) {
        languageList.append(document->identifier(i).name());
    }

    // If we are here, document contains all we need. Clean old document
    delete m_CurrentDocument;
    m_CurrentDocument = document;
    m_CurrentlyOpenedFile = fileName;
    m_Lessons = lessonList;
    m_Languages = languageList;
    m_Dictionaries.insert(QFileInfo(fileName).fileName(), fileName);

    emit dictionaryChanged();
}
