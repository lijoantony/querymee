/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoclesson.h
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

#ifndef QTVTVOCLESSON_H
#define QTVTVOCLESSON_H

#include "libqtvtvocdocument_export.h"

#include "qtvtvoccontainer.h"

class QTvtVocExpression;

/** class to store information about a lesson */
class QTVTVOCDOCUMENT_EXPORT QTvtVocLesson :public QTvtVocContainer
{
public:
    /** default constructor */
    explicit QTvtVocLesson(const QString& name, QTvtVocContainer *parent = 0);

//     void appendChildLesson(QTvtVocLesson *child);

//     QList<QTvtVocLesson *> childLessons();
//     QTvtVocLesson *childLesson(int row);

//     int childLessonCount() const;

//     int row() const;
//     QTvtVocLesson *parent();

    /** copy constructor for d-pointer safe copying */
    QTvtVocLesson( const QTvtVocLesson &other );

    /** destructor */
    ~QTvtVocLesson();

    /** assignment operator */
    QTvtVocLesson& operator= ( const QTvtVocLesson& );

    QTvtVocExpression* entry(int row, EnumEntriesRecursive recursive = NotRecursive);

    /** get a list of all entries in the lesson */
    QList < QTvtVocExpression* > entries(EnumEntriesRecursive recursive = NotRecursive);

    /** get the number of entries in the lesson */
    int entryCount(EnumEntriesRecursive recursive = NotRecursive);

    /** append an entry to the lesson
     * @param entryid id of the entry to add
     */
    void appendEntry(QTvtVocExpression* entry);

    /**
     * insert an entry at a specific position
     * @param index 
     * @param entry 
     */
    void insertEntry(int index, QTvtVocExpression* entry);

    /** remove an entry from the lesson
     * @param entryid id of the entry to remove
     */
    void removeEntry(QTvtVocExpression* entry);

private:
    class Private;
    Private * const d;
};

#endif
