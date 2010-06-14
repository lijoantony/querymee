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

#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>

class QListWidget;

class DownloadWidget : public QWidget
{
Q_OBJECT
public:
    explicit DownloadWidget(QWidget *parent = 0);

signals:
    void finished();

public slots:
    void slotDownloadButton();
    void slotUpDateButton();
    void slotItemClicked(QListWidgetItem *item);
    void slotDownloadDone();

private:
    void updateListWidget();

    QListWidget *listWidget;
    QLabel *label;

};

#endif // DOWNLOADWIDGET_H
