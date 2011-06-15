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
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QSignalMapper>
#include <QTimer>

#include "qmmultiplechoice.h"
#include "qmtrainer.h"
#include "qmvocexpression.h"

#define NumberOfButtons 4

QmMultipleChoice::QmMultipleChoice(QWidget *parent) :
        QmTrainer(parent),
        m_CorrectId(0),
        m_firstAnswerWrong(false)
{
    QVBoxLayout *vbox = new QVBoxLayout();
    QVBoxLayout *vbox_label = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();

    vbox_label->addStretch();

    m_QuestionLabel = new QLabel();
    m_QuestionLabel->setText("Question");
    m_QuestionLabel->setWordWrap(true);
    vbox_label->addWidget(m_QuestionLabel);

    answerLabel = new QLabel();
    answerLabel->setWordWrap(true);
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

#ifndef Q_WS_MAEMO_5
    vbox->addWidget(QmTrainer::m_leitnerWidget);
#endif
    vbox->addLayout(vbox_label);
    setLayout(vbox);
}

QmMultipleChoice::~QmMultipleChoice()
{

}

void QmMultipleChoice::startTraining()
{
    if(QmTrainer::m_lessonIndexes->size() >= 1
       && QmTrainer::m_QuestionLanguage >= 0
       && QmTrainer::m_AnswerLanguage >= 0) {
        slotInit();
        show();
    } else {
        qDebug() << "Something is not selected";
    }
}

void QmMultipleChoice::slotClicked(int id)
{
    if(id == m_CorrectId){
        statusLabel->clear();
        answerLabel->setText( m_AnswerButtonsList.at(id)->text() );
        m_AnswerButtonsList.at(id)->setDown(1);

        bool countAnswerRight;

        if(m_firstAnswerWrong == false){
            countAnswerRight = true;
        }
        else {
            countAnswerRight = false;
        }


        QmTrainer::handleAnswer(countAnswerRight);
        QmTrainer::setLastAnswerRight(countAnswerRight);

        // reset the flag
        m_firstAnswerWrong = false;

        // this allows the user to see the correct answer
        // FIXME: this could be a setting!
        QTimer::singleShot(2200, this, SLOT(slotInit()));
        return;
    }
    else{
        statusLabel->setText(tr(":-( sorry wrong"));
        m_firstAnswerWrong = true;
    }
}


void QmMultipleChoice::slotInit(){

    m_ChoiceList.clear();
    statusLabel->clear();
    answerLabel->clear();


    int random_int = QmTrainer::randomInt(0, NumberOfButtons);

    m_AnswerButtonsList.at(random_int)->setDown(0);
    QmVocExpression* expression =  QmTrainer::getNextEntry();

    if(expression) {
        m_QuestionLabel->setText(expression->translation(QmTrainer::m_QuestionLanguage)->text());
        m_AnswerButtonsList.at(random_int)->setText( expression->translation(QmTrainer::m_AnswerLanguage)->text() );
        QmTrainer::m_CorrectExp = expression;
        m_ChoiceList.append(expression);
    }

    m_CorrectId = random_int;

    for (int i=0; i < NumberOfButtons; ++i){
        if(i != random_int){
           m_AnswerButtonsList.at(i)->setDown(0);
           QmVocExpression* expression = NULL;
           do {
               expression = QmTrainer::getAnyEntryFromLesson();
           } while (m_ChoiceList.contains(expression));
           if(expression) {
               m_ChoiceList.append(expression);
               m_AnswerButtonsList.at(i)->setText( expression->translation(QmTrainer::m_AnswerLanguage)->text() );
           }
        }
    }
}

void QmMultipleChoice::closeEvent ( QCloseEvent * event )
{
    QWidget::closeEvent(event);
    this->deleteLater();
}
