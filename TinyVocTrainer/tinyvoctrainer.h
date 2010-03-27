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

#include <QtGui/QWidget>
#include <QButtonGroup>
#include <QList>
#include <QLabel>
#include <QPushButton>

#include "keduvocdocument.h"
#include "keduvoclesson.h"
#include "keduvocexpression.h"
#include "keduvocleitnerbox.h"

#include "tinyvoctrainersettings.h"

class TinyVocTrainer : public QWidget
{
    Q_OBJECT

public:
    TinyVocTrainer(QWidget *parent = 0, const QString &fileName = "");
    ~TinyVocTrainer();
    KEduVocExpression * getAnyEntryFromLesson(KEduVocLesson *lesson = 0, int language = 0);

//    QButtonGroup *bgroup_lesson;
//    QButtonGroup *bgroup_question_lang;
//    QButtonGroup *bgroup_answer_lang;
    QButtonGroup *bgroup_choice;
    QList<KEduVocContainer *>  lessons;
    QList<KEduVocLesson *> lessonsList;
    KEduVocDocument *docRead;
    QLabel *QuestionLabel;
    QPushButton *answer1;
    QPushButton *answer2;
    QPushButton *answer3;
    QPushButton *answer4;
    QList<KEduVocExpression *> choiceList;
    QList<QPushButton *> answerButtonsList;


    int questionID;
    int answerID;
    int lessonID;
    int CorrectID;

    TinyVocTrainerSettings *dialog;

private slots:
    void reactToToggleQuestion(int id = 0);
    void reactToToggleAnswer(int id = 0);
    void reactToToggleLesson(int id = 0);
    void slotCheck(int buttonID = 0);
    void slotAnswer1(bool clicked = 0);
    void slotAnswer2(bool clicked = 0);
    void slotAnswer3(bool clicked = 0);
    void slotAnswer4(bool clicked = 0);
    void slotInit(bool clicked = 0);
    void slotSettings(bool clicked = 0);

};

#endif // TINYVOCTRAINER_H
