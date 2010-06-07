/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoclesson.cpp
***************************************************************************/

/***************************************************************************

    Copyright 2007 Jeremy Whiting <jpwhiting@kde.org>
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

#include "qtvtvoclesson.h"

#include "qtvtvocexpression.h"

#include <QtCore/QList>

/** private class to store information about a lesson */
class QmVocLesson::Private
{
public:
    // entries
    QList<QmVocExpression*> m_entries;
};


QmVocLesson::QmVocLesson(const QString& name, QmVocContainer *parent)
        : QmVocContainer(name, Lesson, parent), d( new Private )
{
}


QmVocLesson::QmVocLesson( const QmVocLesson &other )
        : QmVocContainer(other), d( new Private )
{
    d->m_entries = other.d->m_entries;
}


QmVocLesson::~QmVocLesson()
{
    qDeleteAll(d->m_entries);
    delete d;
}

QList<QmVocExpression*> QmVocLesson::entries(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive();
    }
    return d->m_entries;
}

int QmVocLesson::entryCount(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().count();
    }
    return d->m_entries.count();
}

void QmVocLesson::appendEntry(QmVocExpression* entry)
{
    insertEntry(d->m_entries.size(), entry);
}

void QmVocLesson::insertEntry(int index, QmVocExpression * entry)
{
    Q_ASSERT(entry);
    d->m_entries.insert( index, entry );
    entry->setLesson(this);
    invalidateChildLessonEntries();
}

void QmVocLesson::removeEntry(QmVocExpression* entry)
{
    Q_ASSERT(entry);
    if (d->m_entries.indexOf(entry) == -1) {
        //attempting to remove entry from lesson that does not contain it!
        return;
    }
    d->m_entries.removeAt( d->m_entries.indexOf(entry) );
    invalidateChildLessonEntries();
}

QmVocExpression * QmVocLesson::entry(int row, EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().value(row);
    }
    return d->m_entries.value(row);
}


