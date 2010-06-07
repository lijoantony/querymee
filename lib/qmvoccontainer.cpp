/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoccontainer.cpp
***************************************************************************/

/***************************************************************************

    Copyright 2007 Jeremy Whiting <jpwhiting@kde.org>
    Copyright 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

#include "qmvoccontainer.h"

#include "qmvocexpression.h"

// #include <KDebug>
#include <QList>
#include <QtDebug>

/** private class to store information about a lesson */
class QmVocContainer::Private
{
public:
    ~Private();

    // properties for this lesson
    QString m_name;
    bool m_inPractice;

    // other lessons in the tree
    QmVocContainer *m_parentContainer;
    QList < QmVocContainer * > m_childContainers;

    EnumContainerType m_type;

    QList < QmVocExpression* > m_childLessonEntries;
    bool m_childLessonEntriesValid;

    /// Image url
    QUrl m_imageUrl;
};

QmVocContainer::Private::~ Private()
{
    qDeleteAll(m_childContainers);
}

QmVocContainer::QmVocContainer(const QString& name, EnumContainerType type, QmVocContainer *parent)
        : d( new Private )
{
    d->m_parentContainer = parent;
    d->m_name = name;
    d->m_inPractice = true;
    d->m_type = type;
    d->m_childLessonEntriesValid = false;
}

QmVocContainer::QmVocContainer( const QmVocContainer &other )
        : d( new Private )
{
    d->m_name = other.d->m_name;
    d->m_inPractice = other.d->m_inPractice;
    d->m_type = other.d->m_type;
    d->m_parentContainer = other.d->m_parentContainer;
    d->m_childLessonEntriesValid = false;
}

QmVocContainer::~QmVocContainer()
{
    delete d;
}

void QmVocContainer::appendChildContainer(QmVocContainer * child)
{
    d->m_childContainers.append(child);
    child->d->m_parentContainer = this;

    invalidateChildLessonEntries();
}

QmVocContainer * QmVocContainer::childContainer(int row)
{
    return d->m_childContainers.value(row);
}


QmVocContainer * QmVocContainer::childContainer(const QString & name)
{
    foreach (QmVocContainer *container, d->m_childContainers) {
        if (container->name() == name) {
            return container;
        }
    }
    return 0;
}


void QmVocContainer::deleteChildContainer(int row)
{
    qDebug() << "Delete of container - check entry deletion!";
    delete d->m_childContainers.takeAt(row);

    invalidateChildLessonEntries();
}

void QmVocContainer::removeChildContainer(int row)
{
    d->m_childContainers.removeAt(row);
    invalidateChildLessonEntries();
}


int QmVocContainer::childContainerCount() const
{
    return d->m_childContainers.count();
}

int QmVocContainer::row() const
{
    if (d->m_parentContainer) {
        return d->m_parentContainer->d->m_childContainers.indexOf(const_cast<QmVocContainer*>(this));
    }
    return 0;
}


QmVocContainer& QmVocContainer::operator= ( const QmVocContainer &other )
{
    d->m_name = other.d->m_name;
    d->m_inPractice = other.d->m_inPractice;
    return *this;
}

bool QmVocContainer::operator==(const QmVocContainer &other)
{
    return  d->m_name == other.d->m_name &&
            d->m_inPractice == other.d->m_inPractice
/// @todo make this return something useful
            ;
}

void QmVocContainer::setName( const QString &name )
{
    d->m_name = name;
}

QString QmVocContainer::name()
{
    return d->m_name;
}

bool QmVocContainer::inPractice()
{
    return d->m_inPractice;
}

void QmVocContainer::setInPractice(bool inPractice)
{
    d->m_inPractice = inPractice;
}

void QmVocContainer::removeTranslation(int translation)
{
    foreach(QmVocContainer *childContainer, d->m_childContainers) {
        childContainer->removeTranslation(translation);
    }

    foreach(QmVocExpression *entry, entries() ) {
        entry->removeTranslation( translation );
    }
}

QList< QmVocExpression * > QmVocContainer::entriesRecursive()
{
//     kDebug() << "entriesRecursive: " << name();
    if (!d->m_childLessonEntriesValid) {
        updateChildLessonEntries();
    }
    return d->m_childLessonEntries;
}

QList< QmVocContainer * > QmVocContainer::childContainers()
{
    return d->m_childContainers;
}

QmVocContainer * QmVocContainer::parent()
{
    return d->m_parentContainer;
}

void QmVocContainer::setContainerType(QmVocContainer::EnumContainerType type)
{
    d->m_type = type;
}

QmVocContainer::EnumContainerType QmVocContainer::containerType()
{
    return d->m_type;
}


QUrl QmVocContainer::imageUrl()
{
    return d->m_imageUrl;
}

void QmVocContainer::setImageUrl(const QUrl &url)
{
    d->m_imageUrl = url;
}

void QmVocContainer::insertChildContainer(int row, QmVocContainer * child)
{
    d->m_childContainers.insert(row, child);
    child->d->m_parentContainer = this;

    invalidateChildLessonEntries();
}

void QmVocContainer::updateChildLessonEntries()
{
    QList < QmVocExpression* > entriesRecursive = entries();

    foreach(QmVocContainer *childContainer, d->m_childContainers)
        foreach(QmVocExpression * expr, childContainer->entries(Recursive))
            entriesRecursive.append(expr);

    d->m_childLessonEntries = entriesRecursive;
    d->m_childLessonEntriesValid = true;
}

void QmVocContainer::invalidateChildLessonEntries()
{
    d->m_childLessonEntriesValid = false;
    // propagate to parent
    if (d->m_parentContainer) {
        d->m_parentContainer->invalidateChildLessonEntries();
    }
}

double QmVocContainer::averageGrade(int translation, EnumEntriesRecursive recursive)
{
    // grades range from 0..7 right now
    int sum = 0;
    foreach (QmVocExpression *entry, entries(recursive)) {
        sum += entry->translation(translation)->grade();
    }
    // make that a percentage
    return (sum * 100.0/7.0)/entryCount(recursive);
}

int QmVocContainer::expressionsOfGrade(int translation, grade_t grade, EnumEntriesRecursive recursive)
{
    int sum = 0;
    foreach (QmVocExpression *entry, entries(recursive)) {
        if (entry->translation(translation)->grade() == grade) {
            sum++;
        }
    }
    return sum;
}

void QmVocContainer::resetGrades(int translation, EnumEntriesRecursive recursive)
{
    foreach (QmVocExpression *entry, entries(recursive)) {
        entry->resetGrades(translation);
    }
}

