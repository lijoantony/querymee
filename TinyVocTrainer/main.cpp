#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    TinyVocTrainer w;
//    w.show();

    MainWindow *mainWin = new MainWindow;
    mainWin->show();
    return a.exec();
}
