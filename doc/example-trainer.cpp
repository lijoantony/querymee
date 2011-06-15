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

#include "example-trainer.h"


QmEXAMPLE::QmEXAMPLE(QWidget *parent) :
    QmTrainer(parent)
{
    // for non maemo views you might add the leitner progressbars
    // assuming you have a QVBoxLayout *vbox!
#ifndef Q_WS_MAEMO_5
    vbox->addWidget(QmTrainer::m_leitnerWidget);
#endif
}

QmEXAMPLE::~QmEXAMPLE(){

}

void QmEXAMPLE::startTraining(){
    if(QmTrainer::m_lessonIndexes->size() >= 1
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
