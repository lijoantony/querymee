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
#include <qmvocdocument.h>

class QmVocExpression;
class QmVocLesson;
class QPushButton;
class QLabel;

class QueryMee : public QWidget
{
    Q_OBJECT

public:
    QueryMee(QWidget *parent = 0);
    ~QueryMee();

public:
    void setLession(int lessionIndex);
    void setQuestionLanguage(int languageIndex);
    void setAnswerLanguage(int languageIndex);
    void setQmVocDocument(QmVocDocument *doc);
    void startTraining();
    void setCurrentFileName(QString fileName);

private:
    QmVocExpression * getAnyEntryFromLesson();
    QmVocExpression * getNextEntry();

    void closeEvent ( QCloseEvent * event );
    int randomInt(int min, int max);

private Q_SLOTS:
    void slotInit();
    void slotClicked(int id);

private:
    int m_CorrectId;
    int m_LessionIndex;
    int m_QuestionLanguage;
    int m_AnswerLanguage;
    bool m_firstAnswerWrong;
    QList<QmVocExpression *> m_ChoiceList;
    QList<QPushButton *> m_AnswerButtonsList;
    QLabel* m_QuestionLabel;
    QLabel *statusLabel;
    QLabel *answerLabel;
    QmVocLesson *m_lesson;
    QList<QmVocExpression *> m_entries;
    QList<QmVocExpression *> inPractice;
    QList<QmVocExpression *> leitnerBox1;
    QList<QmVocExpression *> leitnerBox2;
    QList<QmVocExpression *> leitnerBox3;
    QList<QmVocExpression *> leitnerBox4;
    QList<QmVocExpression *> leitnerBox5;
    QList<QmVocExpression *> leitnerBox6;
    QList<QmVocExpression *> leitnerBox7;
    QmVocExpression *m_CorrectExp;
    QmVocExpression *m_LastExp;
    QmVocDocument *m_QmVocDocument;
    QString m_CurrentFileName;
};

#endif // TINYVOCTRAINER_H
