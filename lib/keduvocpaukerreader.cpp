/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocpaukerreader.cpp
***************************************************************************/

/***************************************************************************
                     create a QTvtVocDocument from a Pauker file
    -----------------------------------------------------------------------
    copyright     : (C) 2004, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#include "keduvocpaukerreader.h"

#include <QIODevice>

// #include <KLocale>

#include "keduvocexpression.h"
#include "keduvoclesson.h"
#include "keduvocdocument.h"

QTvtVocPaukerReader::QTvtVocPaukerReader( QTvtVocDocument * doc )
{
    m_doc = doc;
}


bool QTvtVocPaukerReader::read( QIODevice * device )
{
    setDevice( device );

    while ( !atEnd() ) {
        readNext();

        if ( isStartElement() ) {
            if ( name() == "Lesson" )
                readPauker();
            else
                raiseError(  "This is not a Pauker document"  );
        }
    }

    return !error();
}


void QTvtVocPaukerReader::readUnknownElement()
{
    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() )
            readUnknownElement();
    }
}


void QTvtVocPaukerReader::readPauker()
{
    m_doc->setAuthor( "http://pauker.sf.net" );
    ///Pauker does not provide any column titles
    m_doc->appendIdentifier();
    m_doc->appendIdentifier();

    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() ) {
            if ( name() == "Description" )
                m_doc->setDocumentComment( readElementText() );
            else if ( name() == "Batch" )
                readBatch();
            else
                readUnknownElement();
        }
    }
}


void QTvtVocPaukerReader::readBatch()
{
    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() ) {
            if ( name() == "Card" )
                readCard();
            else
                readUnknownElement();
        }
    }
}


void QTvtVocPaukerReader::readCard()
{
    QString front;
    QString back;

    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() ) {
            if ( name() == "FrontSide" )
                front = readText();
            else if ( name() == "ReverseSide" )
                back = readText();
            else
                readUnknownElement();
        }
    }

    QTvtVocLesson* lesson = new QTvtVocLesson("Vocabulary", m_doc->lesson());
    m_doc->lesson()->appendChildContainer(lesson);

    QTvtVocExpression* expr = new QTvtVocExpression( QStringList() << front << back);
    lesson->appendEntry( expr );
}


QString QTvtVocPaukerReader::readText()
{
    QString result;

    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() ) {
            if ( name() == "Text" )
                result = readElementText();
            else
                readUnknownElement();
        }
    }
    return result;
}
