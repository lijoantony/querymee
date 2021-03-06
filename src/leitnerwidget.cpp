/***************************************************************************

    Copyright 2011 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QHBoxLayout>
#include <QProgressBar>
#include <QList>
#include <QLabel>
#include <QDebug>

#include "leitnerwidget.h"
#include "querymee_defaults.h"

LeitnerWidget::LeitnerWidget(QWidget *parent) :
    QWidget(parent)
{ 
    // see querymee_defaults.h
    MAXENTRIES;

    QHBoxLayout *hbox = new QHBoxLayout(this);
    progressBarsList = new QList<QProgressBar*>();

    for(int i=0; i<= NumberOfLeitnerBoxes;i++){
        QProgressBar *box = new QProgressBar();
        progressBarsList->insert(i, box);
        hbox->addWidget(progressBarsList->at(i));
        if (i < NumberOfLeitnerBoxes){
            progressBarsList->at(i)->setMaximum(maxEntries[i]);
            progressBarsList->at(i)->setToolTip(QString("Leitnerbox %1").arg(i));
        }
        qDebug() << i << "max:" << progressBarsList->at(i)->maximum();
    }

    progressBarsList->at(0)->setToolTip(QString("Pool"));
    progressBarsList->at(NumberOfLeitnerBoxes)->setToolTip(QString("Leitnerbox %1").arg(NumberOfLeitnerBoxes));

    setLayout(hbox);
}

void LeitnerWidget::updateProgressBar(int index,int value){
    progressBarsList->at(index)->setValue(value);
}

void LeitnerWidget::updateMaxProgressBar(int index,int value){
    progressBarsList->at(index)->setMaximum(value);
}
