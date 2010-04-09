/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocpaukerreader.h
***************************************************************************/

/***************************************************************************
                  create a QTvtVocDocument from a Pauker file
    -----------------------------------------------------------------------
    copyright     : (C) 2004, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright     : (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QTVTVOCPAUKERREADER_H
#define QTVTVOCPAUKERREADER_H

#include <QXmlStreamReader>

class QIODevice;
class QTvtVocDocument;

class QTvtVocPaukerReader : public QXmlStreamReader
{
public:
    QTvtVocPaukerReader( QTvtVocDocument *doc );

    bool read( QIODevice *device );

private:
    void readUnknownElement();
    void readPauker();
    void readBatch();
    void readCard();
    QString readText();

    QTvtVocDocument *m_doc;
};

#endif
