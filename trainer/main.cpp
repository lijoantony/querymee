#include <QtGui/QApplication>
#include "trainer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Trainer w;
    w.show();
    return a.exec();
}
