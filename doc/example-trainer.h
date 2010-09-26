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

#ifndef QMEXAMPLE_H
#define QMEXAMPLE_H

#include "qmtrainer.h"

class QmEXAMPLE : public QmTrainer
{
    Q_OBJECT
public:
    explicit QmEXAMPLE(QWidget *parent = 0);
    ~QmEXAMPLE();
    void startTraining();

signals:

public slots:

private:
    void closeEvent ( QCloseEvent * event );

};

#endif // QMEXAMPLE_H
