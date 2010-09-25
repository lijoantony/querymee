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

#ifndef QMTRAINER_H
#define QMTRAINER_H

#include <QWidget>
#include <qmvocdocument.h>

class QmVocExpression;
class QmVocLesson;
class QPushButton;
class QLabel;

class QmTrainer : public QWidget
{
    Q_OBJECT
public:
    explicit QmTrainer(QWidget *parent = 0);
    ~QmTrainer();

    void setLession(int lessionIndex);
    void setQuestionLanguage(int languageIndex);
    void setAnswerLanguage(int languageIndex);
    void setQmVocDocument(QmVocDocument *doc);
    void setRandomOnly(bool randomOnly);
    virtual void startTraining() = 0;
    void setCurrentFileName(QString fileName);


signals:

public slots:

protected:
    int m_LessionIndex;
    int m_QuestionLanguage;
    int m_AnswerLanguage;
    bool m_randomOnly;
    bool m_lastAnswerRight;
    QList<QmVocExpression *> m_ChoiceList;
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
    void handleAnswer(bool answerCountsAsRight);
    void setLastAnswerRight(bool lastAnswerRight);

    QmVocExpression * getNextEntry();
    QmVocExpression * getAnyEntryFromLesson();
    int randomInt(int min, int max);

private:



};

#endif // QMTRAINER_H
