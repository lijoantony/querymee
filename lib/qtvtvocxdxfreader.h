/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocxdxfreader.h
***************************************************************************/

/***************************************************************************
                  create a QTvtVocDocument from a XDXF file
    -----------------------------------------------------------------------
    copyright     : (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QTVTVOCXDXFREADER_H
#define QTVTVOCXDXFREADER_H

#include <QXmlStreamReader>

class QIODevice;
class QTvtVocDocument;

class QTvtVocXdxfReader : public QXmlStreamReader
{
public:
    QTvtVocXdxfReader( QTvtVocDocument *doc );

    bool read( QIODevice *device );

private:
    void readUnknownElement();
    void readXdxf();
    void readEntry();

    QTvtVocDocument *m_doc;
};

#endif
