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

#ifndef OCSXMLSTREAMREADER_H
#define OCSXMLSTREAMREADER_H

#include <QXmlStreamReader>

class QListWidget;
class QListWidgetItem;
class QXmlStreamReader;


class OCSXmlStreamReader
{
public:
    OCSXmlStreamReader(QListWidget *list);

    bool readFile(const QString &fileName);

private:
    void readOcsElement();
    void readDataElement();
    void readContentElement();
    void readNameElement(QListWidgetItem *item);
    void readDescriptionElement(QListWidgetItem *item);
    void readDownloadlinkElement(QListWidgetItem *item);
    void skipUnknownElement();

    QXmlStreamReader reader;
    QListWidget *listWidget;

};

#endif // OCSXMLSTREAMREADER_H
