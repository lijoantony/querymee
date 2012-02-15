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

#ifndef QMFLASHCARD_H
#define QMFLASHCARD_H

#include <QWidget>

#include "qmtrainer.h"

class QmFlashCard : public QmTrainer
{
    Q_OBJECT
public:
    explicit QmFlashCard(QWidget *parent = 0);
    ~QmFlashCard();
    void startTraining();

signals:

public slots:

private slots:
    void button_showBackSideClicked();
    void button_correctClicked();
    void button_wrongClicked();
    void init();

private:
    void closeEvent ( QCloseEvent * event );
    QLabel *label_index;
    QLabel *label_frontside;
    QLabel *label_backside;
};

#endif // QMFLASHCARD_H
