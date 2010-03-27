#include "tinyvoctrainer.h"

#include <ctime>

#include <QApplication>
#include <QtDebug>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QList>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>

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

    QPushButton *button = new QPushButton("Settings");
    hbox_buttons->addWidget(button);
    connect(button, SIGNAL(clicked(bool)), this, SLOT(slotSettings(bool)));


    QComboBox *combox_lesson = new QComboBox();
    QComboBox *combox_question = new QComboBox();
    QComboBox *combox_answer = new QComboBox();
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
        combox_question->insertItem(i, docRead->identifier(i).name(), NULL);
        combox_answer->insertItem(i, docRead->identifier(i).name(), NULL);
    }

    connect(combox_question,SIGNAL(currentIndexChanged(int)),this,SLOT(reactToToggleQuestion(int)));
    connect(combox_answer,SIGNAL(currentIndexChanged(int)),this,SLOT(reactToToggleAnswer(int)));
    combox_question->setCurrentIndex(0);
    combox_answer->setCurrentIndex(1);
    hbox_question_lang->addWidget(combox_question);
    hbox_answer_lang->addWidget(combox_answer);

    int lessonId = 0;
    foreach(KEduVocContainer * c, lessons) {
        if (c->containerType() == KEduVocLesson::Lesson) {
                lessonsList.append( static_cast<KEduVocLesson *>(c) );
                KEduVocLesson *m_lesson;
                m_lesson = lessonsList.last() ;
                qDebug () << "Lesson: " << m_lesson->name();

                combox_lesson->insertItem(lessonId, m_lesson->name(), NULL);
        }
        ++lessonId;
    }

    connect(combox_lesson,SIGNAL(currentIndexChanged(int)),this,SLOT(reactToToggleLesson(int)));
    combox_lesson->setCurrentIndex(0);
    hbox_less->addWidget(combox_lesson);

    // vbox->addLayout(hbox_less);
    // vbox->addLayout(hbox_question_lang);
    // vbox->addLayout(hbox_answer_lang);
    vbox->addLayout(hbox_buttons);
    vbox->addLayout(vbox_label);
    setLayout(vbox);

    dialog = new TinyVocTrainerSettings();

    connect(dialog,SIGNAL(SignalToggleAnswer(int)),this,SLOT(reactToToggleAnswer(int)));
    connect(dialog,SIGNAL(SignalToggleLesson(int)),this,SLOT(reactToToggleLesson(int)));
    connect(dialog,SIGNAL(SignalToggleQuestion(int)),this,SLOT(reactToToggleQuestion(int)));

    slotSettings();
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



void TinyVocTrainer::reactToToggleQuestion(int id)
{ 
    qDebug() << "Toggle Question: " << id;
    questionID = id;
}


void TinyVocTrainer::reactToToggleAnswer(int id)
{
    qDebug() << "Toggle Answer: " << id;
    answerID = id;
}


void TinyVocTrainer::reactToToggleLesson(int id)
{
    qDebug() << "Toggle Lesson: " << id;
    lessonID = id;
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

void TinyVocTrainer::slotSettings(bool clicked){
    Q_UNUSED(clicked);

    dialog->exec();
    slotInit();
}
