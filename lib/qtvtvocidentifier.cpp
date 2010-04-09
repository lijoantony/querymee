/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocidentifier.cpp
***************************************************************************/

/***************************************************************************
    Copyright 2007-2008 Frederik Gladhorn <gladhorn@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qtvtvocidentifier.h"

class QTvtVocIdentifier::Private
{
public:
    /// the name: English, Anatomy, Fruit salad
    QString m_name;
    /// the locale: en, de, es, ...
    QString m_locale;

    /**
     * Let the user provide some additional informatioin about the language.
     * This could be Traditional/Simplified for chinese to differentiate between them.
     */
    QString m_comment;

    /** not sure yet: language|other|??? */
    QString m_type;

    /** I, you, he, she, it... */
    QTvtVocPersonalPronoun m_personalPronouns;

    /** the for english ;)
       der, die, das ... in german */
    QTvtVocArticle m_articles;

    /** Future, present and past... and many more */
    QStringList m_tenses;
};

QTvtVocIdentifier::QTvtVocIdentifier()
: d( new Private )
{
    ///@todo maybe the user locale would be more appropriate
    d->m_locale = "en";
}

QTvtVocIdentifier::~QTvtVocIdentifier()
{
    delete d;
}

QTvtVocIdentifier::QTvtVocIdentifier( const QTvtVocIdentifier &other )
: d( new Private( *other.d ) )
{
#if 0
    d->m_locale = other.d->m_locale;
    d->m_name = other.d->m_name;
    d->m_articles = other.d->m_articles;
    d->m_personalPronouns = other.d->m_personalPronouns;
    d->m_comment = other.d->m_comment;
    d->m_tenses = other.d->m_tenses;
    d->m_type = other.d->m_type;
#endif
}

QTvtVocIdentifier& QTvtVocIdentifier::operator= ( const QTvtVocIdentifier &other )
{
    d->m_locale = other.d->m_locale;
    d->m_name = other.d->m_name;
    d->m_articles = other.d->m_articles;
    d->m_personalPronouns = other.d->m_personalPronouns;
    d->m_comment = other.d->m_comment;
    d->m_tenses = other.d->m_tenses;
    d->m_type = other.d->m_type;
    return *this;
}

QString QTvtVocIdentifier::name() const
{
    return d->m_name;
}

void QTvtVocIdentifier::setName(const QString & name)
{
    d->m_name = name;
}

QString QTvtVocIdentifier::locale() const
{
    return d->m_locale;
}

void QTvtVocIdentifier::setLocale(const QString & locale)
{
    d->m_locale = locale;
}

void QTvtVocIdentifier::setArticle( const QTvtVocArticle& articles )
{
    d->m_articles = articles;
}

QTvtVocArticle& QTvtVocIdentifier::article() const
{
    return d->m_articles;
}

QTvtVocPersonalPronoun& QTvtVocIdentifier::personalPronouns() const
{
    return d->m_personalPronouns;
}

void QTvtVocIdentifier::setPersonalPronouns( const QTvtVocPersonalPronoun & pronouns )
{
    d->m_personalPronouns = pronouns;
}

QString QTvtVocIdentifier::tense(int tenseIndex) const
{
    Q_ASSERT(d->m_tenses.size() > tenseIndex);
    return d->m_tenses.value(tenseIndex);
}

void QTvtVocIdentifier::setTense(int tenseIndex, const QString& tense)
{
    Q_ASSERT(d->m_tenses.size() >= tenseIndex);
    if (tenseIndex == d->m_tenses.size()) {
        d->m_tenses.append(tense);
    } else {
        d->m_tenses[tenseIndex] = tense;
    }
}

QStringList QTvtVocIdentifier::tenseList() const
{
    return d->m_tenses;
}

void QTvtVocIdentifier::setTenseList(const QStringList& tenses)
{
    d->m_tenses = tenses;
}

