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

#include "tinyvoctrainer.h"
#include "tinyvoctrainersettings.h"

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

TinyVocTrainer::TinyVocTrainer(QWidget *parent) :
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

    m_QuestionLabel = new QLabel();
    m_QuestionLabel->setText(tr("Here comes..."));
    hbox->addWidget(m_QuestionLabel);
    
    answerLabel = new QLabel();
    hbox->addWidget(answerLabel);
    
    vbox_label->addLayout(hbox);
    
    statusLabel = new QLabel();
    vbox_label->addWidget(statusLabel);

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
}

void TinyVocTrainer::startTraining()
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

TinyVocTrainer::~TinyVocTrainer()
{
}

QmVocExpression * TinyVocTrainer::getAnyEntryFromLesson()
{
    QmVocExpression* vocExpression = 0;
    QmVocLesson* lesson =
    TinyVocTrainerSettings::instance()->lesson(m_LessionIndex);
    if(lesson) {
        int random_int = ( rand() %  ( lesson->entries().size() - 1 ) ) + 0;
        qDebug() << "anyEntry random_int: " << random_int << "Lesson Size: " << lesson->entries().size();
        vocExpression = lesson->entry(random_int);
    }
    return vocExpression;
}

void TinyVocTrainer::setLession(int lessionIndex)
{
    m_LessionIndex = lessionIndex;
}

void TinyVocTrainer::setQuestionLanguage(int languageIndex)
{
    m_QuestionLanguage = languageIndex;
}

void TinyVocTrainer::setAnswerLanguage(int languageIndex)
{
    m_AnswerLanguage = languageIndex;
}

void TinyVocTrainer::slotClicked(int id)
{
    qDebug() << "slotCheck(): buton id: " << id << "Correct Id: " << m_CorrectId;
    
    if(id == m_CorrectId){
        qDebug() << "\\o/ correct answer...";
        statusLabel->setText("\\o/ correct answer");
        answerLabel->setText( m_AnswerButtonsList.at(id)->text() );
        m_AnswerButtonsList.at(id)->setDown(1);
        QTimer::singleShot(2200, this, SLOT(slotInit()));
        return;
    }
    else{
        QString str = ":-( sorry wrong... it's not: ";
        str.append(m_AnswerButtonsList.at(id)->text());
        qDebug() << str ;
        statusLabel->setText( str );
    }
}

void TinyVocTrainer::slotInit(){
    m_ChoiceList.clear();
    statusLabel->clear();
    answerLabel->clear();
    
    for (int i=0; i < NumberOfButtons; ++i){
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

    int random_int = rand() % NumberOfButtons  + 0;
    qDebug() << "ask for random_int: " <<  random_int;

    if(random_int < m_ChoiceList.count() && random_int >= 0) {
        QmVocExpression* expression = m_ChoiceList.at(random_int);
        if(expression) {
            m_QuestionLabel->setText(expression->translation(m_QuestionLanguage)->text());
        }
    }
    m_CorrectId = random_int;

}

void TinyVocTrainer::closeEvent ( QCloseEvent * event )
{
    QWidget::closeEvent(event);
    this->deleteLater();
}
