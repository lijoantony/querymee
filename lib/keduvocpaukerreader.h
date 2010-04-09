/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocpaukerreader.h
***************************************************************************/

/***************************************************************************
                  create a QTvtVocDocument from a Pauker file
    -----------------------------------------------------------------------
    copyright     : (C) 2004, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCPAUKERREADER_H
#define KEDUVOCPAUKERREADER_H

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
