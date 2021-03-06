/***************************************************************************

    Copyright 2010, 2011 Reto Zingg <g.d0b3rm4n@gmail.com>

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
            if (   info.fileName().endsWith(".kvtml",Qt::CaseInsensitive)
                || info.fileName().endsWith(".voc",Qt::CaseInsensitive)
                || info.fileName().endsWith(".tsv",Qt::CaseInsensitive)){
                m_Dictionaries.insert(info.fileName(),info.absoluteFilePath());
            }
        }
    }
}

void QueryMeeSettings::openDictionary()
{
    QString fileName = QFileDialog::getOpenFileName(0,
                                                    tr("Open dictionary"),
                                                    QDir::homePath(),"*.kvtml *.voc *.tsv");
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
    if(index < m_Lessons.size() && index >= 0) {
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

        // Read lessons
        foreach(QmVocContainer *c, lessonContainers) {
            if (c && c->containerType() == QmVocLesson::Lesson) {

                QmVocLesson* lesson = static_cast<QmVocLesson *>(c);

                // check if in the lesson are any entries, if not we don't need it in the list
                if (lesson->entries().size() > 0 ){
                    lessonList.append(lesson);
                }

                // if there are child lessons we need to add them
                if(c->childContainers().size() > 0){
                    foreach (QmVocContainer *cc, getChildLessons(c)){
                        QmVocLesson* childLessons = static_cast<QmVocLesson *>(cc);
                        lessonList.append(childLessons);
                    }
                }
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

QmVocDocument* QueryMeeSettings::getQmVocDocument(){
    return m_CurrentDocument;
}

QString QueryMeeSettings::getCurrentlyOpenedFile(){
    return m_CurrentlyOpenedFile;
}

QList<QmVocContainer*> QueryMeeSettings::getChildLessons(QmVocContainer* container){

    QList<QmVocContainer*> childLessons;

    foreach (QmVocContainer *cc, container->childContainers()){

        // add a child lesson to the list
        if(cc && cc->containerType() == QmVocLesson::Lesson){

            QmVocLesson* lesson = static_cast<QmVocLesson *>(cc);

            // check if in the lesson are any entries, if not we don't need it in the list
            if (lesson->entries().size() > 0 ){
                childLessons.append(cc);
            }
        }

        // check if there are child lessons in the child...
        if(cc && cc->childContainers().size() > 0){
            childLessons.append(getChildLessons(cc));
        }
    }

    return childLessons;
}
