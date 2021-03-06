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

#include <QDebug>
#include <QList>

#include "qmtrainer.h"
#include "querymeesettings.h"
#include "qmvocexpression.h"
#include "version.h"
#include "querymee_defaults.h"
#include "leitnerwidget.h"

QmTrainer::QmTrainer(QWidget *parent) :
    QWidget(parent),
    m_QuestionLanguage(-1),
    m_AnswerLanguage(-1),
    m_randomOnly(false),
    m_revision(false),
    m_lastAnswerRight(true)
{
    srand(time(NULL));

    m_LastExp = new QmVocExpression();
    m_lessonIndexes = new QList<int>();

#ifndef Q_WS_MAEMO_5
    m_leitnerWidget = new LeitnerWidget(this);
#endif

    for(int i=0; i <= NumberOfLeitnerBoxes; i++){
        QList<QmVocExpression *> *leitnerBox = new QList<QmVocExpression *>();
        leitnerBoxes.insert(i, leitnerBox);
    }
}

QmTrainer::~QmTrainer(){
    // FIXME: should we ask the user if overwritting is OK?
    // FIXME: is it OK to have it in the destructor?

    if(m_CurrentFileName.endsWith(".tsv") || m_CurrentFileName.endsWith(".voc")){
        m_CurrentFileName.append(".kvtml");
    }

    qDebug() << "We are gonna overwrite the file, should we ask the user?" << m_CurrentFileName;
    m_QmVocDocument->saveAs(QUrl(m_CurrentFileName),
                            QmVocDocument::Kvtml,
                            QString("Querymee ").append(QM_VERSION));
}

QmVocExpression * QmTrainer::getAnyEntryFromLesson()
{
    QmVocExpression* vocExpression = 0;
    QmVocLesson* lesson =
    QueryMeeSettings::instance()->lesson(m_lessonIndexes->at(randomInt(0, m_lessonIndexes->size() - 1 )));
    if(lesson) {
        int random_int = randomInt(0, lesson->entries().size() - 1 );
        vocExpression = lesson->entry(random_int);
    }
    return vocExpression;
}

QmVocExpression * QmTrainer::getNextEntryFromLesson()
{
    static int index = 0;

    QmVocExpression* vocExpression = 0;
    QmVocLesson* lesson =
    QueryMeeSettings::instance()->lesson(m_lessonIndexes->at(0)); //assuming only one lession
    if(lesson) {
        vocExpression = lesson->entry(index);
    }

    //Stck at last entry
    if(index < (lesson->entries().size() - 1))
    {
        ++index;
    }
    return vocExpression;
}

QmVocExpression * QmTrainer::getNextEntry()
{
    QmVocExpression* vocExpression = 0;

    if(m_randomOnly == true){
        return getAnyEntryFromLesson();
    }
    else if(m_revision == true){
        return getNextEntryFromLesson();
    }
    else {
        // see querymee_defaults.h
        MAXENTRIES;

        // check if a leitner box has reached the limit
        // if so add the entries to the inPractice QList
        for(int i=1;i < NumberOfLeitnerBoxes; i++){
            if (i != 0 && leitnerBoxes.at(i)->size() >= maxEntries[i]){

                inPractice.append(*leitnerBoxes.at(i));
                leitnerBoxes.at(i)->clear();

                m_lastAnswerRight = true;
            }
        }

        // check if we should add a entry to inPractice from Pool (m_entries)
        if (inPractice.size() < MaxGrade0 && m_entries.size() > 0 && m_lastAnswerRight == false){

            vocExpression = m_entries.at( randomInt(0, m_entries.size()) );

            inPractice.append(vocExpression);
            m_entries.removeOne(vocExpression);

            m_lastAnswerRight = true;
        }
        else if   ((m_lastAnswerRight == false && inPractice.size() < MaxGrade0)
                || (m_lastAnswerRight == true  && m_entries.size() == 0 && inPractice.size() == 0 )) {
            // check if we should add an entry to inPractice from one of the leitner boxes
            for(int i=0;i <= NumberOfLeitnerBoxes; i++){

                if (leitnerBoxes.at(i)->size() > 0){
                    vocExpression = leitnerBoxes.at(i)->at( randomInt(0, leitnerBoxes.at(i)->size()) );

                    inPractice.append(vocExpression);
                    leitnerBoxes.at(i)->removeOne(vocExpression);

                    if (vocExpression != m_LastExp && inPractice.size() == 1){
                        m_lastAnswerRight = true;
                        break;
                    }
                }
            }
        }

        // return an entry prefered from inPractice otherwise from pool (m_entries)
        if ( inPractice.size() > 2 ){
            do {
                vocExpression = inPractice.at(randomInt(0, inPractice.size()));
            } while (m_LastExp == vocExpression);

            return vocExpression;
        }
        else if (inPractice.size() > 0 && inPractice.isEmpty() == false){

            if (inPractice.last() == m_LastExp){
                vocExpression = inPractice.first();
            } else {
                vocExpression = inPractice.last();
            }
            return vocExpression;
        }
        else if ( m_entries.size()  > 0) {

            vocExpression = m_entries.at(randomInt(0 ,m_entries.size()));

            inPractice.append(vocExpression);
            m_entries.removeOne(vocExpression);

            return vocExpression;
        }

        // we never should end up here, but we never know...
        qDebug() << "Uups, the Leitner System failed... return a random value...";
        return getAnyEntryFromLesson();
    }
}


void QmTrainer::setLesson(QList<int> *lessonIndexes)
{
    m_lessonIndexes = lessonIndexes;

    foreach (int lessonIndex, *m_lessonIndexes){
        QmVocLesson *lesson;
        lesson = QueryMeeSettings::instance()->lesson(lessonIndex);
        qDebug() << "Entries from the following lesson are selected for the training:" << lesson->name();

        // put all entries to a pool list
        m_entries.append(lesson->entries());
    }

#ifndef Q_WS_MAEMO_5
    // position 0 is pool
    m_leitnerWidget->updateMaxProgressBar(0, m_entries.size());
    m_leitnerWidget->updateMaxProgressBar(NumberOfLeitnerBoxes, m_entries.size());
#endif

    foreach (QmVocExpression* entry ,m_entries){

        // depending on the grade the entry has, put it into a leitner box (QList)
        int grade = entry->translation(m_AnswerLanguage)->grade();

        if (grade > 0){
            if (grade < NumberOfLeitnerBoxes){
                leitnerBoxes.at(grade)->append(entry);
#ifndef Q_WS_MAEMO_5
                m_leitnerWidget->updateProgressBar(grade,leitnerBoxes.at(grade)->size());
#endif
                m_entries.removeOne(entry);
            }
            else {
                leitnerBoxes.at(7)->append(entry);
                m_entries.removeOne(entry);
            }
        }
    }

#ifndef Q_WS_MAEMO_5
    // position 0 is pool
    m_leitnerWidget->updateProgressBar(0, m_entries.size());
#endif
}


int QmTrainer::randomInt(int min, int max){
    int i = 0;

    if (max != 0){
        i = ( rand() %  max ) + min ;
    }
    else{
        i = min;
    }

    return i;
}

void QmTrainer::handleAnswer(bool answerCountsAsRight){

    // update the time when this expression was practiced
    m_CorrectExp->translation(m_AnswerLanguage)->setPracticeDate(QDateTime::currentDateTime());
    // increment the practice counter
    m_CorrectExp->translation(m_AnswerLanguage)->incPracticeCount();

    if(answerCountsAsRight == true && m_randomOnly == false && m_revision == false){

        // remove from the practice pool
        inPractice.removeOne(m_CorrectExp);

        int grade = m_CorrectExp->translation(m_AnswerLanguage)->grade();

        // put it to the next leitner box
        if (grade < NumberOfLeitnerBoxes){
            leitnerBoxes.at(grade + 1)->append(m_CorrectExp);
        } else {
            leitnerBoxes.at(7)->append(m_CorrectExp);
        }

        // answer was correct so increase the grade
        m_CorrectExp->translation(m_AnswerLanguage)->incGrade();
        qDebug() << "New Grade:" << m_CorrectExp->translation(m_AnswerLanguage)->grade();

    }
    else{
        if(m_randomOnly == false && m_revision == false){
            // the answer was not correct in the first attempt
            // set grade back to 0 and increment the errorcount
            m_CorrectExp->translation(m_AnswerLanguage)->setGrade(0);
            m_CorrectExp->translation(m_AnswerLanguage)->incBadCount();
        }
    }

    // store the expression so we can check later that it isn't used 2 times in a row
    m_LastExp = m_CorrectExp;


    qDebug() << "Pool:" << m_entries.size();
#ifndef Q_WS_MAEMO_5
    m_leitnerWidget->updateProgressBar(0, m_entries.size());
#endif
    qDebug() << "Practice: " << inPractice.size();
    for(int i=1;i <= NumberOfLeitnerBoxes; i++){
        qDebug() << "Box:" << i << "size:" << leitnerBoxes.at(i)->size();
#ifndef Q_WS_MAEMO_5
        m_leitnerWidget->updateProgressBar(i, leitnerBoxes.at(i)->size());
#endif
    }


    return;
}

void QmTrainer::setRandomOnly(bool randomOnly){
    m_randomOnly = randomOnly;
}

void QmTrainer::setRevision(bool revision){
    m_revision = revision;
}

void QmTrainer::setLastAnswerRight(bool lastAnswerRight){
    m_lastAnswerRight = lastAnswerRight;
}

void QmTrainer::setCurrentFileName(QString fileName){
    m_CurrentFileName = fileName;
}

void QmTrainer::setQuestionLanguage(int languageIndex)
{
    m_QuestionLanguage = languageIndex;
}

void QmTrainer::setAnswerLanguage(int languageIndex)
{
    m_AnswerLanguage = languageIndex;
}

void QmTrainer::setQmVocDocument(QmVocDocument *doc){
    m_QmVocDocument = doc;
}
