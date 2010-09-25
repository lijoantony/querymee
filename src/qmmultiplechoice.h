/***************************************************************************

    Copyright 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QMMULTIPLECHOICE_H
#define QMMULTIPLECHOICE_H

#include <QWidget>

#include "qmtrainer.h"

class QmVocExpression;
class QPushButton;
class QLabel;

class QmMultipleChoice : public QmTrainer
{
    Q_OBJECT
public:
    explicit QmMultipleChoice(QWidget *parent = 0);
    ~QmMultipleChoice();
    void startTraining();

signals:

public slots:
    void slotInit();
    void slotClicked(int id);

private:
    void closeEvent ( QCloseEvent * event );
    QList<QmVocExpression *> m_ChoiceList;
    QList<QPushButton *> m_AnswerButtonsList;
    QLabel* m_QuestionLabel;
    QLabel *statusLabel;
    QLabel *answerLabel;
    int m_CorrectId;
};

#endif // QMMULTIPLECHOICE_H
