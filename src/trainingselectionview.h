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

#ifndef TRAININGSELECTIONVIEW_H
#define TRAININGSELECTIONVIEW_H

#include <QWidget>
#include <QCheckBox>
#include <QPushButton>

class QComboBox;

class TrainingSelectionView : public QWidget
{
    Q_OBJECT

public:
    TrainingSelectionView(QWidget* parent = 0);
    void setTrainingsLessons(QList<int>*);

public Q_SLOTS:
    /*!
     * \brief Starts training
     */
    void start();
    void slotInitView();
    void slotLessonDialog();

private Q_SLOTS:
    void slotDictionaryChanged();
    void slotDictionarySelected(int index);


private:
    QComboBox *m_ComboDictionary;
    QList<int> *m_TrainingLessons;
    QComboBox *m_ComboQuestionLang;
    QComboBox *m_ComboAnswerLang;
    QComboBox *m_ComboTrainer;
    QCheckBox *m_checkbox_random;
    QCheckBox *m_checkbox_revice;
    QPushButton *m_button;
#ifdef Q_WS_MAEMO_5
    QCheckBox *m_checkbox_portrait;
#endif
};

#endif // TRAININGSELECTIONVIEW_H
