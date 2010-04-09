/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoccsvwriter.h
***************************************************************************/

/***************************************************************************
                    export a QTvtVocDocument to a delimited text file
    -----------------------------------------------------------------------
    copyright           : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                          (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KEDUVOCCSVWRITER_H
#define KEDUVOCCSVWRITER_H

class QFile;
class QString;
class QTvtVocDocument;

class QTvtVocCsvWriter
{
public:
    QTvtVocCsvWriter( QFile *file );

    bool writeDoc( QTvtVocDocument *doc, const QString &generator );

private:
    QFile *m_outputFile;
    QTvtVocDocument *m_doc;
};

#endif
