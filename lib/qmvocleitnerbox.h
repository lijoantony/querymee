/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocleitnerbox.h
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

#ifndef QMVOCLEITNERBOX_H
#define QMVOCLEITNERBOX_H

#include "libqmvocdocument_export.h"

#include "qmvoccontainer.h"

#include <QtCore/QList>
#include <QtCore/QString>

class QmVocExpression;
class QmVocTranslation;

/**
 * Leitner Boxes are an alternative grading system.
 * Classically flash cards are kept in boxes and moved corresponding to the users knowledge level.
 */
class QTVTVOCDOCUMENT_EXPORT QmVocLeitnerBox :public QmVocContainer
{
public:
    /** default constructor */
    explicit QmVocLeitnerBox(const QString& name, QmVocLeitnerBox *parent = 0);

    /** destructor */
    ~QmVocLeitnerBox();

    /**
     * The leitner box class keeps track of individual translations, because for one entry, the translations can have different grades.
     * @param row 
     * @return 
     */
    QmVocTranslation * translation(int row);

    /**
     * get a list of all entries in the box
     * @return 
     */
    QList < QmVocExpression* > entries(EnumEntriesRecursive recursive = NotRecursive);

    QmVocExpression* entry(int row, EnumEntriesRecursive recursive = NotRecursive);

    /** get the number of entries in the lesson */
    int entryCount(EnumEntriesRecursive recursive = NotRecursive);

private:
    class Private;
    Private * const d;

    /** add an entry to the lesson
     * @param entryid id of the entry to add
     */
    void addTranslation(QmVocTranslation* translation);

    /** remove an entry from the lesson
     * @param entryid id of the entry to remove
     */
    void removeTranslation(QmVocTranslation* translation);

    friend class QmVocTranslation;
};

#endif
