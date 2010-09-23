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
    querymee_random.cpp \
    querymee_leitner.cpp \
    querymeesettings.cpp \
    mainwindow.cpp \
    trainingselectionview.cpp \
    downloadmanager.cpp \
    downloadwidget.cpp \
    knsxmlstreamreader.cpp \
    ocsxmlstreamreader.cpp
HEADERS += querymee_random.h \
    querymee_leitner.h \
    querymeesettings.h \
    mainwindow.h \
    trainingselectionview.h \
    downloadmanager.h \
    downloadwidget.h \
    knsxmlstreamreader.h \
    querymee_defaults.h \
    ocsxmlstreamreader.h \
    version.h
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
