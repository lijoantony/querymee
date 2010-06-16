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
#include "querymee_defaults.h"

MainWindow::MainWindow()
{
    TrainingSelectionView* trainingView = new TrainingSelectionView(this);

    connect(this,
            SIGNAL(updateFileList()),
            trainingView,
            SLOT(slotInitView()));

    createActions();
    createMenus();
    setCentralWidget(trainingView);

#ifdef Q_WS_MAEMO_5
    this->setAttribute(Qt::WA_Maemo5StackedWindow, true);
#endif

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

    helpAction = new QAction(tr("Help"), this);
    connect(helpAction,
            SIGNAL(triggered()),
            this,
            SLOT(help()));
}

void MainWindow::createMenus(){
    mainMenu = menuBar()->addMenu("Main");
    mainMenu->addAction(settingAction);
    mainMenu->addAction(downloadAction);
    mainMenu->addAction(helpAction);
}

void MainWindow::downloadFiles()
{
    DownloadWidget *dlw = new DownloadWidget(this);

    connect(dlw,
            SIGNAL(finished()),
            this,
            SIGNAL(updateFileList()));

#ifdef Q_WS_MAEMO_5
    dlw->setAttribute(Qt::WA_Maemo5StackedWindow, true);
#endif

    dlw->setWindowFlags(dlw->windowFlags() | Qt::Window);

    dlw->show();

}

void MainWindow::help(){
    QDesktopServices::openUrl(QUrl("../doc/help.html"));
}
