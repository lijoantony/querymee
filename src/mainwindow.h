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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QFlags>

#include "trainingselectionview.h"
#include "querymeesettings.h"
#include "downloadwidget.h"

class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
//    TrainingSelectionView* trainingView;
//    DownloadWidget *dlw;
signals:
    void updateFileList();

public slots:
    void downloadFiles();

private:
    QAction *settingAction;
    QAction *downloadAction;
    QMenu *mainMenu;

    void createActions();
    void createMenus();
};

#endif // MAINWINDOW_H
