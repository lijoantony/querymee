/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocconjugation.h
***************************************************************************/

/***************************************************************************

    C++ Interface: qtvtvocconjugation

    -----------------------------------------------------------------------

    begin         : Di Aug 28 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright     : (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

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


#ifndef QMVOCCONJUGATION_H
#define QMVOCCONJUGATION_H

#include "libqmvocdocument_export.h"
#include "qmvoctext.h"
#include "qmvocwordflags.h"

#include <QtCore/QStringList>

/**
 * The conjugation of a verb
 */
class QTVTVOCDOCUMENT_EXPORT QmVocConjugation
{
public:

    /**
     * The constructor
     */
    explicit QmVocConjugation();

    QmVocConjugation( const QmVocConjugation& rhs );

    ~QmVocConjugation();

    QmVocConjugation& operator = ( const QmVocConjugation& a );
    bool operator == ( const QmVocConjugation& a ) const;

    QmVocText& conjugation(QmVocWordFlags flags) const;
    void setConjugation(const QmVocText& conjugation, QmVocWordFlags flags);

    QList<QmVocWordFlags> keys();

    bool isEmpty();

    /**
     * Create xml for this declension
     * @param parent
     */
    void toKVTML2(QDomElement& parent, const QString &tense);

    /**
     * Reads a declension from xml, returns 0 if it is empty
     * @param parent
     * @return
     */
    static QmVocConjugation* fromKVTML2(QDomElement& parent);

private:
    class Private;
    Private* const d;
};


#endif // QTVTVOCCONJUGATION_H
