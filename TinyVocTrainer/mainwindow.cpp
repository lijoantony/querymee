#include <QtGui>
#include <QFlags>

#include "mainwindow.h"
#include "tinyvoctrainer.h"
#include "tinyvoctrainersettings.h"

MainWindow::MainWindow()
{


    curFile = "";
    // setCurrentFile("");

    QStringList args = QApplication::arguments();
    args.removeFirst();

    if (args.count() > 0){
        if ( ! QString(args.at(0)).isEmpty() ){
            qDebug() << "curFile is gonna be: " << args.at(0);
            curFile = args.at(0);
        }
        else{
            qDebug() << "something went wrong, lets the user choose for a file... args.at0: " << args.at(0);
            open();
        }
    }
    else{
        qDebug() << "not enough arguments";
        open();
    }

    trainer = new TinyVocTrainer(this, curFile);

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

void MainWindow::open(){
    curFile = QFileDialog::getOpenFileName(this, QString("Open kvmtl file"),
                                                 QString(""),
                                                 QString("kvtml file (*.kvtml)") );
    if ( curFile.isEmpty() ){
        open();
    }
}
