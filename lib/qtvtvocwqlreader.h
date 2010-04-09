/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocwqlreader.h
***************************************************************************/

/***************************************************************************
                     read a QTvtVocDocument from a WQL file
    -----------------------------------------------------------------------
    copyright     : (C) 2004, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                  : (c) 2005 Eric Pignet

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
class QTvtVocDocument;

class QTvtVocWqlReader
{
public:
    QTvtVocWqlReader( QIODevice *file );

    bool readDoc( QTvtVocDocument *doc );

    QString errorMessage() const
    {
        return m_errorMessage;
    }

private:
    QIODevice *m_inputFile;
    QTvtVocDocument *m_doc;
    QString m_errorMessage;
};

#endif
