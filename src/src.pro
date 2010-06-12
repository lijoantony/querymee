# -------------------------------------------------
# Project created by QtCreator 2010-03-17T19:16:34
# -------------------------------------------------
TARGET = querymee
TEMPLATE = app
LIBS += -L../lib \
    -lqmvocdocument
DEPENDPATH += . \
    ../lib
INCLUDEPATH += . \
    ../lib
QMAKE_LFLAGS += -Wl,-rpath,../lib
SOURCES += main.cpp \
    querymee.cpp \
    querymeesettings.cpp \
    mainwindow.cpp \
    trainingselectionview.cpp \
    downloadmanager.cpp \
    downloadwidget.cpp \
    xmlstreamreader.cpp
HEADERS += querymee.h \
    querymeesettings.h \
    mainwindow.h \
    trainingselectionview.h \
    downloadmanager.h \
    downloadwidget.h \
    xmlstreamreader.h \
    querymee_defaults.h
FORMS += 
QT += network \
    xml
