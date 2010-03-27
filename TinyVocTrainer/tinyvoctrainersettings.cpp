#include "tinyvoctrainersettings.h"

#include <QApplication>
#include <QtDebug>

TinyVocTrainerSettings::TinyVocTrainerSettings(QWidget *parent, const QString &fileName)
    : QDialog(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    QHBoxLayout *hbox_less = new QHBoxLayout();
    QHBoxLayout *hbox_question_lang = new QHBoxLayout();
    QHBoxLayout *hbox_answer_lang = new QHBoxLayout();

    QComboBox *combox_lesson = new QComboBox();
    QComboBox *combox_question = new QComboBox();
    QComboBox *combox_answer = new QComboBox();

    QLabel *label_question = new QLabel("Question:");
    hbox_question_lang->addWidget(label_question);

    QLabel *label_answer = new QLabel("Answer:");
    hbox_answer_lang->addWidget(label_answer);

    QLabel *label_lesson = new QLabel("Lesson:");
    hbox_less->addWidget(label_lesson);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, this);

    connect(buttons, SIGNAL(accepted()),this,SLOT(accept()));
    connect(buttons,SIGNAL(rejected()),this,SLOT(reject()));

    KEduVocDocument *docRead = new KEduVocDocument();
    docRead->open(fileName);

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

    vbox->addLayout(hbox_less);
    vbox->addLayout(hbox_question_lang);
    vbox->addLayout(hbox_answer_lang);
    vbox->addWidget(buttons);
    setLayout(vbox);

}

void TinyVocTrainerSettings::reactToToggleQuestion(int id)
{
    qDebug() << "TinyVocTrainerSettings Toggle Question: " << id;
    emit SignalToggleQuestion(id);
}


void TinyVocTrainerSettings::reactToToggleAnswer(int id)
{
    qDebug() << "TinyVocTrainerSettings Toggle Answer: " << id;
    emit SignalToggleAnswer(id);
}


void TinyVocTrainerSettings::reactToToggleLesson(int id)
{
    qDebug() << "TinyVocTrainerSettings Toggle Lesson: " << id;
    emit SignalToggleLesson(id);
}
