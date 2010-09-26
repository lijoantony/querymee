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

#include <QWidget>
#include <QDebug>

#include "example-trainer.h"


QmEXAMPLE::QmEXAMPLE(QWidget *parent) :
    QmTrainer(parent)
{
}

QmEXAMPLE::~QmEXAMPLE(){

}

void QmEXAMPLE::startTraining(){
    if(QmTrainer::m_LessionIndex >= 0
       && QmTrainer::m_QuestionLanguage >= 0
       && QmTrainer::m_AnswerLanguage >= 0) {

        qDebug() << "Call here the methode to start the excercise...";
        show();

    } else {
        qDebug() << "Something is not selected";
    }
}

void QmEXAMPLE::closeEvent ( QCloseEvent * event )
{
    QWidget::closeEvent(event);
    this->deleteLater();
}
