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

#include "mainwindow.h"

MainWindow::MainWindow()
{
    TrainingSelectionView* trainingView = new TrainingSelectionView(this);

    createActions();
    createMenus();
    setCentralWidget(trainingView);

}

void MainWindow::createActions(){


    settingAction = new QAction(tr("Open dictionary"), this);
    connect(settingAction,
            SIGNAL(triggered()),
            QueryMeeSettings::instance(),
            SLOT(openDictionary()));


    downloadAction = new QAction(tr("Download KVTML files"), this);
    connect(downloadAction,
            SIGNAL(triggered()),
            this,
            SLOT(downloadFiles()));
}

void MainWindow::createMenus(){
    mainMenu = menuBar()->addMenu("Main");
    mainMenu->addAction(settingAction);
    mainMenu->addAction(downloadAction);
}

void MainWindow::downloadFiles()
{
    DownloadWidget *dlw = new DownloadWidget();
    dlw->show();
}

