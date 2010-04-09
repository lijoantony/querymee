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

#include "qtvtvocmultiplechoice.h"

class QTvtVocMultipleChoice::QTvtVocMultipleChoicePrivate
{
public:
    QStringList m_choices;
};

QTvtVocMultipleChoice::QTvtVocMultipleChoice()
        : d( new QTvtVocMultipleChoicePrivate )
{}

QTvtVocMultipleChoice::QTvtVocMultipleChoice( const QTvtVocMultipleChoice &other )
        : d( new QTvtVocMultipleChoicePrivate )
{
    d->m_choices = other.choices();
}

QTvtVocMultipleChoice::QTvtVocMultipleChoice( const QStringList &choices )
        : d( new QTvtVocMultipleChoicePrivate )
{
    foreach ( const QString &choice, choices ) {
        // use appendChoice to check for empty entries
        appendChoice(choice);
    }
}

QTvtVocMultipleChoice::~QTvtVocMultipleChoice()
{
    delete d;
}

QTvtVocMultipleChoice &QTvtVocMultipleChoice::operator= ( const QTvtVocMultipleChoice &other )
{
    d->m_choices = other.choices();
    return *this;
}

QStringList QTvtVocMultipleChoice::choices() const
{
    return d->m_choices;
}

bool QTvtVocMultipleChoice::isEmpty() const
{
    return d->m_choices.isEmpty();
}


void QTvtVocMultipleChoice::clear()
{
    d->m_choices.clear();
}


QString QTvtVocMultipleChoice::choice( int index ) const
{
    QString choice;
    if ( d->m_choices.size() > index ) {
        choice = d->m_choices[index];
    }
    return choice;
}

unsigned QTvtVocMultipleChoice::size() const
{
    return d->m_choices.size();
}

bool QTvtVocMultipleChoice::operator== ( const QTvtVocMultipleChoice &choice ) const
{
    return d->m_choices == choice.choices();
}

void QTvtVocMultipleChoice::appendChoice( const QString &s )
{
    if ( !s.isEmpty() ) {
        d->m_choices.append( s );
    }
}
