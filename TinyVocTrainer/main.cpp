#include <QtGui/QApplication>
#include "tinyvoctrainer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TinyVocTrainer w;
    w.show();
    return a.exec();
}
