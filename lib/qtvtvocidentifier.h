/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocidentifier.h
***************************************************************************/

/***************************************************************************
    Copyright 2007-2008 Frederik Gladhorn <gladhorn@kde.org>
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
#ifndef QTVTVOCIDENTIFIER_H
#define QTVTVOCIDENTIFIER_H

#include "libqtvtvocdocument_export.h"

#include "qtvtvocpersonalpronoun.h"
#include "qtvtvocarticle.h"

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QSet>


/**
Class to store meta information about a language or any other category in the vocabulary.
*/
class QTVTVOCDOCUMENT_EXPORT QTvtVocIdentifier
{
public:
    /**
     * Default ctor.
     */
    explicit QTvtVocIdentifier();

    /**
     * Copy ctor.
     * @param other
     */
    QTvtVocIdentifier( const QTvtVocIdentifier &other );

    /**
     * dtor
     */
    ~QTvtVocIdentifier();

    /**
     * assignment operator
     * @param other
     * @return
     */
    QTvtVocIdentifier& operator= ( const QTvtVocIdentifier &other );
public:

    /**
     * Name of this identifier. (English, Anatomy, Fruit salad...)
     * @return name
     */
    QString name() const;
    /**
     * Set the name
     * @param name
     */
    void setName( const QString& name );

    /**
     * The locale of the contents: en, de, es, ...
     * @return locale
     */
    QString locale() const;
    /**
     * Set the locale
     * @param name
     */
    void setLocale( const QString& name );

    /**
     * Articles (a, the in English, el, la,... in Spanish)
     * @returns articles
     */
    QTvtVocArticle& article() const;

    /**
     * Sets the articles for this identifier
     * @param art              article block
     */
    void setArticle( const QTvtVocArticle& article );

    /**
     * Get the personal pronouns for this identifier
     * @returns a QTvtVocPersonalPronoun containing the personal pronouns
     */
    QTvtVocPersonalPronoun& personalPronouns() const;

    /**
     * Sets personal pronouns
     * @param pronouns a QTvtVocConjugation containing the personal pronouns
     */
    void setPersonalPronouns( const QTvtVocPersonalPronoun &pronouns );

    /**
     * Returns the name of tense number @p tenseIndex
     * @param tenseIndex desired tense
     * @return name of the tense
     */
    QString tense(int tenseIndex) const;

    /**
     * Sets the name of a tense for this language
     * @param tenseIndex 
     * @param tense 
     */
    void setTense(int tenseIndex, const QString& tense);

    QStringList tenseList() const;

    void setTenseList(const QStringList& tenses);

private:
    class Private;
    Private * const d;
};

#endif
