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


#include "qmvocexpression.h"

// #include <KDebug>


class QmVocExpression::QmVocExpressionPrivate
{
public:
    QmVocExpressionPrivate()
    {
        m_active = true;
        m_lesson = 0;
    }
    ~QmVocExpressionPrivate();

    QmVocExpressionPrivate(const QmVocExpressionPrivate &other);
    QmVocExpressionPrivate& operator= (const QmVocExpressionPrivate &other);

    bool operator== ( const QmVocExpressionPrivate &p ) const;

    QmVocLesson* m_lesson;
    bool m_active;

    QMap <int, QmVocTranslation*> m_translations;
};

QmVocExpression::QmVocExpressionPrivate::~QmVocExpressionPrivate()
{
    QMap <int, QmVocTranslation*> translations = m_translations;
    // empty the translations map, otherwise removal from word type will try to access them again when they don't exist any more
    m_translations.clear();
    qDeleteAll(translations);
}

QmVocExpression::QmVocExpressionPrivate::QmVocExpressionPrivate(const QmVocExpressionPrivate & other)
{
    m_active = other.m_active;
    m_lesson = 0;
}

QmVocExpression::QmVocExpressionPrivate & QmVocExpression::QmVocExpressionPrivate::operator =(const QmVocExpressionPrivate & other)
{
    m_active = other.m_active;
    m_lesson = 0;

    return *this;
}

bool QmVocExpression::QmVocExpressionPrivate::operator== ( const QmVocExpression::QmVocExpressionPrivate &p ) const
{
    return
        m_translations == p.m_translations &&
        m_lesson == p.m_lesson &&
        m_active == p.m_active;
}


QmVocExpression::QmVocExpression()
        : d( new QmVocExpressionPrivate )
{}

QmVocExpression::QmVocExpression( const QString & expression )
        : d( new QmVocExpressionPrivate )
{
    setTranslation( 0, expression.simplified() );
}

QmVocExpression::QmVocExpression( const QStringList & translations)
        : d( new QmVocExpressionPrivate )
{
    foreach ( const QString &translation, translations ) {
        setTranslation(d->m_translations.count(), translation);
    }
}


QmVocExpression::QmVocExpression(const QmVocExpression & other)
    : d(new QmVocExpressionPrivate(*other.d))
{
    foreach (int key, other.d->m_translations.keys()) {
        d->m_translations[key] = new QmVocTranslation(*other.d->m_translations.value(key));
        d->m_translations[key]->setEntry(this);
    }
}

QmVocExpression& QmVocExpression::operator= ( const QmVocExpression &other )
{
    *d = *other.d;
    foreach (int key, other.d->m_translations.keys()) {
        d->m_translations[key] = new QmVocTranslation(*other.d->m_translations.value(key));
        d->m_translations[key]->setEntry(this);
    }
    return *this;
}

QmVocExpression::~QmVocExpression()
{
    setLesson(0);
    delete d;
}

void QmVocExpression::removeTranslation( int index )
{
    int count = d->m_translations.count();

    // remove the index we delete
    delete d->m_translations.take(index);

    // shift all other indexes, +1 for the deleted
    for (int j = index; j <  count-1; j++) {
        d->m_translations[j] = d->m_translations.take(j+1);
    }
}


void QmVocExpression::setTranslation( int index, const QString & expr )
{
    if ( index < 0 ) {
        return;
    }

    if (!d->m_translations.contains(index)) {
        d->m_translations[index] = new QmVocTranslation(this);
    }
    d->m_translations[index]->setText(expr.simplified());
}


QmVocLesson* QmVocExpression::lesson() const
{
    return d->m_lesson;
}


bool QmVocExpression::isActive() const
{
    return d->m_active;
}


void QmVocExpression::setActive( bool flag )
{
    d->m_active = flag;
}


void QmVocExpression::resetGrades( int index )
{
    if ( index == -1 ) { // clear grades for all languages
        foreach( QmVocTranslation* trans, d->m_translations ) {
            trans->resetGrades();
        }
        return;
    }

    // only language index
    if ( d->m_translations.contains( index ) ) {
        d->m_translations[index]->resetGrades();
    }
}

bool QmVocExpression::operator== ( const QmVocExpression &expression ) const
{
    return ( *d == *expression.d );
}

QmVocTranslation* QmVocExpression::translation( int index )
{
    if(translationIndices().contains(index)) {
        return d->m_translations[index];
    }
    d->m_translations[index] = new QmVocTranslation(this);
    return d->m_translations[index];
}

QmVocTranslation * QmVocExpression::translation(int index) const
{
    if(d->m_translations.contains(index)) {
        return 0;
    }
    return d->m_translations[index];
}

QList< int > QmVocExpression::translationIndices() const
{
    return d->m_translations.keys();
}

void QmVocExpression::setLesson(QmVocLesson * l)
{
    if (d->m_lesson) {
        d->m_lesson->removeEntry(this);
    }
    d->m_lesson = l;
}


