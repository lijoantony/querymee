#include <QtGui>

#include "mainwindow.h"
#include "tinyvoctrainer.h"
#include "tinyvoctrainersettings.h"

MainWindow::MainWindow()
{
    trainer = new TinyVocTrainer;

    createActions();
    createMenus();

    setCentralWidget(trainer);
}

void MainWindow::createActions(){
    settingAction = new QAction("Settings", this);
    connect(settingAction,SIGNAL(triggered()), trainer, SLOT(slotSettings()));
}

void MainWindow::createMenus(){
    mainMenu = menuBar()->addMenu("Main");
    mainMenu->addAction(settingAction);
}
