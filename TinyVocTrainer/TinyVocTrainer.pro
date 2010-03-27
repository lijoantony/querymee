# -------------------------------------------------
# Project created by QtCreator 2010-03-17T19:16:34
# -------------------------------------------------
TARGET = TinyVocTrainer
TEMPLATE = app
LIBS += -L../lib \
    -lkeduvocdocument
DEPENDPATH += . \
    ../lib
INCLUDEPATH += . \
    ../lib
QMAKE_LFLAGS += -Wl,-rpath,../lib
SOURCES += main.cpp \
    tinyvoctrainer.cpp
#    tinyvoctrainersettings.cpp
HEADERS += tinyvoctrainer.h
#    tinyvoctrainersettings.h
FORMS += 
