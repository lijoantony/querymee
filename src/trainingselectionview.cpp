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

#include "trainingselectionview.h"
#include "querymeesettings.h"
#include "qmtrainer.h"
#include "qmmultiplechoice.h"
#include "qmflashcard.h"
#include "lessonchoosewidget.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QCheckBox>
#include <QList>
#include <QDebug>

TrainingSelectionView::TrainingSelectionView(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    QHBoxLayout *hbox_dictionary = new QHBoxLayout();
    QHBoxLayout *hbox_lession = new QHBoxLayout();
    QHBoxLayout *hbox_question_lang = new QHBoxLayout();
    QHBoxLayout *hbox_answer_lang = new QHBoxLayout();
    QHBoxLayout *hbox_trainer = new QHBoxLayout();
    QHBoxLayout *hbox_bottom = new QHBoxLayout();

    m_ComboDictionary = new QComboBox();

    QPushButton *m_buttonLesson = new QPushButton();
    m_buttonLesson->setText(tr("Choose Lessons"));

    connect(m_buttonLesson,
            SIGNAL(clicked()),
            this,
            SLOT(slotLessonDialog()));

    m_ComboQuestionLang = new QComboBox();
    m_ComboAnswerLang = new QComboBox();
    m_ComboTrainer = new QComboBox();

    m_ComboTrainer->addItem(tr("Flash Card"));
    m_ComboTrainer->setCurrentIndex(0);
    m_ComboTrainer->addItem(tr("Multiple Choice"));


    QLabel *label_dictionary = new QLabel(tr("Dictionary:"));
    hbox_dictionary->addWidget(label_dictionary);
    hbox_dictionary->addWidget(m_ComboDictionary);

    QLabel *label_lesson = new QLabel(tr("Lesson:"));
    hbox_lession->addWidget(label_lesson);
    hbox_lession->addWidget(m_buttonLesson);

    QLabel *label_question = new QLabel(tr("Question:"));
    hbox_question_lang->addWidget(label_question);
    hbox_question_lang->addWidget(m_ComboQuestionLang);

    QLabel *label_answer = new QLabel(tr("Answer:"));
    hbox_answer_lang->addWidget(label_answer);
    hbox_answer_lang->addWidget(m_ComboAnswerLang);

    QLabel *label_trainer = new QLabel(tr("Typ of training"));
    hbox_trainer->addWidget(label_trainer);
    hbox_trainer->addWidget(m_ComboTrainer);

    m_checkbox_random = new QCheckBox(tr("Random"), this);

    QPushButton *m_button = new QPushButton();
    m_button->setText(tr("Start"));

    connect(m_button,
            SIGNAL(clicked()),
            this,
            SLOT(start()));

    vbox->addLayout(hbox_dictionary);
    vbox->addLayout(hbox_lession);
    vbox->addLayout(hbox_question_lang);
    vbox->addLayout(hbox_answer_lang);
    vbox->addLayout(hbox_trainer);
#ifdef Q_WS_MAEMO_5
    m_checkbox_portrait = new QCheckBox(tr("Portrait"), this);
    hbox_bottom->addWidget(m_checkbox_portrait);
#endif
    hbox_bottom->addWidget(m_checkbox_random);
    hbox_bottom->addWidget(m_button);

    vbox->addLayout(hbox_bottom);

    setLayout(vbox);

    connect(QueryMeeSettings::instance(),
            SIGNAL(dictionaryChanged()),
            SLOT(slotDictionaryChanged()));

    connect(m_ComboDictionary,
            SIGNAL(activated(int)),
            SLOT(slotDictionarySelected(int)));

    m_TrainingLessons = new QList<int>;
    m_TrainingLessons->append(0);

    QTimer::singleShot(1000,this,SLOT(slotInitView()));
}

void TrainingSelectionView::slotInitView()
{
    QueryMeeSettings* settings = QueryMeeSettings::instance();
    if(settings->dictionaries().count()) {
        QString dictionaryName = settings->dictionaries().first();
        settings->openDictionary(dictionaryName);
    }
    slotDictionaryChanged();
}

void TrainingSelectionView::start()
{
    QueryMeeSettings* settings = QueryMeeSettings::instance();

    // check what is selected
    if(m_ComboDictionary && m_ComboDictionary->count()) {
        slotDictionarySelected(m_ComboDictionary->currentIndex());
        // QueryMee* trainer = new QueryMee(this);

        QmTrainer *trainer;

        switch (m_ComboTrainer->currentIndex()){

            case 0:
                trainer = new QmFlashCard(this);
                break;

            case 1:
                trainer = new QmMultipleChoice(this);
                break;

            default :
                break;
        }

        trainer->setRandomOnly(m_checkbox_random->isChecked());
        trainer->setQuestionLanguage(m_ComboQuestionLang->currentIndex());
        trainer->setAnswerLanguage(m_ComboAnswerLang->currentIndex());
        trainer->setLesson(m_TrainingLessons);
        trainer->setQmVocDocument(settings->getQmVocDocument());
        trainer->setCurrentFileName(settings->getCurrentlyOpenedFile());

#ifdef Q_WS_MAEMO_5
        /* due to bug:
                    * https://bugs.maemo.org/show_bug.cgi?id=10521
                    * http://bugreports.qt.nokia.com/browse/QTBUG-11190
                    * it's not possible to have Portrait and Stackedwindows
                    * so we have either one
                    */
        if(m_checkbox_portrait->isChecked() == true){
            trainer->setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
        } else {
            trainer->setAttribute(Qt::WA_Maemo5StackedWindow, true);
        }
#endif

        trainer->setWindowFlags(trainer->windowFlags() | Qt::Window);

        trainer->startTraining();

    } else {
        QueryMeeSettings::instance()->openDictionary();
    }
}

void TrainingSelectionView::slotDictionaryChanged()
{
    QueryMeeSettings* settings = QueryMeeSettings::instance();

    m_ComboDictionary->clear();
    m_ComboQuestionLang->clear();
    m_ComboAnswerLang->clear();
    m_TrainingLessons->clear();

    m_ComboDictionary->addItems(settings->dictionaries());
    m_ComboQuestionLang->addItems(settings->languages());
    m_ComboAnswerLang->addItems(settings->languages());

    m_ComboDictionary->setCurrentIndex(settings->openedDictionary());
    m_ComboQuestionLang->setCurrentIndex(0);
    m_ComboAnswerLang->setCurrentIndex(1);
    m_TrainingLessons->append(0);

#ifdef Q_WS_MAEMO_5
    this->parentWidget()->setAttribute(Qt::WA_Maemo5ShowProgressIndicator, false);
#endif
}

void TrainingSelectionView::slotDictionarySelected(int index)
{
    if(index >= 0) {
        QString dictionaryName = m_ComboDictionary->itemText(index);
        QueryMeeSettings::instance()->openDictionary(dictionaryName);
    }
}

void TrainingSelectionView::slotLessonDialog(){
    QueryMeeSettings *settings = QueryMeeSettings::instance();
    LessonChooseWidget *lessonDialog = new LessonChooseWidget(this, settings);
    lessonDialog->setWindowFlags(lessonDialog->windowFlags() | Qt::Window);
#ifdef Q_WS_MAEMO_5
    lessonDialog->setAttribute(Qt::WA_Maemo5StackedWindow, true);
#endif
    lessonDialog->show();
}

void TrainingSelectionView::setTrainingsLessons(QList<int> *trainingLessons){
    m_TrainingLessons->clear();
    m_TrainingLessons = trainingLessons;
}
