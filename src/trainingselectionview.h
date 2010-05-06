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

#ifndef TRAININGSELECTIONVIEW_H
#define TRAININGSELECTIONVIEW_H

#include <QWidget>

class QComboBox;

class TrainingSelectionView : public QWidget
{
    Q_OBJECT

public:
    TrainingSelectionView(QWidget* parent = 0);

public Q_SLOTS:
    /*!
     * \brief Starts training
     */
    void start();

private Q_SLOTS:
    void slotDictionaryChanged();
    void slotDictionarySelected(int index);
    void slotInitView();

private:
    QComboBox *m_ComboDictionary;
    QComboBox *m_ComboLesson;
    QComboBox *m_ComboQuestionLang;
    QComboBox *m_ComboAnswerLang;
};

#endif // TRAININGSELECTIONVIEW_H
