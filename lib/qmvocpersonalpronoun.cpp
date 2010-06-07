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

#include "qmvocpersonalpronoun.h"
#include "qmvoccommon_p.h"

#include <QtCore/QMap>

class QmVocPersonalPronoun::Private
{
public:
    Private();

    bool m_maleFemaleDifferent;
    bool m_neutralExists;
    bool m_dualExists;
    QMap<QmVocWordFlags, QString> m_personalpronouns;
};


QmVocPersonalPronoun::Private::Private()
{
    m_maleFemaleDifferent = false;
    m_neutralExists = false;
    m_dualExists = false;
}

QmVocPersonalPronoun::QmVocPersonalPronoun()
        : d( new Private )
{}


QmVocPersonalPronoun::QmVocPersonalPronoun( const QmVocPersonalPronoun& other )
        : d( new Private )
{
    d->m_maleFemaleDifferent = other.d->m_maleFemaleDifferent;
    d->m_neutralExists = other.d->m_neutralExists;
    d->m_personalpronouns = other.d->m_personalpronouns;
    d->m_dualExists = other.d->m_dualExists;
}


QmVocPersonalPronoun::~QmVocPersonalPronoun()
{
    delete d;
}


QmVocPersonalPronoun& QmVocPersonalPronoun::operator = ( const QmVocPersonalPronoun& other )
{
    d->m_maleFemaleDifferent = other.d->m_maleFemaleDifferent;
    d->m_neutralExists = other.d->m_neutralExists;
    d->m_personalpronouns = other.d->m_personalpronouns;
    d->m_dualExists = other.d->m_dualExists;
    return *this;
}


bool QmVocPersonalPronoun::operator ==(const QmVocPersonalPronoun& other) const
{
    return d->m_personalpronouns == other.d->m_personalpronouns &&
        d->m_maleFemaleDifferent == other.d->m_maleFemaleDifferent &&
        d->m_neutralExists == other.d->m_neutralExists &&
        d->m_dualExists == other.d->m_dualExists;
}


QString QmVocPersonalPronoun::personalPronoun(QmVocWordFlags flags) const
{
        QString p = d->m_personalpronouns.value(flags & (QmVocWordFlag::persons | QmVocWordFlag::numbers| QmVocWordFlag::genders));
        if (p.isEmpty() && !(flags & QmVocWordFlag::genders) && d->m_maleFemaleDifferent && d->m_neutralExists)
        {
           //initial flag lookup failed, adding neuter gender flag
           flags |= QmVocWordFlag::Neuter;
           p = d->m_personalpronouns.value(flags & (QmVocWordFlag::persons | QmVocWordFlag::numbers| QmVocWordFlag::genders));
        }
        return p;
}

void QmVocPersonalPronoun::setPersonalPronoun(const QString & personalpronoun, QmVocWordFlags flags)
{
        d->m_personalpronouns[flags & (QmVocWordFlag::persons | QmVocWordFlag::numbers| QmVocWordFlag::genders)] = personalpronoun;
}


bool QmVocPersonalPronoun::maleFemaleDifferent() const
{
    return d->m_maleFemaleDifferent;
}

void QmVocPersonalPronoun::setMaleFemaleDifferent(bool different)
{
    d->m_maleFemaleDifferent = different;
}

bool QmVocPersonalPronoun::neutralExists() const
{
    return d->m_neutralExists;
}

void QmVocPersonalPronoun::setNeutralExists(bool exists)
{
    d->m_neutralExists = exists;
}

bool QmVocPersonalPronoun::dualExists() const
{
    return d->m_dualExists;
}

void QmVocPersonalPronoun::setDualExists(bool exists)
{
    d->m_dualExists = exists;
}

