/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocdeclension.h
***************************************************************************/

/***************************************************************************

    C++ Interface: qtvtvocdeclension

    -----------------------------------------------------------------------

    begin         : Do Sep 20 2007

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
#ifndef QMVOCDECLENSION_H
#define QMVOCDECLENSION_H

#include "libqtvtvocdocument_export.h"

#include "qtvtvoctext.h"
#include "qtvtvocwordflags.h"

/**
A declension contains all forms that a NOUN possibly can have.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class QTVTVOCDOCUMENT_EXPORT QmVocDeclension{
public:

    /**
     * The constructor without arguments
     */
    explicit QmVocDeclension();

    /** copy constructor
     * @param other comparison object to copy
     */
    QmVocDeclension( const QmVocDeclension &other );

    ~QmVocDeclension();

    /** equality operator
     * @param a object to compare to
     * @returns true if comparisons are the same, false otherwise
     */
//     will probably not be necessary
//     bool operator == ( const QmVocDeclension& a ) const;

    /** assignment operator for d-pointer copying
     * @param other object to copy from
     * @returns reference to this object
     */
    QmVocDeclension& operator= ( const QmVocDeclension& other );

    /**
     * The grammatical number, there is singular and plural for english, some languages have dual for exactly two items.
     * @param gender
     * @param decCase
     * @return
     */
    QmVocText& declension(QmVocWordFlags flags);

    /**
     * Set a declension
     * @param
     * @param number
     * @param decCase
     */
    void setDeclension(const QmVocText& declension, QmVocWordFlags flags);

    bool isEmpty();


    /**
     * Create xml for this declension
     * @param parent
     */
    void toKVTML2(QDomElement& parent);

    /**
     * Reads a declension from xml, returns 0 if it is empty
     * @param parent
     * @return
     */
    static QmVocDeclension* fromKVTML2(QDomElement& parent);

private:
    class Private;
    Private * const d;
};

#endif
