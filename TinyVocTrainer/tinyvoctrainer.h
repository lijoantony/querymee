#ifndef TINYVOCTRAINER_H
#define TINYVOCTRAINER_H

#include <QtGui/QWidget>
#include <QButtonGroup>
#include <QList>
#include <QLabel>

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
    QString getAnyEntryFromLesson(KEduVocLesson *lesson = 0, int language = 0);

    QButtonGroup *bgroup_lesson;
    QButtonGroup *bgroup_question_lang;
    QButtonGroup *bgroup_answer_lang;
    QList<KEduVocContainer *>  lessons;
    QList<KEduVocLesson *> lessonsList;
    KEduVocDocument *docRead;
    QLabel *QuestionLabel;

    int questionID;
    int answerID;
    int lessonID;

private slots:
    void reactToToggleQuestionLang(bool checked = 0);
    void reactToToggleAnswerLang(bool checked = 0);
    void reactToToggleLesson(bool checked = 0);
    void slotNext(bool clicked = 0);

};

#endif // TINYVOCTRAINER_H
