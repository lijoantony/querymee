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

#include "trainingselectionview.h"
#include "querymee.h"
#include "querymeesettings.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QTimer>

TrainingSelectionView::TrainingSelectionView(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout();

    QHBoxLayout *hbox_dictionary = new QHBoxLayout();
    QHBoxLayout *hbox_lession = new QHBoxLayout();
    QHBoxLayout *hbox_question_lang = new QHBoxLayout();
    QHBoxLayout *hbox_answer_lang = new QHBoxLayout();

    m_ComboDictionary = new QComboBox();
    m_ComboLesson = new QComboBox();
    m_ComboQuestionLang = new QComboBox();
    m_ComboAnswerLang = new QComboBox();

    QLabel *label_dictionary = new QLabel(tr("Dictionary:"));
    hbox_dictionary->addWidget(label_dictionary);
    hbox_dictionary->addWidget(m_ComboDictionary);

    QLabel *label_lesson = new QLabel(tr("Lesson:"));
    hbox_lession->addWidget(label_lesson);
    hbox_lession->addWidget(m_ComboLesson);

    QLabel *label_question = new QLabel(tr("Question:"));
    hbox_question_lang->addWidget(label_question);
    hbox_question_lang->addWidget(m_ComboQuestionLang);

    QLabel *label_answer = new QLabel(tr("Answer:"));
    hbox_answer_lang->addWidget(label_answer);
    hbox_answer_lang->addWidget(m_ComboAnswerLang);

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    buttons->setOrientation(Qt::Horizontal);
    buttons->addButton(tr("Start"),QDialogButtonBox::AcceptRole);

    connect(buttons, SIGNAL(accepted()),this,SLOT(start()));

    vbox->addLayout(hbox_dictionary);
    vbox->addLayout(hbox_lession);
    vbox->addLayout(hbox_question_lang);
    vbox->addLayout(hbox_answer_lang);
    vbox->addWidget(buttons);

    setLayout(vbox);

    connect(QueryMeeSettings::instance(),
            SIGNAL(dictionaryChanged()),
            SLOT(slotDictionaryChanged()));

    connect(m_ComboDictionary,
            SIGNAL(activated(int)),
            SLOT(slotDictionarySelected(int)));

    QTimer::singleShot(1000,this,SLOT(slotInitView()));
}

void TrainingSelectionView::slotInitView()
{
    QueryMeeSettings* settings = QueryMeeSettings::instance();
    if(settings->dictionaries().count()) {
        QString dictionaryName = settings->dictionaries().first();
        settings->openDictionary(dictionaryName);
    }
}

void TrainingSelectionView::start()
{
    // check what is selected
    if(m_ComboDictionary && m_ComboDictionary->count()) {
        slotDictionarySelected(m_ComboDictionary->currentIndex());
        QueryMee* trainer = new QueryMee();
        trainer->setLession(m_ComboLesson->currentIndex());
        trainer->setQuestionLanguage(m_ComboQuestionLang->currentIndex());
        trainer->setAnswerLanguage(m_ComboAnswerLang->currentIndex());
        trainer->startTraining();
    } else {
        QueryMeeSettings::instance()->openDictionary();
    }
}

void TrainingSelectionView::slotDictionaryChanged()
{
    QueryMeeSettings* settings = QueryMeeSettings::instance();

    m_ComboDictionary->clear();
    m_ComboLesson->clear();
    m_ComboQuestionLang->clear();
    m_ComboAnswerLang->clear();

    m_ComboDictionary->addItems(settings->dictionaries());
    m_ComboLesson->addItems(settings->lessons());
    m_ComboQuestionLang->addItems(settings->languages());
    m_ComboAnswerLang->addItems(settings->languages());

    m_ComboDictionary->setCurrentIndex(settings->openedDictionary());
}

void TrainingSelectionView::slotDictionarySelected(int index)
{
    if(index >= 0) {
        QString dictionaryName = m_ComboDictionary->itemText(index);
        QueryMeeSettings::instance()->openDictionary(dictionaryName);
    }
}
