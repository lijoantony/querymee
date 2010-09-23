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

#include "querymee.h"
#include "querymeesettings.h"
#include "version.h"

#include <ctime>

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QSignalMapper>
#include <QTimer>
#include <QDateTime>

#include "qmvocdocument.h"
#include "qmvoclesson.h"
#include "qmvocexpression.h"
#include "qmvocleitnerbox.h"

#define NumberOfButtons 4
#define MaxGrade0 5

QueryMee::QueryMee(QWidget *parent) :
        QWidget(parent),
        m_CorrectId(0),
        m_LessionIndex(-1),
        m_QuestionLanguage(-1),
        m_AnswerLanguage(-1)
{
    srand(time(NULL));

    QVBoxLayout *vbox = new QVBoxLayout();
    QVBoxLayout *vbox_label = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();

    vbox_label->addStretch();

    m_QuestionLabel = new QLabel();
    m_QuestionLabel->setText(tr("Here comes..."));
    vbox_label->addWidget(m_QuestionLabel);
    
    answerLabel = new QLabel();
    hbox->addWidget(answerLabel);
    
    statusLabel = new QLabel();
    hbox->addWidget(statusLabel);

    vbox_label->addLayout(hbox);
    vbox_label->addStretch();

    QSignalMapper* signalMapper = new QSignalMapper(this);
    for (int i = 0; i < NumberOfButtons; i++) {
        QPushButton *answer = new QPushButton;
        vbox_label->addWidget(answer);
        m_AnswerButtonsList.append(answer);
        connect(answer, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(answer, i);
        }

    connect(signalMapper, SIGNAL(mapped(int)), SLOT(slotClicked(int)));

    vbox->addLayout(vbox_label);
    setLayout(vbox);

    m_firstAnswerWrong = 0;
    m_LastExp = new QmVocExpression();

}

void QueryMee::startTraining()
{
    if(m_LessionIndex >= 0
       && m_QuestionLanguage >= 0
       && m_AnswerLanguage >= 0) {
        slotInit();
        show();
    } else {
        qDebug() << "Something is not selected";
    }
}

QueryMee::~QueryMee()
{

    // FIXME: should we ask the user if overwritting is OK?
    // FIXME: is it OK to have it in the destructor?
    qDebug() << "We are gonna overwrite the file, should we ask the user?";
    m_QmVocDocument->saveAs(QUrl(m_CurrentFileName),
                            QmVocDocument::Kvtml,
                            QString("Querymee ").append(QM_VERSION).append(" build date: ").append(__DATE__));
}


QmVocExpression * QueryMee::getAnyEntryFromLesson()
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


QmVocExpression * QueryMee::getNextEntry()
{
    QmVocExpression* vocExpression = 0;

    // FIXME: there has to be a better solution for that?!

    /******************************************************************************
    **
    ** Pool
    **
    ******************************************************************************/
    if (inPractice.count() < MaxGrade0 && m_entries.count() > 0){
        qDebug() << "Pool";

        do{           

            vocExpression = m_entries.at( randomInt(0, m_entries.count() - 1) );

            inPractice.append(vocExpression);
            m_entries.removeOne(vocExpression);

            qDebug() << "Entry: " << vocExpression->translation(m_QuestionLanguage)->text();
            qDebug() << "inPractice: " << inPractice.count() << "m_entries:" << m_entries.count();

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
        qDebug() << "-- leitnerBox1";

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
        qDebug() << "-- leitnerBox2";

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
        qDebug() << "-- leitnerBox3";

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
        qDebug() << "-- leitnerBox4";

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
        qDebug() << "-- leitnerBox5";

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
        qDebug() << "-- leitnerBox6";

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
        qDebug() << "-- leitnerBox7";

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
    else if (inPractice.count() == 5){

        do {
            vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
        } while (m_LastExp == vocExpression);

        return vocExpression;
    }

    // we never should end up here, but we never know...
    qDebug() << "Uups, the Leitner System failed... return a random value...";
    return getAnyEntryFromLesson();
}


void QueryMee::setLession(int lessionIndex)
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

void QueryMee::slotClicked(int id)
{
    if(id == m_CorrectId){

        // show the user the answer was right
        statusLabel->setText("\\o/ correct answer");
        answerLabel->setText( m_AnswerButtonsList.at(id)->text() );
        m_AnswerButtonsList.at(id)->setDown(1);

        // update the time when this expression was practiced
        m_CorrectExp->translation(m_AnswerLanguage)->setPracticeDate(QDateTime::currentDateTime());
        // increment the practice counter
        m_CorrectExp->translation(m_AnswerLanguage)->incPracticeCount();

        if(m_firstAnswerWrong == 0){

            // remove from the practice pool
            inPractice.removeOne(m_CorrectExp);

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

            // answer was correct so increase the grade
            m_CorrectExp->translation(m_AnswerLanguage)->incGrade();

        }
        else{
            // the answer was not correct in the first attempt
            // set grade back to 0 and increment the errorcount
            m_CorrectExp->translation(m_AnswerLanguage)->setGrade(0);
            m_CorrectExp->translation(m_AnswerLanguage)->incBadCount();

        }

        // reset the flag
        m_firstAnswerWrong = false;

        // this allows the user to see the correct answer
        // FIXME: this could be a setting!
        QTimer::singleShot(2200, this, SLOT(slotInit()));

        // store the expression so we can check later that it isn't used 2 times in a row
        m_LastExp = m_CorrectExp;

        return;
    }
    else{
        // set the flag so the grade will not be incremented
        m_firstAnswerWrong = true;

        // show the user that the answer was wrong
        QString str = ":-( sorry wrong... it's not: ";
        str.append(m_AnswerButtonsList.at(id)->text());
        statusLabel->setText( str );

        return;
    }

}

void QueryMee::slotInit(){

    m_ChoiceList.clear();
    statusLabel->clear();
    answerLabel->clear();
    

    int random_int = randomInt(0, NumberOfButtons);

    m_AnswerButtonsList.at(random_int)->setDown(0);
    QmVocExpression* expression =  getNextEntry();
    if(expression) {
        m_QuestionLabel->setText(expression->translation(m_QuestionLanguage)->text());
        m_AnswerButtonsList.at(random_int)->setText( expression->translation(m_AnswerLanguage)->text() );
        m_CorrectExp = expression;
        m_ChoiceList.append(expression);
    }

    m_CorrectId = random_int;

    for (int i=0; i < NumberOfButtons; ++i){
        if(i != random_int){
           m_AnswerButtonsList.at(i)->setDown(0);
           QmVocExpression* expression = NULL;
           do {
               expression = getAnyEntryFromLesson();
           } while (m_ChoiceList.contains(expression));
           if(expression) {
               m_ChoiceList.append(expression);
               m_AnswerButtonsList.at(i)->setText( expression->translation(m_AnswerLanguage)->text() );
           }
        }
    }
}

void QueryMee::closeEvent ( QCloseEvent * event )
{
    QWidget::closeEvent(event);
    this->deleteLater();
}

int QueryMee::randomInt(int min, int max){
    int i = 0;

    if (max != 0){
        i = ( rand() %  max ) + min ;
    }
    else{
        i = min;
    }

    return i;
}

void QueryMee::setCurrentFileName(QString fileName){
    m_CurrentFileName = fileName;
}

void QueryMee::setQuestionLanguage(int languageIndex)
{
    m_QuestionLanguage = languageIndex;
}

void QueryMee::setAnswerLanguage(int languageIndex)
{
    m_AnswerLanguage = languageIndex;
}

void QueryMee::setQmVocDocument(QmVocDocument *doc){
    m_QmVocDocument = doc;
}
