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
    QHBoxLayout *hbox_buttons = new QHBoxLayout();
    QHBoxLayout *hbox_label = new QHBoxLayout();

    QPushButton *button = new QPushButton("Next");

    hbox_buttons->addWidget(button);

    connect(button, SIGNAL(clicked(bool)), this, SLOT(slotNext(bool)));

    bgroup_lesson = new QButtonGroup();
    bgroup_question_lang = new QButtonGroup();
    bgroup_answer_lang =  new QButtonGroup();

    QuestionLabel = new QLabel();
    QuestionLabel->setText("Here comes...");
    hbox_label->addWidget(QuestionLabel);


    questionID = 0;
    answerID = 0;
    lessonID = 0;

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

    vbox->addLayout(hbox_less);
    vbox->addLayout(hbox_question_lang);
    vbox->addLayout(hbox_answer_lang);
    vbox->addLayout(hbox_label);
    vbox->addLayout(hbox_buttons);
    setLayout(vbox);

}

TinyVocTrainer::~TinyVocTrainer()
{

}

QString TinyVocTrainer::getAnyEntryFromLesson(KEduVocLesson *lesson, int language)
{
    int random_int = ( rand() %  ( lesson->entries().size() - 1 ) ) + 0;
    qDebug() << "random_int: " << random_int << "Lesson Size: " << lesson->entries().size();
    return lesson->entry(random_int)->translation(language)->text();
}



void TinyVocTrainer::reactToToggleQuestionLang(bool checked)
{
    if(checked){   
        qDebug() << "bgroup_question_lang: " << bgroup_question_lang->checkedId();
        questionID = bgroup_question_lang->checkedId();
    }
}


void TinyVocTrainer::reactToToggleAnswerLang(bool checked)
{
    if(checked){
        qDebug() << "bgroup_answer_lang: " << bgroup_answer_lang->checkedId();
        answerID = bgroup_answer_lang->checkedId();
    }
}


void TinyVocTrainer::reactToToggleLesson(bool checked)
{
    if(checked){
        qDebug() << "bgroup_lesson: " << bgroup_lesson->checkedId();
        lessonID = bgroup_lesson->checkedId();
    }
}

void TinyVocTrainer::slotNext(bool clicked){
    qDebug() << "lessonsList count: " << lessonsList.count();
    qDebug() << "lessonsList Name: " << lessonsList.at(lessonID)->name();
    QuestionLabel->setText( getAnyEntryFromLesson(lessonsList.at(lessonID), questionID) );
}
