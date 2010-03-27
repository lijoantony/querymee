#include "tinyvoctrainer.h"

#include <ctime>

#include <QApplication>
#include <QtDebug>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QList>
#include <QRadioButton>
#include <QPushButton>

TinyVocTrainer::TinyVocTrainer(QWidget *parent)
    : QWidget(parent)
{
    srand(time(NULL));

    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox_less = new QHBoxLayout();
    QHBoxLayout *hbox_question_lang = new QHBoxLayout();
    QHBoxLayout *hbox_answer_lang = new QHBoxLayout();
    QVBoxLayout *vbox_label = new QVBoxLayout();
    QHBoxLayout *hbox_buttons = new QHBoxLayout();

    QPushButton *button = new QPushButton("Apply");
    hbox_buttons->addWidget(button);
    connect(button, SIGNAL(clicked(bool)), this, SLOT(slotInit(bool)));

    bgroup_lesson = new QButtonGroup();
    bgroup_question_lang = new QButtonGroup();
    bgroup_answer_lang =  new QButtonGroup();
    bgroup_choice =  new QButtonGroup();

    QuestionLabel = new QLabel();
    QuestionLabel->setText("Here comes...");
    vbox_label->addWidget(QuestionLabel);

    answer1 = new QPushButton;
    answer2 = new QPushButton;
    answer3 = new QPushButton;
    vbox_label->addWidget(answer1);
    vbox_label->addWidget(answer2);
    vbox_label->addWidget(answer3);
    answerButtonsList.append(answer1);
    answerButtonsList.append(answer2);
    answerButtonsList.append(answer3);
    bgroup_choice->addButton(answer1);
    bgroup_choice->setId(answer1, 0);
    bgroup_choice->addButton(answer2);
    bgroup_choice->setId(answer2, 1);
    bgroup_choice->addButton(answer3);
    bgroup_choice->setId(answer3, 2);

    connect(answer1,SIGNAL(clicked(bool)),this,SLOT(slotAnswer1(bool)));
    connect(answer2,SIGNAL(clicked(bool)),this,SLOT(slotAnswer2(bool)));
    connect(answer3,SIGNAL(clicked(bool)),this,SLOT(slotAnswer3(bool)));

    questionID = 0;
    answerID = 0;
    lessonID = 0;
    CorrectID = 0;

    QLabel *label_question = new QLabel("Question:");
    hbox_question_lang->addWidget(label_question);

    QLabel *label_answer = new QLabel("Answer:");
    hbox_answer_lang->addWidget(label_answer);

    QLabel *label_lesson = new QLabel("Lesson:");
    hbox_less->addWidget(label_lesson);

    QStringList args = QApplication::arguments();
    args.removeFirst();

    KEduVocDocument *docRead = new KEduVocDocument();
    docRead->open(args.at(0));

    lessons = docRead->lesson()->childContainers();

    for (int i = 0; i < docRead->identifierCount(); ++i)
    {
        QRadioButton *radio_question_lang = new QRadioButton(docRead->identifier(i).name());
        QRadioButton *radio_answer_lang = new QRadioButton(docRead->identifier(i).name());

        bgroup_question_lang->addButton(radio_question_lang);
        bgroup_answer_lang->addButton(radio_answer_lang);

        bgroup_question_lang->setId(radio_question_lang, i);
        bgroup_answer_lang->setId(radio_answer_lang, i);

        hbox_question_lang->addWidget(radio_question_lang);
        hbox_answer_lang->addWidget(radio_answer_lang);

        connect(radio_question_lang, SIGNAL(toggled(bool)), this, SLOT(reactToToggleQuestionLang(bool)));
        connect(radio_answer_lang, SIGNAL(toggled(bool)), this, SLOT(reactToToggleAnswerLang(bool)));
    }

    bgroup_question_lang->button(0)->click();;
    bgroup_answer_lang->button(1)->click();

    int lessonId = 0;
    foreach(KEduVocContainer * c, lessons) {
        if (c->containerType() == KEduVocLesson::Lesson) {
                lessonsList.append( static_cast<KEduVocLesson *>(c) );
                KEduVocLesson *m_lesson;
                m_lesson = lessonsList.last() ;
                qDebug () << "Lesson: " << m_lesson->name();
                QRadioButton *radio_less = new QRadioButton(m_lesson->name());

                bgroup_lesson->addButton(radio_less);
                bgroup_lesson->setId(radio_less, lessonId);

                hbox_less->addWidget(radio_less);

                connect(radio_less, SIGNAL(toggled(bool)), this, SLOT(reactToToggleLesson(bool)));

// qDebug() << "Return rand Entry: " << getAnyEntryFromLesson(m_lesson,1);
        }
        ++lessonId;
    }

    bgroup_lesson->button(0)->click();

    vbox->addLayout(hbox_less);
    vbox->addLayout(hbox_question_lang);
    vbox->addLayout(hbox_answer_lang);
    vbox->addLayout(hbox_buttons);
    vbox->addLayout(vbox_label);
    setLayout(vbox);

    slotInit();
}

TinyVocTrainer::~TinyVocTrainer()
{

}

KEduVocExpression * TinyVocTrainer::getAnyEntryFromLesson(KEduVocLesson *lesson, int language)
{
    Q_UNUSED(language);
    int random_int = ( rand() %  ( lesson->entries().size() - 1 ) ) + 0;
    qDebug() << "anyEntry random_int: " << random_int << "Lesson Size: " << lesson->entries().size();
    return lesson->entry(random_int);
}



void TinyVocTrainer::reactToToggleQuestionLang(bool checked)
{
    if(checked){   
        qDebug() << "bgroup_question_lang: " << bgroup_question_lang->checkedId();
        questionID = bgroup_question_lang->checkedId();
        // slotInit();
    }
}


void TinyVocTrainer::reactToToggleAnswerLang(bool checked)
{
    if(checked){
        qDebug() << "bgroup_answer_lang: " << bgroup_answer_lang->checkedId();
        answerID = bgroup_answer_lang->checkedId();
        // slotInit();
    }
}


void TinyVocTrainer::reactToToggleLesson(bool checked)
{
    if(checked){
        qDebug() << "bgroup_lesson: " << bgroup_lesson->checkedId();
        lessonID = bgroup_lesson->checkedId();
        // slotInit();
    }
}

void TinyVocTrainer::slotAnswer1(bool clicked){
    Q_UNUSED(clicked);
    slotCheck(0);
}

void TinyVocTrainer::slotAnswer2(bool clicked){
    Q_UNUSED(clicked);
    slotCheck(1);
}

void TinyVocTrainer::slotAnswer3(bool clicked){
    Q_UNUSED(clicked);
    slotCheck(2);
}

void TinyVocTrainer::slotCheck(int buttonID){
    qDebug() << "slotCheck(): buton id: " << buttonID << "Correct Id: " << CorrectID;

    if(buttonID == CorrectID){
        qDebug() << "\\o/ correct answer...";
        slotInit(1);
        return;
    }
    else{
        qDebug() << ":-( sorry wrong...";
    }

}

void TinyVocTrainer::slotInit(bool clicked){

    Q_UNUSED(clicked);

    choiceList.clear();
    for (int i=0; i < 3; ++i){
        choiceList.append( getAnyEntryFromLesson(lessonsList.at(lessonID), questionID) );
        answerButtonsList.at(i)->setText( choiceList.at(i)->translation(questionID)->text() );
    }

    int random_int = rand() %  3  + 0;
    qDebug() << "ask for random_int: " <<  random_int;

    QuestionLabel->setText(choiceList.at(random_int)->translation(answerID)->text());
    CorrectID = random_int;

}
