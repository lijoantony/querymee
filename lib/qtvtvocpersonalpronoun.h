/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocpersonalpronoun.h
***************************************************************************/

/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef QTVTVOCPERSONALPRONOUN_H
#define QTVTVOCPERSONALPRONOUN_H

#include "libqtvtvocdocument_export.h"
#include "qtvtvocconjugation.h"
#include <QtCore/QStringList>

/**
 * The conjugation of a verb
 */
class QTVTVOCDOCUMENT_EXPORT QmVocPersonalPronoun
{
public:

    /**
     * The constructor
     */
    explicit QmVocPersonalPronoun();

    QmVocPersonalPronoun( const QmVocPersonalPronoun& rhs );

    ~QmVocPersonalPronoun();

    QmVocPersonalPronoun& operator = ( const QmVocPersonalPronoun& a );
    bool operator == ( const QmVocPersonalPronoun& a ) const;

    QString personalPronoun(QmVocWordFlags flags) const;
    void setPersonalPronoun(const QString& conjugation, QmVocWordFlags flags);

    bool maleFemaleDifferent() const;
    void setMaleFemaleDifferent(bool different);

    bool neutralExists() const;
    void setNeutralExists(bool exists);

    bool dualExists() const;
    void setDualExists(bool exists);
private:
    class Private;
    Private* const d;

};



#endif // QTVTVOCCONJUGATION_H

