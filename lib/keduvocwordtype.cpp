/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocwordtype.cpp
***************************************************************************/

/***************************************************************************

    Copyright 2007 Jeremy Whiting <jpwhiting@kde.org>
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "keduvocwordtype.h"

#include "keduvocexpression.h"

#include <QtCore/QList>
#include <QtCore/QSet>

class QTvtVocWordType::Private
{
public:
    // bitvector of word type flags
    QTvtVocWordFlags m_flags;
    QList<QTvtVocExpression*> m_expressions;
    // list of translations
    QList<QTvtVocTranslation*> m_translations;
};

QTvtVocWordType::QTvtVocWordType(const QString& name, QTvtVocWordType *parent)
        : QTvtVocContainer(name, WordType, parent), d( new Private )
{}

QTvtVocWordType::~QTvtVocWordType()
{
    foreach(QTvtVocTranslation* translation, d->m_translations) {
        translation->setWordType(0);
    }
    delete d;
}

QList<QTvtVocExpression*> QTvtVocWordType::entries(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive();
    }

    return d->m_expressions;
}

int QTvtVocWordType::entryCount(EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().count();
    }
    return d->m_expressions.count();
}

void QTvtVocWordType::addTranslation(QTvtVocTranslation* translation)
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

void QTvtVocWordType::removeTranslation(QTvtVocTranslation* translation)
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

QTvtVocTranslation * QTvtVocWordType::translation(int row)
{

    return d->m_translations.value(row);
}

QTvtVocExpression * QTvtVocWordType::entry(int row, EnumEntriesRecursive recursive)
{
    if (recursive == Recursive) {
        return entriesRecursive().value(row);
    }
    return entries().value(row);
}

QTvtVocWordFlags QTvtVocWordType::wordType() const
{
    return d->m_flags;
}

void QTvtVocWordType::setWordType(QTvtVocWordFlags flags)
{
    d->m_flags = flags;
}

QTvtVocWordType* QTvtVocWordType::childOfType(const QTvtVocWordFlags& flags)
{
    if(d->m_flags == flags) {
        return this;
    }
    foreach(QTvtVocContainer* child, childContainers()) {
        QTvtVocWordType* result = static_cast<QTvtVocWordType*>(child)->childOfType(flags);
        if(result) {
            return result;
        }
    }
    return 0;
}

