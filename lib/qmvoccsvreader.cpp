/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoccsvreader.cpp
***************************************************************************/

/***************************************************************************

                    create a QmVocDocument from a text file

    -----------------------------------------------------------------------

    begin        : Sat Sep 06 11:00:53 MET 1999

    copyright    : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                   (C) 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>

                   (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qmvoccsvreader.h"

#include <QIODevice>
#include <QTextStream>

// #include <klocale.h>


#include "qmvocdocument.h"
#include "qmvoclesson.h"
#include "qmvocexpression.h"

QmVocCsvReader::QmVocCsvReader( QIODevice *file )
{
    // the file must be already open
    m_inputFile = file;
    m_errorMessage = "";
}


bool QmVocCsvReader::readDoc( QmVocDocument *doc )
{
    m_doc = doc;

    QString separator = m_doc->csvDelimiter();

    QTextStream inputStream( m_inputFile );
    inputStream.setCodec( "UTF-8" );
    inputStream.setAutoDetectUnicode( true );
    inputStream.seek( 0 );

    int languageCount = 0;

    QmVocLesson* lesson = new QmVocLesson( "Vocabulary", m_doc->lesson());
    m_doc->lesson()->appendChildContainer(lesson);

    while ( !inputStream.atEnd() ) {
        QString s = inputStream.readLine();

        if ( !s.simplified().isEmpty() ) {
            QmVocExpression* expression = new QmVocExpression( s.split(separator) );
            languageCount = qMax( languageCount,
                expression->translationIndices().count() );
            lesson->appendEntry( expression );
        }
    }

    for ( int j = 0; j < languageCount; j++ )
    {
        m_doc->appendIdentifier();
    }

    return true;
}
