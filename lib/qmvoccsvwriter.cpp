/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoccsvwriter.cpp
***************************************************************************/

/***************************************************************************
                   export a QmVocDocument to a delimited text file
    -----------------------------------------------------------------------
    copyright       : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                      (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                      (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qmvoccsvwriter.h"

#include <QFile>
#include <QTextStream>

// #include <KLocale>

#include "qmvocdocument.h"
#include "qmvoclesson.h"
#include "qmvocexpression.h"

QmVocCsvWriter::QmVocCsvWriter( QFile *file )
{
    // the file must be already open
    m_outputFile = file;
}


bool QmVocCsvWriter::writeDoc( QmVocDocument *doc, const QString &generator )
{
    Q_UNUSED( generator );

    m_doc = doc;

    QString separator = m_doc->csvDelimiter();
    ;

    QTextStream outputStream;
    outputStream.setDevice( m_outputFile );
    outputStream.setCodec( "UTF-8" );

    outputStream <<  "Title:"  << separator << m_doc->title() << "\n";
    outputStream <<  "Author:"  << separator << m_doc->author() << "\n";

    QmVocExpression *expression;
    int idCount = m_doc->identifierCount();
    QString currentRow;

    for ( int e = 0; e < m_doc->lesson()->entryCount(QmVocLesson::Recursive); e++ ) {
        expression = m_doc->lesson()->entries(QmVocLesson::Recursive).value( e );
        currentRow = "";
        bool sep = false;

        for ( int i = 0; i < idCount; i++ ) {
            if ( !sep )
                sep = true;
            else
                currentRow += separator;

            currentRow += expression->translation( i )->text();
        }

        if ( !currentRow.isEmpty() )
            outputStream << currentRow << "\n";
    }

    return true;
}

