/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoccsvreader.h
***************************************************************************/

/***************************************************************************
                  create a QTvtVocDocument from a text file
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

#ifndef QTVTVOCCSVREADER_H
#define QTVTVOCCSVREADER_H

#include <QString>

class QIODevice;

class QTvtVocDocument;

class QTvtVocCsvReader
{
public:
    QTvtVocCsvReader( QIODevice *file );

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
