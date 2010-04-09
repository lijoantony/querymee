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

#include "keduvoccontainer.h"

#include "keduvocexpression.h"

// #include <KDebug>
#include <QList>
#include <QtDebug>

/** private class to store information about a lesson */
class QTvtVocContainer::Private
{
public:
    ~Private();

    // properties for this lesson
    QString m_name;
    bool m_inPractice;

    // other lessons in the tree
    QTvtVocContainer *m_parentContainer;
    QList < QTvtVocContainer * > m_childContainers;

    EnumContainerType m_type;

    QList < QTvtVocExpression* > m_childLessonEntries;
    bool m_childLessonEntriesValid;

    /// Image url
    QUrl m_imageUrl;
};

QTvtVocContainer::Private::~ Private()
{
    qDeleteAll(m_childContainers);
}

QTvtVocContainer::QTvtVocContainer(const QString& name, EnumContainerType type, QTvtVocContainer *parent)
        : d( new Private )
{
    d->m_parentContainer = parent;
    d->m_name = name;
    d->m_inPractice = true;
    d->m_type = type;
    d->m_childLessonEntriesValid = false;
}

QTvtVocContainer::QTvtVocContainer( const QTvtVocContainer &other )
        : d( new Private )
{
    d->m_name = other.d->m_name;
    d->m_inPractice = other.d->m_inPractice;
    d->m_type = other.d->m_type;
    d->m_parentContainer = other.d->m_parentContainer;
    d->m_childLessonEntriesValid = false;
}

QTvtVocContainer::~QTvtVocContainer()
{
    delete d;
}

void QTvtVocContainer::appendChildContainer(QTvtVocContainer * child)
{
    d->m_childContainers.append(child);
    child->d->m_parentContainer = this;

    invalidateChildLessonEntries();
}

QTvtVocContainer * QTvtVocContainer::childContainer(int row)
{
    return d->m_childContainers.value(row);
}


QTvtVocContainer * QTvtVocContainer::childContainer(const QString & name)
{
    foreach (QTvtVocContainer *container, d->m_childContainers) {
        if (container->name() == name) {
            return container;
        }
    }
    return 0;
}


void QTvtVocContainer::deleteChildContainer(int row)
{
    qDebug() << "Delete of container - check entry deletion!";
    delete d->m_childContainers.takeAt(row);

    invalidateChildLessonEntries();
}

void QTvtVocContainer::removeChildContainer(int row)
{
    d->m_childContainers.removeAt(row);
    invalidateChildLessonEntries();
}


int QTvtVocContainer::childContainerCount() const
{
    return d->m_childContainers.count();
}

int QTvtVocContainer::row() const
{
    if (d->m_parentContainer) {
        return d->m_parentContainer->d->m_childContainers.indexOf(const_cast<QTvtVocContainer*>(this));
    }
    return 0;
}


QTvtVocContainer& QTvtVocContainer::operator= ( const QTvtVocContainer &other )
{
    d->m_name = other.d->m_name;
    d->m_inPractice = other.d->m_inPractice;
    return *this;
}

bool QTvtVocContainer::operator==(const QTvtVocContainer &other)
{
    return  d->m_name == other.d->m_name &&
            d->m_inPractice == other.d->m_inPractice
/// @todo make this return something useful
            ;
}

void QTvtVocContainer::setName( const QString &name )
{
    d->m_name = name;
}

QString QTvtVocContainer::name()
{
    return d->m_name;
}

bool QTvtVocContainer::inPractice()
{
    return d->m_inPractice;
}

void QTvtVocContainer::setInPractice(bool inPractice)
{
    d->m_inPractice = inPractice;
}

void QTvtVocContainer::removeTranslation(int translation)
{
    foreach(QTvtVocContainer *childContainer, d->m_childContainers) {
        childContainer->removeTranslation(translation);
    }

    foreach(QTvtVocExpression *entry, entries() ) {
        entry->removeTranslation( translation );
    }
}

QList< QTvtVocExpression * > QTvtVocContainer::entriesRecursive()
{
//     kDebug() << "entriesRecursive: " << name();
    if (!d->m_childLessonEntriesValid) {
        updateChildLessonEntries();
    }
    return d->m_childLessonEntries;
}

QList< QTvtVocContainer * > QTvtVocContainer::childContainers()
{
    return d->m_childContainers;
}

QTvtVocContainer * QTvtVocContainer::parent()
{
    return d->m_parentContainer;
}

void QTvtVocContainer::setContainerType(QTvtVocContainer::EnumContainerType type)
{
    d->m_type = type;
}

QTvtVocContainer::EnumContainerType QTvtVocContainer::containerType()
{
    return d->m_type;
}


QUrl QTvtVocContainer::imageUrl()
{
    return d->m_imageUrl;
}

void QTvtVocContainer::setImageUrl(const QUrl &url)
{
    d->m_imageUrl = url;
}

void QTvtVocContainer::insertChildContainer(int row, QTvtVocContainer * child)
{
    d->m_childContainers.insert(row, child);
    child->d->m_parentContainer = this;

    invalidateChildLessonEntries();
}

void QTvtVocContainer::updateChildLessonEntries()
{
    QList < QTvtVocExpression* > entriesRecursive = entries();

    foreach(QTvtVocContainer *childContainer, d->m_childContainers)
        foreach(QTvtVocExpression * expr, childContainer->entries(Recursive))
            entriesRecursive.append(expr);

    d->m_childLessonEntries = entriesRecursive;
    d->m_childLessonEntriesValid = true;
}

void QTvtVocContainer::invalidateChildLessonEntries()
{
    d->m_childLessonEntriesValid = false;
    // propagate to parent
    if (d->m_parentContainer) {
        d->m_parentContainer->invalidateChildLessonEntries();
    }
}

double QTvtVocContainer::averageGrade(int translation, EnumEntriesRecursive recursive)
{
    // grades range from 0..7 right now
    int sum = 0;
    foreach (QTvtVocExpression *entry, entries(recursive)) {
        sum += entry->translation(translation)->grade();
    }
    // make that a percentage
    return (sum * 100.0/7.0)/entryCount(recursive);
}

int QTvtVocContainer::expressionsOfGrade(int translation, grade_t grade, EnumEntriesRecursive recursive)
{
    int sum = 0;
    foreach (QTvtVocExpression *entry, entries(recursive)) {
        if (entry->translation(translation)->grade() == grade) {
            sum++;
        }
    }
    return sum;
}

void QTvtVocContainer::resetGrades(int translation, EnumEntriesRecursive recursive)
{
    foreach (QTvtVocExpression *entry, entries(recursive)) {
        entry->resetGrades(translation);
    }
}

