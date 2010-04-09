/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocmultiplechoice.h
***************************************************************************/

/***************************************************************************

              manage multiple choice suggestions for queries

    -----------------------------------------------------------------------

    begin          : Mon Oct 29 18:09:29 1999

    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005 Peter Hedlund <peter.hedlund@kdemail.net>

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


#ifndef QTVTVOCMULTIPLECHOICE_H
#define QTVTVOCMULTIPLECHOICE_H

#include "libqtvtvocdocument_export.h"

#include <QtCore/QStringList>

class QTVTVOCDOCUMENT_EXPORT QTvtVocMultipleChoice
{
public:
    QTvtVocMultipleChoice();
    QTvtVocMultipleChoice( const QTvtVocMultipleChoice &other );
    QTvtVocMultipleChoice( const QStringList &choices );

    ~QTvtVocMultipleChoice();

    QStringList choices() const;
    void appendChoice( const QString &s );

    QString choice( int index ) const;

    bool isEmpty() const;
    void clear();
    unsigned size() const;

    bool operator== ( const QTvtVocMultipleChoice &choice ) const;

    QTvtVocMultipleChoice &operator= ( const QTvtVocMultipleChoice &other );

private:
    class QTvtVocMultipleChoicePrivate;
    QTvtVocMultipleChoicePrivate * const d;
};


#endif // MultipleChoice_included
