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

#ifndef LESSONCHOOSEWIDGET_H
#define LESSONCHOOSEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QCloseEvent>
#include "querymeesettings.h"
#include "trainingselectionview.h"

class LessonChooseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LessonChooseWidget(TrainingSelectionView *parent = 0, QueryMeeSettings *settings = 0);
    ~LessonChooseWidget();

signals:
    void signalSelectAll();
    void signalClearAll();

public slots:
    void toggleSelection();

private:
    QListWidget *listWidget;
    QueryMeeSettings *m_settings;
    TrainingSelectionView *m_parent;
    void updateListWidget();
    bool m_isAllSelected;
    void closeEvent(QCloseEvent *);

};

#endif // LESSONCHOOSEWIDGET_H
