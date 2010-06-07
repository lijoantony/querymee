/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocxdxfreader.h
***************************************************************************/

/***************************************************************************
                  create a QmVocDocument from a XDXF file
    -----------------------------------------------------------------------
    copyright     : (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#ifndef QTVTVOCXDXFREADER_H
#define QTVTVOCXDXFREADER_H

#include <QXmlStreamReader>

class QIODevice;
class QmVocDocument;

class QmVocXdxfReader : public QXmlStreamReader
{
public:
    QmVocXdxfReader( QmVocDocument *doc );

    bool read( QIODevice *device );

private:
    void readUnknownElement();
    void readXdxf();
    void readEntry();

    QmVocDocument *m_doc;
};

#endif
