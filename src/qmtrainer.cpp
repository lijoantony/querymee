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

#include <QDebug>

#include "qmtrainer.h"
#include "querymeesettings.h"
#include "qmvocexpression.h"
#include "version.h"

#define MaxGrade0 5

QmTrainer::QmTrainer(QWidget *parent) :
    QWidget(parent),
    m_LessionIndex(-1),
    m_QuestionLanguage(-1),
    m_AnswerLanguage(-1),
    m_randomOnly(false)
{
    srand(time(NULL));

    m_firstAnswerWrong = 0;
    m_LastExp = new QmVocExpression();
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
    QueryMeeSettings::instance()->lesson(m_LessionIndex);
    if(lesson) {
        int random_int = randomInt(0, lesson->entries().size() - 1 );
        vocExpression = lesson->entry(random_int);
    }
    return vocExpression;
}

QmVocExpression * QmTrainer::getNextEntry()
{
    QmVocExpression* vocExpression = 0;

    if(m_randomOnly == true){
        return getAnyEntryFromLesson();
    }
    else {

        // FIXME: there has to be a better solution for that?!

        if( leitnerBox1.count() > 15 ){
            inPractice.append(leitnerBox1);
            leitnerBox1.clear();
        }

        if( leitnerBox2.count() > 30 ){
            inPractice.append(leitnerBox2);
            leitnerBox2.clear();
        }

        if( leitnerBox3.count() > 60 ){
            inPractice.append(leitnerBox3);
            leitnerBox3.clear();
        }

        if( leitnerBox4.count() > 120 ){
            inPractice.append(leitnerBox4);
            leitnerBox4.clear();
        }

        if( leitnerBox5.count() > 240 ){
            inPractice.append(leitnerBox5);
            leitnerBox5.clear();
        }

        if( leitnerBox6.count() > 480 ){
            inPractice.append(leitnerBox6);
            leitnerBox6.clear();
        }


    /******************************************************************************
    **
    ** Pool
    **
    ******************************************************************************/
        if (inPractice.count() < MaxGrade0 && m_entries.count() > 0){

            do{

                vocExpression = m_entries.at( randomInt(0, m_entries.count() - 1) );

                inPractice.append(vocExpression);
                m_entries.removeOne(vocExpression);

            } while ( inPractice.count() < MaxGrade0);

            do {
                vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
            } while (m_LastExp == vocExpression);

            return vocExpression;

        }
    /******************************************************************************
    **
    ** Leitner Box 1
    **
    ******************************************************************************/
        else if (inPractice.count() < MaxGrade0 && leitnerBox1.count() > 0){

            do{

                vocExpression = leitnerBox1.at( randomInt(0, leitnerBox1.count() - 1) );

                inPractice.append(vocExpression);
                leitnerBox1.removeOne(vocExpression);

            } while ( inPractice.count() < MaxGrade0);

            do {
                vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
            } while (m_LastExp == vocExpression);

            return vocExpression;

        }
    /******************************************************************************
    **
    ** Leitner Box 2
    **
    ******************************************************************************/
        else if (inPractice.count() < MaxGrade0 && leitnerBox2.count() > 0) {

            do{

                vocExpression = leitnerBox2.at( randomInt(0, leitnerBox2.count() - 1) );

                inPractice.append(vocExpression);
                leitnerBox2.removeOne(vocExpression);

            } while ( inPractice.count() < MaxGrade0);

            do {
                vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
            } while (m_LastExp == vocExpression);

            return vocExpression;
        }
    /******************************************************************************
    **
    ** Leitner Box 3
    **
    ******************************************************************************/
        else if (inPractice.count() < MaxGrade0 && leitnerBox3.count() > 0) {

            do{

                vocExpression = leitnerBox3.at( randomInt(0, leitnerBox3.count() - 1) );

                inPractice.append(vocExpression);
                leitnerBox3.removeOne(vocExpression);

            } while ( inPractice.count() < MaxGrade0);

            do {
                vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
            } while (m_LastExp == vocExpression);

            return vocExpression;
        }
    /******************************************************************************
    **
    ** Leitner Box 4
    **
    ******************************************************************************/
        else if (inPractice.count() < MaxGrade0 && leitnerBox4.count() > 0) {

            do{

                vocExpression = leitnerBox4.at( randomInt(0, leitnerBox4.count() - 1) );

                inPractice.append(vocExpression);
                leitnerBox4.removeOne(vocExpression);

            } while ( inPractice.count() < MaxGrade0);

            do {
                vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
            } while (m_LastExp == vocExpression);

            return vocExpression;
        }
    /******************************************************************************
    **
    ** Leitner Box 5
    **
    ******************************************************************************/
        else if (inPractice.count() < MaxGrade0 && leitnerBox5.count() > 0) {

            do{

                vocExpression = leitnerBox5.at( randomInt(0, leitnerBox5.count() - 1) );

                inPractice.append(vocExpression);
                leitnerBox5.removeOne(vocExpression);

            } while ( inPractice.count() < MaxGrade0);

            do {
                vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
            } while (m_LastExp == vocExpression);

            return vocExpression;
        }
    /******************************************************************************
    **
    ** Leitner Box 6
    **
    ******************************************************************************/
        else if (inPractice.count() < MaxGrade0 && leitnerBox6.count() > 0) {

            do{

                vocExpression = leitnerBox6.at( randomInt(0, leitnerBox6.count() - 1) );

                inPractice.append(vocExpression);
                leitnerBox6.removeOne(vocExpression);

            } while ( inPractice.count() < MaxGrade0);

            do {
                vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
            } while (m_LastExp == vocExpression);

            return vocExpression;
        }
    /******************************************************************************
    **
    ** Leitner Box 7
    **
    ******************************************************************************/
        else if (inPractice.count() < MaxGrade0 && leitnerBox7.count() > 0) {

            do{

                vocExpression = leitnerBox7.at( randomInt(0, leitnerBox7.count() - 1) );

                inPractice.append(vocExpression);
                leitnerBox7.removeOne(vocExpression);

            } while ( inPractice.count() < MaxGrade0);

            do {
                vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
            } while (m_LastExp == vocExpression);

            return vocExpression;
        }
        else if (inPractice.count() >= MaxGrade0 ){

            do {
                vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
            } while (m_LastExp == vocExpression);

            return vocExpression;
        }

        // we never should end up here, but we never know...
        qDebug() << "Uups, the Leitner System failed... return a random value...";
        return getAnyEntryFromLesson();

    }
}


void QmTrainer::setLession(int lessionIndex)
{
    m_LessionIndex = lessionIndex;
    m_lesson = QueryMeeSettings::instance()->lesson(m_LessionIndex);

    m_entries = m_lesson->entries();

    foreach(QmVocExpression* entry ,m_entries){

        // FIXME: there has to be a better solution add the cards to the "box" resp. QList
        switch ( entry->translation(m_AnswerLanguage)->grade() ) {

            case 1 :
            // Process for entry->translation(m_AnswerLanguage)->grade() = 1
                leitnerBox1.append(entry);
                m_entries.removeOne(entry);
                break;

            case 2 :
            // Process for entry->translation(m_AnswerLanguage)->grade() = 2
                leitnerBox2.append(entry);
                m_entries.removeOne(entry);
                break;

            case 3 :
            // Process for entry->translation(m_AnswerLanguage)->grade() = 3
                leitnerBox3.append(entry);
                m_entries.removeOne(entry);
                break;

            case 4 :
            // Process for entry->translation(m_AnswerLanguage)->grade() = 4
                leitnerBox4.append(entry);
                m_entries.removeOne(entry);
                break;

            case 5 :
            // Process for entry->translation(m_AnswerLanguage)->grade() = 5
                leitnerBox5.append(entry);
                m_entries.removeOne(entry);
                break;

            case 6 :
            // Process for entry->translation(m_AnswerLanguage)->grade() = 6
                leitnerBox6.append(entry);
                m_entries.removeOne(entry);
                break;

            case 7 :
            // Process for entry->translation(m_AnswerLanguage)->grade() = 7
                leitnerBox7.append(entry);
                m_entries.removeOne(entry);
                break;

            default :
            // Process for all other cases.
            // grades higher than 7 should be well know already

                if (entry->translation(m_AnswerLanguage)->grade() > 7){
                    leitnerBox7.append(entry);
                    m_entries.removeOne(entry);
                }
        }
    }
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

    if(answerCountsAsRight == true){
        // remove from the practice pool
        inPractice.removeOne(m_CorrectExp);

        qDebug() << "Grade:" << m_CorrectExp->translation(m_AnswerLanguage)->grade();

        // FIXME: there must be a easier solution
        switch ( m_CorrectExp->translation(m_AnswerLanguage)->grade() ) {

            case 0 :
            // Process = 1
                leitnerBox1.append(m_CorrectExp);
                break;

            case 1 :
            // Process = 1
                leitnerBox2.append(m_CorrectExp);
                break;

            case 2 :
            // Process = 2
                leitnerBox3.append(m_CorrectExp);
                break;

            case 3 :
            // Process = 3
                leitnerBox4.append(m_CorrectExp);
                break;

            case 4 :
            // Process = 4
                leitnerBox5.append(m_CorrectExp);
                break;

            case 5 :
            // Process = 5
                leitnerBox6.append(m_CorrectExp);
                break;

            case 6 :
            // Process = 6
                leitnerBox7.append(m_CorrectExp);
                break;

            default :
                    ;
        }

        qDebug() << "before Grade:" << m_CorrectExp->translation(m_AnswerLanguage)->grade();
        // answer was correct so increase the grade
        m_CorrectExp->translation(m_AnswerLanguage)->incGrade();
        qDebug() << "after Grade:" << m_CorrectExp->translation(m_AnswerLanguage)->grade();

    }
    else{
        // the answer was not correct in the first attempt
        // set grade back to 0 and increment the errorcount
        m_CorrectExp->translation(m_AnswerLanguage)->setGrade(0);
        m_CorrectExp->translation(m_AnswerLanguage)->incBadCount();

    }

    // store the expression so we can check later that it isn't used 2 times in a row
    m_LastExp = m_CorrectExp;

    return;
}

void QmTrainer::setRandomOnly(bool randomOnly){
    m_randomOnly = randomOnly;
}
