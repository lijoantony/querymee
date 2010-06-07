/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocwordtype.h
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

#ifndef QTVTVOCWORDTYPE_H
#define QTVTVOCWORDTYPE_H

#include "libqtvtvocdocument_export.h"

#include "qtvtvoccontainer.h"

#include "qtvtvocwordflags.h"

#include <QtCore/QList>
#include <QtCore/QString>

class QmVocExpression;
class QmVocTranslation;

/** class to store translation word types */
class QTVTVOCDOCUMENT_EXPORT QmVocWordType :public QmVocContainer
{
public:


    /** default constructor */
    explicit QmVocWordType(const QString& name, QmVocWordType *parent = 0);

    /** destructor */
    ~QmVocWordType();

    /** assignment operator */
//     QmVocWordType& operator= ( const QmVocWordType& );

    /**
     * Internally (different from the name) the class can have one of the preset word types. These are used to determine special properties (verbs have conjugations available for example).
     * @param type
     */
    void setWordType(QmVocWordFlags flags);

    /**
     * Return the raw WordTypeFlags. Returns NoInformation if no flags are set.
     * @return WordTypeFlags
     */
     QmVocWordFlags wordType() const;

    /**
     * Return a child class (or this class) that is of the specified type. Returns 0 if no class of that type is found.
     * @param type
     * @return
     */
    QmVocWordType* childOfType(const QmVocWordFlags& flags);

    /**
     * The word type class does keep track of individual translations, because for one entry, the translations can have different word types (eg. genders of nouns tend to be different in different langues).
     * @param row
     * @return
     */
    QmVocTranslation * translation(int row);

    /**
     * get a list of all entries in the lesson
     * @param recursive include entries in sublessons
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
