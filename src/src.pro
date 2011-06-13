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
    querymeesettings.cpp \
    mainwindow.cpp \
    trainingselectionview.cpp \
    downloadmanager.cpp \
    downloadwidget.cpp \
    knsxmlstreamreader.cpp \
    ocsxmlstreamreader.cpp \
    qmtrainer.cpp \
    qmmultiplechoice.cpp \
    qmflashcard.cpp \
    lessonchoosewidget.cpp
HEADERS += querymeesettings.h \
    mainwindow.h \
    trainingselectionview.h \
    downloadmanager.h \
    downloadwidget.h \
    knsxmlstreamreader.h \
    querymee_defaults.h \
    ocsxmlstreamreader.h \
    version.h \
    qmtrainer.h \
    qmmultiplechoice.h \
    qmflashcard.h \
    lessonchoosewidget.h
FORMS += 
QT += network \
    xml

unix {
    #VARIABLES
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

    BINDIR = $$PREFIX/bin

    #MAKE INSTALL

    INSTALLS += target

    target.path =$$BINDIR

}
