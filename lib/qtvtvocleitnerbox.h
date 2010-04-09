/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocleitnerbox.h
***************************************************************************/

/***************************************************************************
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QTVTVOCLEITNERBOX_H
#define QTVTVOCLEITNERBOX_H

#include "libqtvtvocdocument_export.h"

#include "qtvtvoccontainer.h"

#include <QtCore/QList>
#include <QtCore/QString>

class QTvtVocExpression;
class QTvtVocTranslation;

/**
 * Leitner Boxes are an alternative grading system.
 * Classically flash cards are kept in boxes and moved corresponding to the users knowledge level.
 */
class QTVTVOCDOCUMENT_EXPORT QTvtVocLeitnerBox :public QTvtVocContainer
{
public:
    /** default constructor */
    explicit QTvtVocLeitnerBox(const QString& name, QTvtVocLeitnerBox *parent = 0);

    /** destructor */
    ~QTvtVocLeitnerBox();

    /**
     * The leitner box class keeps track of individual translations, because for one entry, the translations can have different grades.
     * @param row 
     * @return 
     */
    QTvtVocTranslation * translation(int row);

    /**
     * get a list of all entries in the box
     * @return 
     */
    QList < QTvtVocExpression* > entries(EnumEntriesRecursive recursive = NotRecursive);

    QTvtVocExpression* entry(int row, EnumEntriesRecursive recursive = NotRecursive);

    /** get the number of entries in the lesson */
    int entryCount(EnumEntriesRecursive recursive = NotRecursive);

private:
    class Private;
    Private * const d;

    /** add an entry to the lesson
     * @param entryid id of the entry to add
     */
    void addTranslation(QTvtVocTranslation* translation);

    /** remove an entry from the lesson
     * @param entryid id of the entry to remove
     */
    void removeTranslation(QTvtVocTranslation* translation);

    friend class QTvtVocTranslation;
};

#endif
