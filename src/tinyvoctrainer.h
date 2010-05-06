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

#ifndef TINYVOCTRAINER_H
#define TINYVOCTRAINER_H

#include <QWidget>

class QTvtVocExpression;
class QPushButton;
class QLabel;

class TinyVocTrainer : public QWidget
{
    Q_OBJECT

public:
    TinyVocTrainer(QWidget *parent = 0);
    ~TinyVocTrainer();

public:
    void setLession(int lessionIndex);
    void setQuestionLanguage(int languageIndex);
    void setAnswerLanguage(int languageIndex);
    void startTraining();

private:
    QTvtVocExpression * getAnyEntryFromLesson();

    void closeEvent ( QCloseEvent * event );

private Q_SLOTS:
    void slotInit();
    void slotClicked(int id);

private:
    int m_CorrectId;
    int m_LessionIndex;
    int m_QuestionLanguage;
    int m_AnswerLanguage;
    QList<QTvtVocExpression *> m_ChoiceList;
    QList<QPushButton *> m_AnswerButtonsList;
    QLabel* m_QuestionLabel;
    QLabel *statusLabel;
    QLabel *answerLabel;
};

#endif // TINYVOCTRAINER_H
