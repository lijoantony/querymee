/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocleitnerbox.cpp
***************************************************************************/

/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

#include "qtvtvocleitnerbox.h"

#include "qtvtvocexpression.h"

#include <QtCore/QSet>

class QmVocLeitnerBox::Private
{
public:
    // cache the entries
    QList<QmVocExpression*> m_expressions;
    // list of translations
    QList<QmVocTranslation*> m_translations;
};

QmVocLeitnerBox::QmVocLeitnerBox(const QString& name, QmVocLeitnerBox *parent)
        : QmVocContainer(name, Leitner, parent), d( new Private )
{
    // only one top level and children, this is only a list
    Q_ASSERT(!parent || !parent->parent());
}

QmVocLeitnerBox::~QmVocLeitnerBox()
{
    foreach(QmVocTranslation* translation, d->m_translations) {
        translation->setLeitnerBox(0);
    }
    delete d;
}

QList<QmVocExpression*> QmVocLeitnerBox::entries(EnumEntriesRecursive recursive)
{
    Q_UNUSED(recursive)
    return d->m_expressions;
}

int QmVocLeitnerBox::entryCount(EnumEntriesRecursive recursive)
{
    Q_UNUSED(recursive)
    return d->m_expressions.count();
}

void QmVocLeitnerBox::addTranslation(QmVocTranslation* translation)
{
    // add to expression - if not already there because another translation of the same word is there.
    bool found = false;
    foreach(int i, translation->entry()->translationIndices()) {
        if (translation->entry()->translation(i)->leitnerBox() == this) {
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

void QmVocLeitnerBox::removeTranslation(QmVocTranslation* translation)
{
    int index = d->m_translations.indexOf(translation);
    d->m_translations.removeAt(index);

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
        if (translation->entry()->translation(i)->leitnerBox() == this) {
            found = true;
            break;
        }
    }
    if (!found) {
        d->m_expressions.removeAt(d->m_expressions.indexOf(translation->entry()));
    }

    invalidateChildLessonEntries();
}

QmVocTranslation * QmVocLeitnerBox::translation(int row)
{
    return d->m_translations.value(row);
}

QmVocExpression * QmVocLeitnerBox::entry(int row, EnumEntriesRecursive recursive)
{
    Q_UNUSED(recursive)
    return entries().value(row);
}

