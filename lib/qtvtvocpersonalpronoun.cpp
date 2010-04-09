/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocpersonalpronoun.cpp
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

#include "qtvtvocpersonalpronoun.h"
#include "qtvtvoccommon_p.h"

#include <QtCore/QMap>

class QTvtVocPersonalPronoun::Private
{
public:
    Private();

    bool m_maleFemaleDifferent;
    bool m_neutralExists;
    bool m_dualExists;
    QMap<QTvtVocWordFlags, QString> m_personalpronouns;
};


QTvtVocPersonalPronoun::Private::Private()
{
    m_maleFemaleDifferent = false;
    m_neutralExists = false;
    m_dualExists = false;
}

QTvtVocPersonalPronoun::QTvtVocPersonalPronoun()
        : d( new Private )
{}


QTvtVocPersonalPronoun::QTvtVocPersonalPronoun( const QTvtVocPersonalPronoun& other )
        : d( new Private )
{
    d->m_maleFemaleDifferent = other.d->m_maleFemaleDifferent;
    d->m_neutralExists = other.d->m_neutralExists;
    d->m_personalpronouns = other.d->m_personalpronouns;
    d->m_dualExists = other.d->m_dualExists;
}


QTvtVocPersonalPronoun::~QTvtVocPersonalPronoun()
{
    delete d;
}


QTvtVocPersonalPronoun& QTvtVocPersonalPronoun::operator = ( const QTvtVocPersonalPronoun& other )
{
    d->m_maleFemaleDifferent = other.d->m_maleFemaleDifferent;
    d->m_neutralExists = other.d->m_neutralExists;
    d->m_personalpronouns = other.d->m_personalpronouns;
    d->m_dualExists = other.d->m_dualExists;
    return *this;
}


bool QTvtVocPersonalPronoun::operator ==(const QTvtVocPersonalPronoun& other) const
{
    return d->m_personalpronouns == other.d->m_personalpronouns &&
        d->m_maleFemaleDifferent == other.d->m_maleFemaleDifferent &&
        d->m_neutralExists == other.d->m_neutralExists &&
        d->m_dualExists == other.d->m_dualExists;
}


QString QTvtVocPersonalPronoun::personalPronoun(QTvtVocWordFlags flags) const
{
        QString p = d->m_personalpronouns.value(flags & (QTvtVocWordFlag::persons | QTvtVocWordFlag::numbers| QTvtVocWordFlag::genders));
        if (p.isEmpty() && !(flags & QTvtVocWordFlag::genders) && d->m_maleFemaleDifferent && d->m_neutralExists)
        {
           //initial flag lookup failed, adding neuter gender flag
           flags |= QTvtVocWordFlag::Neuter;
           p = d->m_personalpronouns.value(flags & (QTvtVocWordFlag::persons | QTvtVocWordFlag::numbers| QTvtVocWordFlag::genders));
        }
        return p;
}

void QTvtVocPersonalPronoun::setPersonalPronoun(const QString & personalpronoun, QTvtVocWordFlags flags)
{
        d->m_personalpronouns[flags & (QTvtVocWordFlag::persons | QTvtVocWordFlag::numbers| QTvtVocWordFlag::genders)] = personalpronoun;
}


bool QTvtVocPersonalPronoun::maleFemaleDifferent() const
{
    return d->m_maleFemaleDifferent;
}

void QTvtVocPersonalPronoun::setMaleFemaleDifferent(bool different)
{
    d->m_maleFemaleDifferent = different;
}

bool QTvtVocPersonalPronoun::neutralExists() const
{
    return d->m_neutralExists;
}

void QTvtVocPersonalPronoun::setNeutralExists(bool exists)
{
    d->m_neutralExists = exists;
}

bool QTvtVocPersonalPronoun::dualExists() const
{
    return d->m_dualExists;
}

void QTvtVocPersonalPronoun::setDualExists(bool exists)
{
    d->m_dualExists = exists;
}

