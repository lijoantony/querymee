/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocwordtype.h
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

#ifndef KEDUVOCWORDTYPE_H
#define KEDUVOCWORDTYPE_H

#include "libqtvtvocdocument_export.h"

#include "qtvtvoccontainer.h"

#include "qtvtvocwordflags.h"

#include <QtCore/QList>
#include <QtCore/QString>

class QTvtVocExpression;
class QTvtVocTranslation;

/** class to store translation word types */
class QTVTVOCDOCUMENT_EXPORT QTvtVocWordType :public QTvtVocContainer
{
public:


    /** default constructor */
    explicit QTvtVocWordType(const QString& name, QTvtVocWordType *parent = 0);

    /** destructor */
    ~QTvtVocWordType();

    /** assignment operator */
//     QTvtVocWordType& operator= ( const QTvtVocWordType& );

    /**
     * Internally (different from the name) the class can have one of the preset word types. These are used to determine special properties (verbs have conjugations available for example).
     * @param type
     */
    void setWordType(QTvtVocWordFlags flags);

    /**
     * Return the raw WordTypeFlags. Returns NoInformation if no flags are set.
     * @return WordTypeFlags
     */
     QTvtVocWordFlags wordType() const;

    /**
     * Return a child class (or this class) that is of the specified type. Returns 0 if no class of that type is found.
     * @param type
     * @return
     */
    QTvtVocWordType* childOfType(const QTvtVocWordFlags& flags);

    /**
     * The word type class does keep track of individual translations, because for one entry, the translations can have different word types (eg. genders of nouns tend to be different in different langues).
     * @param row
     * @return
     */
    QTvtVocTranslation * translation(int row);

    /**
     * get a list of all entries in the lesson
     * @param recursive include entries in sublessons
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
