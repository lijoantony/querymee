/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocpaukerreader.cpp
***************************************************************************/

/***************************************************************************
                     create a QmVocDocument from a Pauker file
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

#include "qtvtvocpaukerreader.h"

#include <QIODevice>

// #include <KLocale>

#include "qtvtvocexpression.h"
#include "qtvtvoclesson.h"
#include "qtvtvocdocument.h"

QmVocPaukerReader::QmVocPaukerReader( QmVocDocument * doc )
{
    m_doc = doc;
}


bool QmVocPaukerReader::read( QIODevice * device )
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


void QmVocPaukerReader::readUnknownElement()
{
    while ( !atEnd() ) {
        readNext();

        if ( isEndElement() )
            break;

        if ( isStartElement() )
            readUnknownElement();
    }
}


void QmVocPaukerReader::readPauker()
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


void QmVocPaukerReader::readBatch()
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


void QmVocPaukerReader::readCard()
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

    QmVocLesson* lesson = new QmVocLesson("Vocabulary", m_doc->lesson());
    m_doc->lesson()->appendChildContainer(lesson);

    QmVocExpression* expr = new QmVocExpression( QStringList() << front << back);
    lesson->appendEntry( expr );
}


QString QmVocPaukerReader::readText()
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
