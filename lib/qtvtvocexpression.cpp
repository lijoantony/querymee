/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocexpression.cpp
***************************************************************************/

/***************************************************************************
                        Vocabulary Expression for KDE Edu
    -----------------------------------------------------------------------
    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "keduvocexpression.h"

// #include <KDebug>


class QTvtVocExpression::QTvtVocExpressionPrivate
{
public:
    QTvtVocExpressionPrivate()
    {
        m_active = true;
        m_lesson = 0;
    }
    ~QTvtVocExpressionPrivate();

    QTvtVocExpressionPrivate(const QTvtVocExpressionPrivate &other);
    QTvtVocExpressionPrivate& operator= (const QTvtVocExpressionPrivate &other);

    bool operator== ( const QTvtVocExpressionPrivate &p ) const;

    QTvtVocLesson* m_lesson;
    bool m_active;

    QMap <int, QTvtVocTranslation*> m_translations;
};

QTvtVocExpression::QTvtVocExpressionPrivate::~QTvtVocExpressionPrivate()
{
    QMap <int, QTvtVocTranslation*> translations = m_translations;
    // empty the translations map, otherwise removal from word type will try to access them again when they don't exist any more
    m_translations.clear();
    qDeleteAll(translations);
}

QTvtVocExpression::QTvtVocExpressionPrivate::QTvtVocExpressionPrivate(const QTvtVocExpressionPrivate & other)
{
    m_active = other.m_active;
    m_lesson = 0;
}

QTvtVocExpression::QTvtVocExpressionPrivate & QTvtVocExpression::QTvtVocExpressionPrivate::operator =(const QTvtVocExpressionPrivate & other)
{
    m_active = other.m_active;
    m_lesson = 0;

    return *this;
}

bool QTvtVocExpression::QTvtVocExpressionPrivate::operator== ( const QTvtVocExpression::QTvtVocExpressionPrivate &p ) const
{
    return
        m_translations == p.m_translations &&
        m_lesson == p.m_lesson &&
        m_active == p.m_active;
}


QTvtVocExpression::QTvtVocExpression()
        : d( new QTvtVocExpressionPrivate )
{}

QTvtVocExpression::QTvtVocExpression( const QString & expression )
        : d( new QTvtVocExpressionPrivate )
{
    setTranslation( 0, expression.simplified() );
}

QTvtVocExpression::QTvtVocExpression( const QStringList & translations)
        : d( new QTvtVocExpressionPrivate )
{
    foreach ( const QString &translation, translations ) {
        setTranslation(d->m_translations.count(), translation);
    }
}


QTvtVocExpression::QTvtVocExpression(const QTvtVocExpression & other)
    : d(new QTvtVocExpressionPrivate(*other.d))
{
    foreach (int key, other.d->m_translations.keys()) {
        d->m_translations[key] = new QTvtVocTranslation(*other.d->m_translations.value(key));
        d->m_translations[key]->setEntry(this);
    }
}

QTvtVocExpression& QTvtVocExpression::operator= ( const QTvtVocExpression &other )
{
    *d = *other.d;
    foreach (int key, other.d->m_translations.keys()) {
        d->m_translations[key] = new QTvtVocTranslation(*other.d->m_translations.value(key));
        d->m_translations[key]->setEntry(this);
    }
    return *this;
}

QTvtVocExpression::~QTvtVocExpression()
{
    setLesson(0);
    delete d;
}

void QTvtVocExpression::removeTranslation( int index )
{
    int count = d->m_translations.count();

    // remove the index we delete
    delete d->m_translations.take(index);

    // shift all other indexes, +1 for the deleted
    for (int j = index; j <  count-1; j++) {
        d->m_translations[j] = d->m_translations.take(j+1);
    }
}


void QTvtVocExpression::setTranslation( int index, const QString & expr )
{
    if ( index < 0 ) {
        return;
    }

    if (!d->m_translations.contains(index)) {
        d->m_translations[index] = new QTvtVocTranslation(this);
    }
    d->m_translations[index]->setText(expr.simplified());
}


QTvtVocLesson* QTvtVocExpression::lesson() const
{
    return d->m_lesson;
}


bool QTvtVocExpression::isActive() const
{
    return d->m_active;
}


void QTvtVocExpression::setActive( bool flag )
{
    d->m_active = flag;
}


void QTvtVocExpression::resetGrades( int index )
{
    if ( index == -1 ) { // clear grades for all languages
        foreach( QTvtVocTranslation* trans, d->m_translations ) {
            trans->resetGrades();
        }
        return;
    }

    // only language index
    if ( d->m_translations.contains( index ) ) {
        d->m_translations[index]->resetGrades();
    }
}

bool QTvtVocExpression::operator== ( const QTvtVocExpression &expression ) const
{
    return ( *d == *expression.d );
}

QTvtVocTranslation* QTvtVocExpression::translation( int index )
{
    if(translationIndices().contains(index)) {
        return d->m_translations[index];
    }
    d->m_translations[index] = new QTvtVocTranslation(this);
    return d->m_translations[index];
}

QTvtVocTranslation * QTvtVocExpression::translation(int index) const
{
    if(d->m_translations.contains(index)) {
        return 0;
    }
    return d->m_translations[index];
}

QList< int > QTvtVocExpression::translationIndices() const
{
    return d->m_translations.keys();
}

void QTvtVocExpression::setLesson(QTvtVocLesson * l)
{
    if (d->m_lesson) {
        d->m_lesson->removeEntry(this);
    }
    d->m_lesson = l;
}


