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

#include <ctime>

#include <QApplication>
#include <QtDebug>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QList>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>

TinyVocTrainer::TinyVocTrainer(QWidget *parent, const QString &fileName)
    : QWidget(parent)
{
    srand(time(NULL));

    QVBoxLayout *vbox = new QVBoxLayout();
    QVBoxLayout *vbox_label = new QVBoxLayout();

    bgroup_choice =  new QButtonGroup();

    QuestionLabel = new QLabel();
    QuestionLabel->setText("Here comes...");
    vbox_label->addWidget(QuestionLabel);

    answer1 = new QPushButton;
    answer2 = new QPushButton;
    answer3 = new QPushButton;
    answer4 = new QPushButton;

    vbox_label->addWidget(answer1);
    vbox_label->addWidget(answer2);
    vbox_label->addWidget(answer3);
    vbox_label->addWidget(answer4);

    answerButtonsList.append(answer1);
    answerButtonsList.append(answer2);
    answerButtonsList.append(answer3);
    answerButtonsList.append(answer4);

    bgroup_choice->addButton(answer1);
    bgroup_choice->addButton(answer2);
    bgroup_choice->addButton(answer3);
    bgroup_choice->addButton(answer4);

    bgroup_choice->setId(answer1, 0);
    bgroup_choice->setId(answer2, 1);
    bgroup_choice->setId(answer3, 2);
    bgroup_choice->setId(answer4, 3);

    connect(answer1,SIGNAL(clicked(bool)),this,SLOT(slotAnswer1(bool)));
    connect(answer2,SIGNAL(clicked(bool)),this,SLOT(slotAnswer2(bool)));
    connect(answer3,SIGNAL(clicked(bool)),this,SLOT(slotAnswer3(bool)));
    connect(answer4,SIGNAL(clicked(bool)),this,SLOT(slotAnswer4(bool)));

    questionID = 0;
    answerID = 1;
    lessonID = 0;
    CorrectID = 0;

    QTvtVocDocument *docRead = new QTvtVocDocument();
    docRead->open(fileName);

    lessons = docRead->lesson()->childContainers();

    int lessonId = 0;
    foreach(QTvtVocContainer * c, lessons) {
        if (c->containerType() == QTvtVocLesson::Lesson) {
                lessonsList.append( static_cast<QTvtVocLesson *>(c) );
                QTvtVocLesson *m_lesson;
                m_lesson = lessonsList.last() ;
                qDebug () << "Lesson: " << m_lesson->name();
        }
        ++lessonId;
    }

    vbox->addLayout(vbox_label);
    setLayout(vbox);

    dialog = new TinyVocTrainerSettings(this, fileName);

    connect(dialog,SIGNAL(SignalToggleAnswer(int)),this,SLOT(reactToToggleAnswer(int)));
    connect(dialog,SIGNAL(SignalToggleLesson(int)),this,SLOT(reactToToggleLesson(int)));
    connect(dialog,SIGNAL(SignalToggleQuestion(int)),this,SLOT(reactToToggleQuestion(int)));

    slotSettings();
}

TinyVocTrainer::~TinyVocTrainer()
{

}

QTvtVocExpression * TinyVocTrainer::getAnyEntryFromLesson(QTvtVocLesson *lesson, int language)
{
    Q_UNUSED(language);
    int random_int = ( rand() %  ( lesson->entries().size() - 1 ) ) + 0;
    qDebug() << "anyEntry random_int: " << random_int << "Lesson Size: " << lesson->entries().size();
    return lesson->entry(random_int);
}



void TinyVocTrainer::reactToToggleQuestion(int id)
{ 
    qDebug() << "Toggle Question: " << id;
    questionID = id;
}


void TinyVocTrainer::reactToToggleAnswer(int id)
{
    qDebug() << "Toggle Answer: " << id;
    answerID = id;
}


void TinyVocTrainer::reactToToggleLesson(int id)
{
    qDebug() << "Toggle Lesson: " << id;
    lessonID = id;
}

void TinyVocTrainer::slotAnswer1(bool clicked){
    Q_UNUSED(clicked);
    slotCheck(0);
}

void TinyVocTrainer::slotAnswer2(bool clicked){
    Q_UNUSED(clicked);
    slotCheck(1);
}

void TinyVocTrainer::slotAnswer3(bool clicked){
    Q_UNUSED(clicked);
    slotCheck(2);
}

void TinyVocTrainer::slotAnswer4(bool clicked){
    Q_UNUSED(clicked);
    slotCheck(3);
}

void TinyVocTrainer::slotCheck(int buttonID){
    qDebug() << "slotCheck(): buton id: " << buttonID << "Correct Id: " << CorrectID;

    if(buttonID == CorrectID){
        qDebug() << "\\o/ correct answer...";
        slotInit(1);
        return;
    }
    else{
        qDebug() << ":-( sorry wrong...";
    }

}

void TinyVocTrainer::slotInit(bool clicked){

    Q_UNUSED(clicked);

    choiceList.clear();
    for (int i=0; i < 4; ++i){
        choiceList.append( getAnyEntryFromLesson(lessonsList.at(lessonID), answerID) );
        answerButtonsList.at(i)->setText( choiceList.at(i)->translation(answerID)->text() );
    }

    int random_int = rand() %  3  + 0;
    qDebug() << "ask for random_int: " <<  random_int;

    QuestionLabel->setText(choiceList.at(random_int)->translation(questionID)->text());
    CorrectID = random_int;

}

void TinyVocTrainer::slotSettings(bool clicked){
    Q_UNUSED(clicked);

    dialog->exec();
    slotInit();
}
