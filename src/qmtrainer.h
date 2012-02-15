/***************************************************************************

    Copyright 2010, 2011 Reto Zingg <g.d0b3rm4n@gmail.com>

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
#include <QList>
#include "leitnerwidget.h"

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

    void setLesson(QList<int>*);
    void setQuestionLanguage(int languageIndex);
    void setAnswerLanguage(int languageIndex);
    void setQmVocDocument(QmVocDocument *doc);
    void setRandomOnly(bool randomOnly);
    void setRevision(bool revision);
    virtual void startTraining() = 0;
    void setCurrentFileName(QString fileName);


signals:

public slots:

protected:
    QList<int> *m_lessonIndexes;
    int m_QuestionLanguage;
    int m_AnswerLanguage;
    bool m_randomOnly;
    bool m_revision;
    bool m_lastAnswerRight;
    QPushButton *button_correct;
    QPushButton *button_wrong;
    QPushButton *button_showBackSide;
    QList<QmVocExpression *> m_ChoiceList;
    QList<QmVocExpression *> m_entries;
    QList<QmVocExpression *> inPractice;
    QList<QList<QmVocExpression *>*> leitnerBoxes;
    QmVocExpression *m_CorrectExp;
    QmVocExpression *m_LastExp;
    QmVocDocument *m_QmVocDocument;
    QString m_CurrentFileName;
#ifndef Q_WS_MAEMO_5
    LeitnerWidget *m_leitnerWidget;
#endif

    void handleAnswer(bool answerCountsAsRight);
    void setLastAnswerRight(bool lastAnswerRight);

    QmVocExpression * getNextEntry();
    QmVocExpression * getAnyEntryFromLesson();
    QmVocExpression * getNextEntryFromLesson();
    int randomInt(int min, int max);

private:



};

#endif // QMTRAINER_H
