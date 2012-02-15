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

#include <QWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

#include "qmflashcard.h"
#include "qmtrainer.h"
#include "qmvocexpression.h"

QmFlashCard::QmFlashCard(QWidget *parent) :
    QmTrainer(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();
    QVBoxLayout *vbox_frontside = new QVBoxLayout();
    QVBoxLayout *vbox_backside = new QVBoxLayout();

    QHBoxLayout *hbox_button = new QHBoxLayout();

    label_index = new QLabel();
    label_frontside = new QLabel("Front Side");
    label_frontside->setWordWrap(true);
    label_backside = new QLabel("Back Side");
    label_backside->setWordWrap(true);

    //set font
    QFont font;
    font.setPointSize(32);
    //font.setBold(true);
    label_frontside->setFont(font);

    button_showBackSide = new QPushButton();
    button_showBackSide->setText(tr("show answer"));

    connect(button_showBackSide,
            SIGNAL(clicked()),
            this,
            SLOT(button_showBackSideClicked()));

    button_correct = new QPushButton();
    button_correct->setText(tr("I knew"));
    button_correct->hide();

    connect(button_correct,
            SIGNAL(clicked()),
            this,
            SLOT(button_correctClicked()));

    button_wrong = new QPushButton();
    button_wrong->setText(tr("I didn't know"));
    button_wrong->hide();

    connect(button_wrong,
            SIGNAL(clicked()),
            this,
            SLOT(button_wrongClicked()));

    vbox_frontside->addStretch();
    vbox_frontside->addStretch();
    vbox_frontside->addStretch();
    vbox_frontside->addWidget(label_index);
    vbox_frontside->addWidget(label_frontside);
    vbox_frontside->setAlignment(label_frontside, Qt::AlignHCenter);
    vbox_frontside->addStretch();

    vbox_backside->addWidget(label_backside);
    vbox_backside->addStretch();

    hbox_button->addWidget(button_correct);
    hbox_button->addWidget(button_wrong);

#ifndef Q_WS_MAEMO_5
    vbox->addWidget(QmTrainer::m_leitnerWidget);
#endif
    vbox->addLayout(vbox_frontside);
    vbox->addLayout(vbox_backside);
    vbox->addWidget(button_showBackSide);
    vbox->addLayout(hbox_button);

    setLayout(vbox);
}

QmFlashCard::~QmFlashCard(){

}

void QmFlashCard::startTraining(){
    if(QmTrainer::m_lessonIndexes->size() >= 1
       && QmTrainer::m_QuestionLanguage >= 0
       && QmTrainer::m_AnswerLanguage >= 0) {
        init();
        show();
    } else {
        qDebug() << "Something is not selected";
    }
}

void QmFlashCard::init(){

    static int index = 0;

    label_index->clear();
    label_frontside->clear();
    label_backside->clear();

    QmTrainer::m_CorrectExp =  QmTrainer::getNextEntry();

    ++index;
    label_index->setText(QString::number(index));

    label_frontside->setText(QmTrainer::m_CorrectExp->translation(QmTrainer::m_QuestionLanguage)->text());

}

void QmFlashCard::button_showBackSideClicked(){

    QString *backside = new QString(QmTrainer::m_CorrectExp
                                    ->translation(QmTrainer::m_AnswerLanguage)
                                    ->text());

    QString *comperative = new QString(QmTrainer::m_CorrectExp
                                       ->translation(QmTrainer::m_AnswerLanguage)
                                       ->comparative());

    QString *superlative = new QString(QmTrainer::m_CorrectExp
                                       ->translation(QmTrainer::m_AnswerLanguage)
                                       ->superlative());

    if(comperative->size() > 0){
        backside->append("\n").append(comperative);
    }

    if(superlative->size() > 0){
        backside->append("\n").append(superlative);
    }

    label_backside->setText(*backside);

    button_wrong->show();
    button_correct->show();
    button_showBackSide->hide();
}

void QmFlashCard::button_correctClicked(){

    QmTrainer::handleAnswer(true);
    QmTrainer::setLastAnswerRight(true);

    button_wrong->hide();
    button_correct->hide();
    button_showBackSide->show();

    QTimer::singleShot(200, this, SLOT(init()));
    return;
}

void QmFlashCard::button_wrongClicked(){

    QmTrainer::handleAnswer(false);
    QmTrainer::setLastAnswerRight(false);

    button_wrong->hide();
    button_correct->hide();
    button_showBackSide->show();

    QTimer::singleShot(200, this, SLOT(init()));
    return;
}

void QmFlashCard::closeEvent ( QCloseEvent * event )
{
    QWidget::closeEvent(event);
    this->deleteLater();
}
