/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocwqlreader.h
***************************************************************************/

/***************************************************************************
                     read a QmVocDocument from a WQL file
    -----------------------------------------------------------------------
    copyright     : (C) 2004, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                  : (c) 2005 Eric Pignet
                  : (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QTVTVOCWQLREADER_H
#define QTVTVOCWQLREADER_H

#include <QString>

class QIODevice;
class QmVocDocument;

class QmVocWqlReader
{
public:
    QmVocWqlReader( QIODevice *file );

    bool readDoc( QmVocDocument *doc );

    QString errorMessage() const
    {
        return m_errorMessage;
    }

private:
    QIODevice *m_inputFile;
    QmVocDocument *m_doc;
    QString m_errorMessage;
};

#endif
