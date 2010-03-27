#ifndef TINYVOCTRAINERSETTINGS_H
#define TINYVOCTRAINERSETTINGS_H

#include <QObject>
#include <QDialog>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>

#include "keduvoclesson.h"
#include "keduvocexpression.h"
#include "keduvocdocument.h"


class TinyVocTrainerSettings : public QDialog
{

Q_OBJECT

public:
    explicit TinyVocTrainerSettings(QWidget *parent = 0);

    QList<KEduVocContainer *>  lessons;
    QList<KEduVocLesson *> lessonsList;

private slots:
//    void accept();
//    void reject();
    void reactToToggleQuestion(int id = 0);
    void reactToToggleAnswer(int id = 0);
    void reactToToggleLesson(int id = 0);

signals:
    void SignalToggleQuestion(int id);
    void SignalToggleAnswer(int id);
    void SignalToggleLesson(int id);
};

#endif // TINYVOCTRAINERSETTINGS_H
