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

    if (inPractice.count() < MaxGrade0 && m_entries.count() > 0){
        int i = 0;

        do{
            do {
                 vocExpression = m_entries.at( randomInt(0, m_entries.count() - 1) );
                 i++;
            } while (vocExpression->translation(m_QuestionLanguage)->grade() != 0 || i > m_entries.count() );

            inPractice.append(vocExpression);
            m_entries.removeOne(vocExpression);
            qDebug() << "Entry: " << vocExpression->translation(m_QuestionLanguage)->text();
            qDebug() << "inPractice: " << inPractice.count() << "m_entries:" << m_entries.count();
        } while ( inPractice.count() < MaxGrade0);

        do {
            vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
        } while (m_LastExp == vocExpression);

        return vocExpression;

    } else {

        do {
            vocExpression = inPractice.at(randomInt(0, MaxGrade0 - 1));
        } while (m_LastExp == vocExpression);

        return vocExpression;
    }

    return getAnyEntryFromLesson();
}


void QueryMee::setLession(int lessionIndex)
{
    m_LessionIndex = lessionIndex;
    m_lesson = QueryMeeSettings::instance()->lesson(m_LessionIndex);

    m_entries = m_lesson->entries();

//    foreach(QmVocExpression* entry ,m_entries){
//       qDebug() << entry->translation(0)->leitnerBox()->name();
//       qDebug() << entry->translation(0)->grade();
//    }

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
    }
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
