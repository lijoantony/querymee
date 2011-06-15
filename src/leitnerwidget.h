/***************************************************************************

    Copyright 2011 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LEITNERWIDGET_H
#define LEITNERWIDGET_H

#include <QWidget>
#include <QList>
#include <QProgressBar>

class LeitnerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeitnerWidget(QWidget *parent = 0);

signals:

public slots:
    void updateProgressBar(int, int);
    void updateMaxProgressBar(int, int);

private:
    QList<QProgressBar*> *progressBarsList;
};

#endif // LEITNERWIDGET_H
