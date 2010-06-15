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

#include "querymeesettings.h"
#include "querymee.h"

#include "qmvoclesson.h"
#include "qmvocexpression.h"
#include "qmvocdocument.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

#include <querymee_defaults.h>

QueryMeeSettings* QueryMeeSettings::m_Instance = 0;

QueryMeeSettings* QueryMeeSettings::instance()
{
    if(!m_Instance) {
        m_Instance = new QueryMeeSettings(QCoreApplication::instance());
    }
    m_Instance->init();
    return m_Instance;
}

QueryMeeSettings::QueryMeeSettings(QObject *parent)
    : QObject(parent), m_CurrentlyOpenedFile(QString()), m_CurrentDocument(0)
{
}

void QueryMeeSettings::init()
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
            if (info.fileName().endsWith(".kvtml",Qt::CaseInsensitive)){
                m_Dictionaries.insert(info.fileName(),info.absoluteFilePath());
            }
        }
    }
}

void QueryMeeSettings::openDictionary()
{
    QString fileName = QFileDialog::getOpenFileName(0,
                                                    tr("Open dictionary"),
                                                    QDir::homePath(),"*.kvtml");
    openDictionaryFile(fileName);
}

void QueryMeeSettings::openDictionary(const QString& dictionaryName)
{
    if(m_Dictionaries.contains(dictionaryName)) {
        QString filename = m_Dictionaries.value(dictionaryName);
        openDictionaryFile(filename);
    } else {
        qDebug() << "Cannot find dictionary";
    }
}

QStringList QueryMeeSettings::dictionaries() const
{
    return m_Dictionaries.uniqueKeys();
}

QStringList QueryMeeSettings::languages() const
{
    return m_Languages;
}

QStringList QueryMeeSettings::lessons() const
{
    QStringList lessonList;
    foreach(QmVocLesson* lession, m_Lessons) {
        if(lession) {
            lessonList.append(lession->name());
        }
    }
    return lessonList;
}

QmVocLesson* QueryMeeSettings::lesson(int index) const
{
    QmVocLesson* lesson = 0;
    if(index < m_Lessons.count() && index >= 0) {
        lesson = m_Lessons.at(index);
    }
    return lesson;
}

int QueryMeeSettings::openedDictionary() const
{
    return m_Dictionaries.values().indexOf(m_CurrentlyOpenedFile);
}

void QueryMeeSettings::openDictionaryFile(const QString& fileName)
{
    if(fileName.isEmpty()
        || !QFileInfo(fileName).exists()
        || fileName == m_CurrentlyOpenedFile)
        return;

    QmVocDocument* document = new QmVocDocument(this);
    document->open(fileName);

    QList< QmVocContainer* > lessonContainers =
            document->lesson()->childContainers();


    QList<QmVocLesson*> lessonList;
    // if the list is empty, then there should be a default lesson "Document Lesson"
    // in the object document->lesson()
    if(lessonContainers.isEmpty()) {
        // just append the one lesson
        lessonList.append(document->lesson());
    } else {

//        // add an all lesson if there is more than one lesson
//        // and add all the entries from the document
//        if (lessonContainers.count() > 1){
//            QmVocLesson* allLesson = new QmVocLesson("All", document->lesson());
//            foreach (QmVocExpression * entry, document->lesson()->entries(QmVocLesson::Recursive))
//            {
//                QmVocExpression *newentry;
//                newentry = entry;
//                allLesson->appendEntry(newentry);
//            }
//            lessonList.append(allLesson);
//        }

        // Read lessons
        foreach(QmVocContainer *c, lessonContainers) {
            if (c && c->containerType() == QmVocLesson::Lesson) {
                QmVocLesson* lesson = static_cast<QmVocLesson *>(c);
                lessonList.append(lesson);
            }
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
