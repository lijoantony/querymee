/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocwordtype.cpp
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

#include "qtvtvocwordtype.h"

#include "qtvtvocexpression.h"

#include <QtCore/QList>
#include <QtCore/QSet>

class QmVocWordType::Private
{
public:
    // bitvector of word type flags
    QmVocWordFlags m_flags;
    QList<QmVocExpression*> m_expressions;
    // list of translations
    QList<QmVocTranslation*> m_translations;
};

QmVocWordType::QmVocWordType(const QString& name, QmVocWordType *parent)
        : QmVocContainer(name, WordType, parent), d( new Private )
{}

QmVocWordType::~QmVocWordType()
{
    foreach(QmVocTranslation* translation, d->m_translations) {
        translation->setWordType(0);
    }
    delete d;
}

QList<QmVocExpression*> QmVocWordType::entries(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive();
    }

    return d->m_expressions;
}

int QmVocWordType::entryCount(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().count();
    }
    return d->m_expressions.count();
}

void QmVocWordType::addTranslation(QmVocTranslation* translation)
{
    // add to expression - if not already there because another translation of the same word is there.
    bool found = false;
    foreach(int i, translation->entry()->translationIndices()) {
        if (translation->entry()->translation(i)->wordType() == this) {
            found = true;
            break;
        }
    }
    if (!found) {
        d->m_expressions.append(translation->entry());
    }
    d->m_translations.append( translation );
    invalidateChildLessonEntries();
}

void QmVocWordType::removeTranslation(QmVocTranslation* translation)
{
    d->m_translations.removeAt( d->m_translations.indexOf(translation) );

    // no lesson found - this entry is being deleted. remove all its siblings.
    if (!translation->entry()->lesson()) {
        int index = d->m_expressions.indexOf(translation->entry());
        if (index != -1) {
            d->m_expressions.removeAt(index);
        }
    }

    // remove from cache
    bool found = false;
    foreach(int i, translation->entry()->translationIndices()) {
        if (translation->entry()->translation(i)->wordType() && translation->entry()->translation(i)->wordType() == this) {
            found = true;
            break;
        }
    }
    if (!found) {
        d->m_expressions.removeAt(d->m_expressions.indexOf(translation->entry()));
    }

    invalidateChildLessonEntries();
}

QmVocTranslation * QmVocWordType::translation(int row)
{

    return d->m_translations.value(row);
}

QmVocExpression * QmVocWordType::entry(int row, EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().value(row);
    }
    return entries().value(row);
}

QmVocWordFlags QmVocWordType::wordType() const
{
    return d->m_flags;
}

void QmVocWordType::setWordType(QmVocWordFlags flags)
{
    d->m_flags = flags;
}

QmVocWordType* QmVocWordType::childOfType(const QmVocWordFlags& flags)
{
    if(d->m_flags == flags) {
        return this;
    }
    foreach(QmVocContainer* child, childContainers()) {
        QmVocWordType* result = static_cast<QmVocWordType*>(child)->childOfType(flags);
        if(result) {
            return result;
        }
    }
    return 0;
}

