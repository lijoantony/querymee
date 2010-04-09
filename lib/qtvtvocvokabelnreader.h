/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocvokabelnreader.h
***************************************************************************/

/***************************************************************************
                     create a QTvtVocDocument from a Vokabeln file
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

#ifndef QTVTVOCVOKABELNREADER_H
#define QTVTVOCVOKABELNREADER_H

#include <QString>

class QIODevice;
class QTvtVocDocument;

class QTvtVocVokabelnReader
{
public:
    QTvtVocVokabelnReader( QIODevice *file );

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
