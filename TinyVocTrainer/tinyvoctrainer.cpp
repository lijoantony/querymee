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

#include "qtvtvocdocument.h"
#include "qtvtvoclesson.h"
#include "qtvtvocexpression.h"
#include "qtvtvocleitnerbox.h"

#define NumberOfButtons 4

TinyVocTrainer::TinyVocTrainer(QWidget *parent) :
        QWidget(parent),
        m_CorrectId(0),
        m_LessionIndex(0),
        m_QuestionLanguage(0),
        m_AnswerLanguage(0)
{
    srand(time(NULL));

    QVBoxLayout *vbox = new QVBoxLayout();
    QVBoxLayout *vbox_label = new QVBoxLayout();

    m_QuestionLabel = new QLabel();
    m_QuestionLabel->setText(tr("Here comes..."));
    vbox_label->addWidget(m_QuestionLabel);

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
    slotInit();
    show();
}

TinyVocTrainer::~TinyVocTrainer()
{
}

QTvtVocExpression * TinyVocTrainer::getAnyEntryFromLesson()
{
    QTvtVocExpression* vocExpression = 0;
    QTvtVocLesson* lesson =
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
        slotInit();
        return;
    }
    else{
        qDebug() << ":-( sorry wrong...";
    }
}

void TinyVocTrainer::slotInit(){
    m_ChoiceList.clear();
    for (int i=0; i < NumberOfButtons; ++i){
        QTvtVocExpression* expression = NULL;
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
        QTvtVocExpression* expression = m_ChoiceList.at(random_int);
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
