/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocmultiplechoice.cpp
***************************************************************************/

/***************************************************************************

              manage multiple choice suggestions for queries

    -----------------------------------------------------------------------

    begin          : Mon Oct 29 18:09:29 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>
                     
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

#include "qmvocmultiplechoice.h"

class QmVocMultipleChoice::QmVocMultipleChoicePrivate
{
public:
    QStringList m_choices;
};

QmVocMultipleChoice::QmVocMultipleChoice()
        : d( new QmVocMultipleChoicePrivate )
{}

QmVocMultipleChoice::QmVocMultipleChoice( const QmVocMultipleChoice &other )
        : d( new QmVocMultipleChoicePrivate )
{
    d->m_choices = other.choices();
}

QmVocMultipleChoice::QmVocMultipleChoice( const QStringList &choices )
        : d( new QmVocMultipleChoicePrivate )
{
    foreach ( const QString &choice, choices ) {
        // use appendChoice to check for empty entries
        appendChoice(choice);
    }
}

QmVocMultipleChoice::~QmVocMultipleChoice()
{
    delete d;
}

QmVocMultipleChoice &QmVocMultipleChoice::operator= ( const QmVocMultipleChoice &other )
{
    d->m_choices = other.choices();
    return *this;
}

QStringList QmVocMultipleChoice::choices() const
{
    return d->m_choices;
}

bool QmVocMultipleChoice::isEmpty() const
{
    return d->m_choices.isEmpty();
}


void QmVocMultipleChoice::clear()
{
    d->m_choices.clear();
}


QString QmVocMultipleChoice::choice( int index ) const
{
    QString choice;
    if ( d->m_choices.size() > index ) {
        choice = d->m_choices[index];
    }
    return choice;
}

unsigned QmVocMultipleChoice::size() const
{
    return d->m_choices.size();
}

bool QmVocMultipleChoice::operator== ( const QmVocMultipleChoice &choice ) const
{
    return d->m_choices == choice.choices();
}

void QmVocMultipleChoice::appendChoice( const QString &s )
{
    if ( !s.isEmpty() ) {
        d->m_choices.append( s );
    }
}
