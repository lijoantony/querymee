/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocdeclension.h
***************************************************************************/

/***************************************************************************

    C++ Interface: qtvtvocdeclension

    -----------------------------------------------------------------------

    begin         : Do Sep 20 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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
#ifndef QTVTVOCDECLENSION_H
#define QTVTVOCDECLENSION_H

#include "libqtvtvocdocument_export.h"

#include "qtvtvoctext.h"
#include "qtvtvocwordflags.h"

/**
A declension contains all forms that a NOUN possibly can have.

	@author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class QTVTVOCDOCUMENT_EXPORT QTvtVocDeclension{
public:

    /**
     * The constructor without arguments
     */
    explicit QTvtVocDeclension();

    /** copy constructor
     * @param other comparison object to copy
     */
    QTvtVocDeclension( const QTvtVocDeclension &other );

    ~QTvtVocDeclension();

    /** equality operator
     * @param a object to compare to
     * @returns true if comparisons are the same, false otherwise
     */
//     will probably not be necessary
//     bool operator == ( const QTvtVocDeclension& a ) const;

    /** assignment operator for d-pointer copying
     * @param other object to copy from
     * @returns reference to this object
     */
    QTvtVocDeclension& operator= ( const QTvtVocDeclension& other );

    /**
     * The grammatical number, there is singular and plural for english, some languages have dual for exactly two items.
     * @param gender
     * @param decCase
     * @return
     */
    QTvtVocText& declension(QTvtVocWordFlags flags);

    /**
     * Set a declension
     * @param
     * @param number
     * @param decCase
     */
    void setDeclension(const QTvtVocText& declension, QTvtVocWordFlags flags);

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
    static QTvtVocDeclension* fromKVTML2(QDomElement& parent);

private:
    class Private;
    Private * const d;
};

#endif
