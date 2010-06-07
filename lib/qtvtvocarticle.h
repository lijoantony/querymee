/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocarticle.h
***************************************************************************/

/***************************************************************************

    Copyright 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    Copyright 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

#ifndef QTVTVOCARTICLE_H
#define QTVTVOCARTICLE_H

#include "libqtvtvocdocument_export.h"

#include <QtCore/QStringList>

#include "qtvtvocwordflags.h"

class QmVocWordType;
/**
 * Class representing the articles of a language
 *
 * It contains all available articles of the language as QString
 */
class QTVTVOCDOCUMENT_EXPORT QmVocArticle
{
public:



    /**
     * The constructor without arguments
     */
    explicit QmVocArticle();

    /** copy constructor for d-pointer safety
     * @param other article object to copy
     */
    QmVocArticle( const QmVocArticle &other );

    /**
     * DEPRECATED The constructor with arguments, assumes singular!
     * @param fem_def reference to a QString with the definite female article
     * @param fem_indef reference to a QString with the indefinite female article
     * @param mal_def reference to a QString with the definite male article
     * @param mal_indef reference to a QString with the indefinite male article
     * @param nat_def reference to a QString with the definite neutral article
     * @param nat_indef reference to a QString with the indefinite neutral article
     */
    KDE_DEPRECATED QmVocArticle( const QString &fem_def, const QString &fem_indef, const QString &mal_def, const QString &mal_indef, const QString &nat_def, const QString &nat_indef );

    /**
     * default destructor, deletes the d pointer
     */
    ~QmVocArticle();

    /**
     * assignment operator for d-pointer copying
     */
    QmVocArticle &operator= ( const QmVocArticle& other );

    QString article(const QmVocWordFlags&);

    void setArticle(const QString& article, const QmVocWordFlags&);

    bool isArticle(const QString& article) const;

    bool isEmpty();

private:
    class Private;
    Private * const d;
};


#endif
