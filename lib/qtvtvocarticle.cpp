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

#include "qtvtvocarticle.h"
#include "qtvtvocwordflags.h"

#include <QtCore/QMap>

class QTvtVocArticle::Private
{
public:
    QMap <QTvtVocWordFlags, QString>    m_articles;
};

QTvtVocArticle::QTvtVocArticle()
        :d( new Private )
{}

QTvtVocArticle::QTvtVocArticle( const QTvtVocArticle &other )
        :d( new Private )
{
    d->m_articles = other.d->m_articles;
}

QTvtVocArticle &QTvtVocArticle::operator= ( const QTvtVocArticle& other )
{
    d->m_articles = other.d->m_articles;
    return *this;
}

QTvtVocArticle::QTvtVocArticle( const QString &fem_def, const QString &fem_indef, const QString &mal_def, const QString &mal_indef, const QString &neu_def, const QString &neu_indef )
        :d( new Private )
{
    setArticle( mal_def, QTvtVocWordFlag::Singular | QTvtVocWordFlag::Definite | QTvtVocWordFlag::Masculine );
    setArticle( fem_def, QTvtVocWordFlag::Singular | QTvtVocWordFlag::Definite | QTvtVocWordFlag::Feminine );
    setArticle( neu_def, QTvtVocWordFlag::Singular | QTvtVocWordFlag::Definite | QTvtVocWordFlag::Neuter );

    setArticle( mal_indef, QTvtVocWordFlag::Singular | QTvtVocWordFlag::Indefinite | QTvtVocWordFlag::Masculine );
    setArticle( fem_indef, QTvtVocWordFlag::Singular | QTvtVocWordFlag::Indefinite | QTvtVocWordFlag::Feminine );
    setArticle( neu_indef, QTvtVocWordFlag::Singular | QTvtVocWordFlag::Indefinite | QTvtVocWordFlag::Neuter );
}

QTvtVocArticle::~QTvtVocArticle()
{
    delete d;
}


QString QTvtVocArticle::article(const QTvtVocWordFlags& flags)
{
    return d->m_articles.value(flags & (QTvtVocWordFlag::genders | QTvtVocWordFlag::numbers | QTvtVocWordFlag::Definite | QTvtVocWordFlag::Indefinite));
}

void QTvtVocArticle::setArticle(const QString & article, const QTvtVocWordFlags& flags)
{
    d->m_articles[flags & (QTvtVocWordFlag::genders | QTvtVocWordFlag::numbers | QTvtVocWordFlag::Definite | QTvtVocWordFlag::Indefinite)] = article;
}


bool QTvtVocArticle::isArticle(const QString & article) const
{
    return d->m_articles.values().contains(article);
}

bool QTvtVocArticle::isEmpty()
{
    // we don't count empty strings as articles
    foreach(const QString& s, d->m_articles)
    {
        if (!s.isEmpty())
            return false;
    }
    return true;
}



