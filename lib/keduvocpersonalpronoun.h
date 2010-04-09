/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocpersonalpronoun.h
***************************************************************************/

/***************************************************************************
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef KEDUVOCPERSONALPRONOUN_H
#define KEDUVOCPERSONALPRONOUN_H

#include "libkeduvocdocument_export.h"
#include "keduvocconjugation.h"
#include <QtCore/QStringList>

/**
 * The conjugation of a verb
 */
class QTVTVOCDOCUMENT_EXPORT QTvtVocPersonalPronoun
{
public:

    /**
     * The constructor
     */
    explicit QTvtVocPersonalPronoun();

    QTvtVocPersonalPronoun( const QTvtVocPersonalPronoun& rhs );

    ~QTvtVocPersonalPronoun();

    QTvtVocPersonalPronoun& operator = ( const QTvtVocPersonalPronoun& a );
    bool operator == ( const QTvtVocPersonalPronoun& a ) const;

    QString personalPronoun(QTvtVocWordFlags flags) const;
    void setPersonalPronoun(const QString& conjugation, QTvtVocWordFlags flags);

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



#endif // KEDUVOCCONJUGATION_H

