/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocarticle.cpp
***************************************************************************/

/***************************************************************************

    Copyright 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>
    Copyright 2004, 2005, 2007 Peter Hedlund <peter.hedlund@kdemail.net>
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

#include "qmvocarticle.h"
#include "qmvocwordflags.h"

#include <QtCore/QMap>

class QmVocArticle::Private
{
public:
    QMap <QmVocWordFlags, QString>    m_articles;
};

QmVocArticle::QmVocArticle()
        :d( new Private )
{}

QmVocArticle::QmVocArticle( const QmVocArticle &other )
        :d( new Private )
{
    d->m_articles = other.d->m_articles;
}

QmVocArticle &QmVocArticle::operator= ( const QmVocArticle& other )
{
    d->m_articles = other.d->m_articles;
    return *this;
}

QmVocArticle::QmVocArticle( const QString &fem_def, const QString &fem_indef, const QString &mal_def, const QString &mal_indef, const QString &neu_def, const QString &neu_indef )
        :d( new Private )
{
    setArticle( mal_def, QmVocWordFlag::Singular | QmVocWordFlag::Definite | QmVocWordFlag::Masculine );
    setArticle( fem_def, QmVocWordFlag::Singular | QmVocWordFlag::Definite | QmVocWordFlag::Feminine );
    setArticle( neu_def, QmVocWordFlag::Singular | QmVocWordFlag::Definite | QmVocWordFlag::Neuter );

    setArticle( mal_indef, QmVocWordFlag::Singular | QmVocWordFlag::Indefinite | QmVocWordFlag::Masculine );
    setArticle( fem_indef, QmVocWordFlag::Singular | QmVocWordFlag::Indefinite | QmVocWordFlag::Feminine );
    setArticle( neu_indef, QmVocWordFlag::Singular | QmVocWordFlag::Indefinite | QmVocWordFlag::Neuter );
}

QmVocArticle::~QmVocArticle()
{
    delete d;
}


QString QmVocArticle::article(const QmVocWordFlags& flags)
{
    return d->m_articles.value(flags & (QmVocWordFlag::genders | QmVocWordFlag::numbers | QmVocWordFlag::Definite | QmVocWordFlag::Indefinite));
}

void QmVocArticle::setArticle(const QString & article, const QmVocWordFlags& flags)
{
    d->m_articles[flags & (QmVocWordFlag::genders | QmVocWordFlag::numbers | QmVocWordFlag::Definite | QmVocWordFlag::Indefinite)] = article;
}


bool QmVocArticle::isArticle(const QString & article) const
{
    return d->m_articles.values().contains(article);
}

bool QmVocArticle::isEmpty()
{
    // we don't count empty strings as articles
    foreach(const QString& s, d->m_articles)
    {
        if (!s.isEmpty())
            return false;
    }
    return true;
}



