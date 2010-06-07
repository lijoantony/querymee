/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocmultiplechoice.h
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


#ifndef QMVOCMULTIPLECHOICE_H
#define QMVOCMULTIPLECHOICE_H

#include "libqmvocdocument_export.h"

#include <QtCore/QStringList>

class QTVTVOCDOCUMENT_EXPORT QmVocMultipleChoice
{
public:
    QmVocMultipleChoice();
    QmVocMultipleChoice( const QmVocMultipleChoice &other );
    QmVocMultipleChoice( const QStringList &choices );

    ~QmVocMultipleChoice();

    QStringList choices() const;
    void appendChoice( const QString &s );

    QString choice( int index ) const;

    bool isEmpty() const;
    void clear();
    unsigned size() const;

    bool operator== ( const QmVocMultipleChoice &choice ) const;

    QmVocMultipleChoice &operator= ( const QmVocMultipleChoice &other );

private:
    class QmVocMultipleChoicePrivate;
    QmVocMultipleChoicePrivate * const d;
};


#endif // MultipleChoice_included
