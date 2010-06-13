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

#ifndef KNSXMLSTREAMREADER_H
#define KNSXMLSTREAMREADER_H

#include <QXmlStreamReader>

class QListWidget;
class QListWidgetItem;
class QXmlStreamReader;

class KNSXmlStreamReader
{
public:
    KNSXmlStreamReader(QListWidget *list);

    bool readFile(const QString &fileName);

private:
    void readKnewStuffElement();
    void readStuffElement();
    void readNameElement(QListWidgetItem *item);
    void readSummaryElement(QListWidgetItem *item);
    void readPayloadElement(QListWidgetItem *item);
    void skipUnknownElement();


    QXmlStreamReader reader;
    QListWidget *listWidget;
};

#endif
