/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoclesson.h
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

#ifndef QMVOCLESSON_H
#define QMVOCLESSON_H

#include "libqtvtvocdocument_export.h"

#include "qtvtvoccontainer.h"

class QmVocExpression;

/** class to store information about a lesson */
class QTVTVOCDOCUMENT_EXPORT QmVocLesson :public QmVocContainer
{
public:
    /** default constructor */
    explicit QmVocLesson(const QString& name, QmVocContainer *parent = 0);

//     void appendChildLesson(QmVocLesson *child);

//     QList<QmVocLesson *> childLessons();
//     QmVocLesson *childLesson(int row);

//     int childLessonCount() const;

//     int row() const;
//     QmVocLesson *parent();

    /** copy constructor for d-pointer safe copying */
    QmVocLesson( const QmVocLesson &other );

    /** destructor */
    ~QmVocLesson();

    /** assignment operator */
    QmVocLesson& operator= ( const QmVocLesson& );

    QmVocExpression* entry(int row, EnumEntriesRecursive recursive = NotRecursive);

    /** get a list of all entries in the lesson */
    QList < QmVocExpression* > entries(EnumEntriesRecursive recursive = NotRecursive);

    /** get the number of entries in the lesson */
    int entryCount(EnumEntriesRecursive recursive = NotRecursive);

    /** append an entry to the lesson
     * @param entryid id of the entry to add
     */
    void appendEntry(QmVocExpression* entry);

    /**
     * insert an entry at a specific position
     * @param index 
     * @param entry 
     */
    void insertEntry(int index, QmVocExpression* entry);

    /** remove an entry from the lesson
     * @param entryid id of the entry to remove
     */
    void removeEntry(QmVocExpression* entry);

private:
    class Private;
    Private * const d;
};

#endif
