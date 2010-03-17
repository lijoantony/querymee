# -------------------------------------------------
# Project created by QtCreator 2010-03-17T19:16:34
# -------------------------------------------------
TARGET = TinyVocTrainer
TEMPLATE = app
LIBS += -L/home/rzingg/kedu/git/lib \
    -lkeduvocdocument
DEPENDPATH += . \
    /home/rzingg/kedu/git/lib
INCLUDEPATH += . \
    /home/rzingg/kedu/git/lib
QMAKE_LFLAGS += -Wl,-rpath,/home/rzingg/kedu/git/lib
SOURCES += main.cpp \
    tinyvoctrainer.cpp
HEADERS += tinyvoctrainer.h
FORMS += 
