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
#ifndef QMVOCIDENTIFIER_H
#define QMVOCIDENTIFIER_H

#include "libqtvtvocdocument_export.h"

#include "qtvtvocpersonalpronoun.h"
#include "qtvtvocarticle.h"

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QSet>


/**
Class to store meta information about a language or any other category in the vocabulary.
*/
class QTVTVOCDOCUMENT_EXPORT QmVocIdentifier
{
public:
    /**
     * Default ctor.
     */
    explicit QmVocIdentifier();

    /**
     * Copy ctor.
     * @param other
     */
    QmVocIdentifier( const QmVocIdentifier &other );

    /**
     * dtor
     */
    ~QmVocIdentifier();

    /**
     * assignment operator
     * @param other
     * @return
     */
    QmVocIdentifier& operator= ( const QmVocIdentifier &other );
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
    QmVocArticle& article() const;

    /**
     * Sets the articles for this identifier
     * @param art              article block
     */
    void setArticle( const QmVocArticle& article );

    /**
     * Get the personal pronouns for this identifier
     * @returns a QmVocPersonalPronoun containing the personal pronouns
     */
    QmVocPersonalPronoun& personalPronouns() const;

    /**
     * Sets personal pronouns
     * @param pronouns a QmVocConjugation containing the personal pronouns
     */
    void setPersonalPronouns( const QmVocPersonalPronoun &pronouns );

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
