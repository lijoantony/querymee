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

class TinyVocTrainer : public QWidget
{
    Q_OBJECT

public:
    TinyVocTrainer(QWidget *parent = 0);
    ~TinyVocTrainer();
    KEduVocExpression * getAnyEntryFromLesson(KEduVocLesson *lesson = 0, int language = 0);

    QButtonGroup *bgroup_lesson;
    QButtonGroup *bgroup_question_lang;
    QButtonGroup *bgroup_answer_lang;
    QButtonGroup *bgroup_choice;
    QList<KEduVocContainer *>  lessons;
    QList<KEduVocLesson *> lessonsList;
    KEduVocDocument *docRead;
    QLabel *QuestionLabel;
    QPushButton *answer1;
    QPushButton *answer2;
    QPushButton *answer3;
    QList<KEduVocExpression *> choiceList;
    QList<QPushButton *> answerButtonsList;


    int questionID;
    int answerID;
    int lessonID;
    int CorrectID;

private slots:
    void reactToToggleQuestionLang(bool checked = 0);
    void reactToToggleAnswerLang(bool checked = 0);
    void reactToToggleLesson(bool checked = 0);
    void slotCheck(int buttonID = 0);
    void slotAnswer1(bool clicked = 0);
    void slotAnswer2(bool clicked = 0);
    void slotAnswer3(bool clicked = 0);
    void slotInit(bool clicked = 0);

};

#endif // TINYVOCTRAINER_H
