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

#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QCloseEvent>

#include "qmvoclesson.h"
#include "querymeesettings.h"
#include "lessonchoosewidget.h"
#include "trainingselectionview.h"

LessonChooseWidget::LessonChooseWidget(TrainingSelectionView *parent, QueryMeeSettings *settings) :
    QWidget(parent)
{
    m_settings = settings;
    m_parent = parent;
    m_isAllSelected = false;
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *hbox = new QHBoxLayout(this);

    QPushButton *doneButton = new QPushButton(this);
    doneButton->setText(tr("Done"));
    hbox->addWidget(doneButton);

    connect(doneButton,
            SIGNAL(clicked()),
            this,
            SLOT(close()));

    QPushButton *selectButton = new QPushButton(this);
    selectButton->setText(tr("Toggle Selection"));
    hbox->addWidget(selectButton);

    vbox->addLayout(hbox);

    listWidget = new QListWidget(this);
    listWidget->setSortingEnabled(true);
    listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(selectButton,
            SIGNAL(clicked()),
            this,
            SLOT(toggleSelection()));

    connect(this,
            SIGNAL(signalClearAll()),
            listWidget,
            SLOT(clearSelection()));

    connect(this,
            SIGNAL(signalSelectAll()),
            listWidget,
            SLOT(selectAll()));

    vbox->addWidget(listWidget);

    updateListWidget();

    setLayout(vbox);

}

LessonChooseWidget::~LessonChooseWidget(){

}

void LessonChooseWidget::updateListWidget(){
    int counter = 0;
    foreach (QString lesson, m_settings->lessons()){
        QListWidgetItem *item = new QListWidgetItem();
        listWidget->addItem(item);
        item->setText(lesson);
        item->setData(33, counter);
        if(counter == 0){
            item->setSelected(true);
        }
        counter++;
    }
}

void LessonChooseWidget::toggleSelection(){
    if(m_isAllSelected){
        emit signalClearAll();
        m_isAllSelected = false;
    } else {
        emit signalSelectAll();
        m_isAllSelected = true;
    }
}

void LessonChooseWidget::closeEvent(QCloseEvent *event)
 {
    QList<int> *selectedLessons = new QList<int>();
    foreach (QListWidgetItem *item, listWidget->selectedItems()){
        selectedLessons->append((item->data(33).toInt()));
    }

    if(selectedLessons->size() >= 1){
        m_parent->setTrainingsLessons(selectedLessons);
    } else {
        selectedLessons->append(0);
        m_parent->setTrainingsLessons(selectedLessons);
    }
    event->accept();
 }
