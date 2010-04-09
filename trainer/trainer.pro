#-------------------------------------------------
#
# Project created by QtCreator 2010-03-16T20:41:35
#
#-------------------------------------------------

TARGET = trainer
TEMPLATE = app
LIBS += -L../lib -lqtvtvocdocument
DEPENDPATH += . ../lib
INCLUDEPATH += . ../lib

QMAKE_LFLAGS += -Wl,-rpath,../lib

SOURCES += main.cpp\
        trainer.cpp

HEADERS  += trainer.h
