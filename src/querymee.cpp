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

#include <ctime>

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QSignalMapper>
#include <QTimer>

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

    qDebug() << "inPractice:" << inPractice.count();
    qDebug() << "m_entries" << m_entries.count() << __LINE__;
    qDebug() << "leitnerBox1" << leitnerBox1.count() << __LINE__;
    qDebug() << "leitnerBox2" << leitnerBox2.count() << __LINE__;
    qDebug() << "leitnerBox3" << leitnerBox3.count() << __LINE__;
    qDebug() << "leitnerBox4" << leitnerBox4.count() << __LINE__;
    qDebug() << "leitnerBox5" << leitnerBox5.count() << __LINE__;
    qDebug() << "leitnerBox6" << leitnerBox6.count() << __LINE__;
    qDebug() << "leitnerBox7" << leitnerBox7.count() << __LINE__;

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

            qDebug() << "Entry: " << vocExpression->translation(m_QuestionLanguage)->text();
            qDebug() << "inPractice: " << inPractice.count() << "leitnerBox1:" << leitnerBox1.count();

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

            qDebug() << "Entry: " << vocExpression->translation(m_QuestionLanguage)->text();
            qDebug() << "inPractice: " << inPractice.count() << "leitnerBox2:" << leitnerBox2.count();

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

            qDebug() << "Entry: " << vocExpression->translation(m_QuestionLanguage)->text();
            qDebug() << "inPractice: " << inPractice.count() << "leitnerBox3:" << leitnerBox3.count();

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

            qDebug() << "Entry: " << vocExpression->translation(m_QuestionLanguage)->text();
            qDebug() << "inPractice: " << inPractice.count() << "leitnerBox4:" << leitnerBox4.count();

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

            qDebug() << "Entry: " << vocExpression->translation(m_QuestionLanguage)->text();
            qDebug() << "inPractice: " << inPractice.count() << "leitnerBox5:" << leitnerBox5.count();

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

            qDebug() << "Entry: " << vocExpression->translation(m_QuestionLanguage)->text();
            qDebug() << "inPractice: " << inPractice.count() << "leitnerBox6:" << leitnerBox6.count();

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

            qDebug() << "Entry: " << vocExpression->translation(m_QuestionLanguage)->text();
            qDebug() << "inPractice: " << inPractice.count() << "leitnerBox7:" << leitnerBox7.count();

        } while ( inPractice.count() < MaxGrade0);

        do {
            vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
        } while (m_LastExp == vocExpression);

        return vocExpression;
    }
    else if (inPractice.count() == 5){
        qDebug() << "-- no new entry inPractice needed...";

        do {
            vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
        } while (m_LastExp == vocExpression);

        return vocExpression;
    }

    // we never should end up here, but we never know...
    qDebug() << "we never should end up here...";
    return getAnyEntryFromLesson();
}


void QueryMee::setLession(int lessionIndex)
{
    m_LessionIndex = lessionIndex;
    m_lesson = QueryMeeSettings::instance()->lesson(m_LessionIndex);

    m_entries = m_lesson->entries();

    foreach(QmVocExpression* entry ,m_entries){
        // qDebug() << entry->translation(m_QuestionLanguage)->leitnerBox()->name();
        qDebug() <<  entry->translation(m_QuestionLanguage)->text() << entry->translation(m_AnswerLanguage)->grade();

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
                if (entry->translation(m_AnswerLanguage)->grade() > 7){
                    leitnerBox7.append(entry);
                    m_entries.removeOne(entry);
                }
        }
    }

    qDebug() << "m_entries" << m_entries.count() << __LINE__;
    qDebug() << "leitnerBox1" << leitnerBox1.count() << __LINE__;
    qDebug() << "leitnerBox2" << leitnerBox2.count() << __LINE__;
    qDebug() << "leitnerBox3" << leitnerBox3.count() << __LINE__;
    qDebug() << "leitnerBox4" << leitnerBox4.count() << __LINE__;
    qDebug() << "leitnerBox5" << leitnerBox5.count() << __LINE__;
    qDebug() << "leitnerBox6" << leitnerBox6.count() << __LINE__;
    qDebug() << "leitnerBox7" << leitnerBox7.count() << __LINE__;

}

void QueryMee::setQuestionLanguage(int languageIndex)
{
    m_QuestionLanguage = languageIndex;
}

void QueryMee::setAnswerLanguage(int languageIndex)
{
    m_AnswerLanguage = languageIndex;
}

void QueryMee::slotClicked(int id)
{
    if(id == m_CorrectId){
        statusLabel->setText("\\o/ correct answer");
        answerLabel->setText( m_AnswerButtonsList.at(id)->text() );
        m_AnswerButtonsList.at(id)->setDown(1);
        qDebug() << "Correct Exp: " << m_CorrectExp->translation(m_QuestionLanguage)->text();
        qDebug() << "m_firstAnswerWrong" << m_firstAnswerWrong;

        if(m_firstAnswerWrong == 0){

            inPractice.removeOne(m_CorrectExp);

            qDebug() << "inPractice: " << inPractice.count();

            qDebug() << "Grade: " << m_CorrectExp->translation(m_AnswerLanguage)->grade();

            switch ( m_CorrectExp->translation(m_AnswerLanguage)->grade() ) {

                case 0 :
                // Process = 1
                    qDebug() << __LINE__;
                    leitnerBox1.append(m_CorrectExp);
                    m_CorrectExp->translation(m_AnswerLanguage)->setGrade(1);
                    break;

                case 1 :
                // Process = 1
                    qDebug() << __LINE__;
                    leitnerBox2.append(m_CorrectExp);
                    m_CorrectExp->translation(m_AnswerLanguage)->setGrade(2);
                    break;

                case 2 :
                // Process = 2
                    qDebug() << __LINE__;
                    leitnerBox3.append(m_CorrectExp);
                    m_CorrectExp->translation(m_AnswerLanguage)->setGrade(3);
                    break;

                case 3 :
                // Process = 3
                    qDebug() << __LINE__;
                    leitnerBox4.append(m_CorrectExp);
                    m_CorrectExp->translation(m_AnswerLanguage)->setGrade(4);
                    break;

                case 4 :
                // Process = 4
                    qDebug() << __LINE__;
                    leitnerBox5.append(m_CorrectExp);
                    m_CorrectExp->translation(m_AnswerLanguage)->setGrade(5);
                    break;

                case 5 :
                // Process = 5
                    qDebug() << __LINE__;
                    leitnerBox6.append(m_CorrectExp);
                    m_CorrectExp->translation(m_AnswerLanguage)->setGrade(6);
                    break;

                case 6 :
                // Process = 6
                    qDebug() << __LINE__;
                    leitnerBox7.append(m_CorrectExp);
                    m_CorrectExp->translation(m_AnswerLanguage)->setGrade(7);
                    break;

                default :
                        qDebug() << __LINE__;
                        ;
                // Process for all other cases.
//                    if (entry->translation(m_AnswerLanguage)->grade() > 7){
//                        leitnerBox7.append(entry);
//                        leitnerBox1.removeOne(entry);
//                    }
            }


        qDebug() << "m_entries" << m_entries.count() << __LINE__;
        qDebug() << "leitnerBox1" << leitnerBox1.count() << __LINE__;
        qDebug() << "leitnerBox2" << leitnerBox2.count() << __LINE__;
        qDebug() << "leitnerBox3" << leitnerBox3.count() << __LINE__;
        qDebug() << "leitnerBox4" << leitnerBox4.count() << __LINE__;
        qDebug() << "leitnerBox5" << leitnerBox5.count() << __LINE__;
        qDebug() << "leitnerBox6" << leitnerBox6.count() << __LINE__;
        qDebug() << "leitnerBox7" << leitnerBox7.count() << __LINE__;

        }
        else{
            // the answer was not correct in the first attempt
            // set grade back to 0
            m_CorrectExp->translation(m_AnswerLanguage)->setGrade(0);
        }

        m_firstAnswerWrong = 0;
        QTimer::singleShot(2200, this, SLOT(slotInit()));
        m_LastExp = m_CorrectExp;
        return;
    }
    else{
        m_firstAnswerWrong = 1;
        QString str = ":-( sorry wrong... it's not: ";
        str.append(m_AnswerButtonsList.at(id)->text());
        statusLabel->setText( str );
        m_LastExp = m_CorrectExp;
    }


    qDebug() << "m_entries" << m_entries.count() << __LINE__;
    qDebug() << "leitnerBox1" << leitnerBox1.count() << __LINE__;
    qDebug() << "leitnerBox2" << leitnerBox2.count() << __LINE__;
    qDebug() << "leitnerBox3" << leitnerBox3.count() << __LINE__;
    qDebug() << "leitnerBox4" << leitnerBox4.count() << __LINE__;
    qDebug() << "leitnerBox5" << leitnerBox5.count() << __LINE__;
    qDebug() << "leitnerBox6" << leitnerBox6.count() << __LINE__;
    qDebug() << "leitnerBox7" << leitnerBox7.count() << __LINE__;

}

void QueryMee::slotInit(){
    m_ChoiceList.clear();
    statusLabel->clear();
    answerLabel->clear();
    


    int random_int = randomInt(0, NumberOfButtons) ;

    m_AnswerButtonsList.at(random_int)->setDown(0);
    // QmVocExpression* expression = m_ChoiceList.at(random_int);
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
           // qDebug() << "returned entry" << getNextEntry()->translation(m_QuestionLanguage)->text();
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
//    qDebug() << "RandomInt Min: " << min;
//    qDebug() << "RandomInt Max: " << max;
    int i = 0;
    if (max != 0){
        i = ( rand() %  max ) + min ;
    }
    else{
        i = min;
    }
//    qDebug() << "RandomInt: " << i;
    return i;
}
