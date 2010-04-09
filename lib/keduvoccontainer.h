/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoccontainer.h
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

#ifndef KEDUVOCCONTAINER_H
#define KEDUVOCCONTAINER_H

#include "libkeduvocdocument_export.h"

#include "keduvoctext.h"

// #include <KDE/KUrl>
#include <QUrl>
#include <QtCore/QList>

class QTvtVocExpression;

/** class to store information about a container - that can be a lesson or word types */
class QTVTVOCDOCUMENT_EXPORT QTvtVocContainer
{
    // make this a template?

public:
    enum EnumContainerType{
        Container,
        Lesson,
        WordType,
        Leitner
    };

    enum EnumEntriesRecursive{
        NotRecursive = 0,
        Recursive = 1
    };

    /** default constructor */
    explicit QTvtVocContainer(const QString& name, EnumContainerType type, QTvtVocContainer *parent = 0);

    void appendChildContainer(QTvtVocContainer *child);
    void insertChildContainer(int row, QTvtVocContainer *child);
    void deleteChildContainer(int row);
    void removeChildContainer(int row);
    QTvtVocContainer *childContainer(int row);

    /**
     * Retrieve a child container by its name
     * Returns 0 if no container is found
     * @param name container name
     * @return the child container
     */
    QTvtVocContainer *childContainer(const QString& name);

    QList<QTvtVocContainer *> childContainers();

    /**
     * Find a child container
     * @param name
     * @return
     */
//     QTvtVocContainer *childContainer(const QString& name);

    int childContainerCount() const;

    int row() const;
    virtual QTvtVocContainer *parent();

    /** copy constructor for d-pointer safe copying */
    QTvtVocContainer( const QTvtVocContainer &other );

    /** destructor */
    virtual ~QTvtVocContainer();

    /** assignment operator */
    QTvtVocContainer& operator= ( const QTvtVocContainer& );

    /** set the container name
     * @param name text to set for the name
     */
    void setName( const QString &name );

    /** get the container name */
    QString name();

    /** get a list of all entries in the container */
    virtual QList < QTvtVocExpression* > entries(EnumEntriesRecursive recursive = NotRecursive) =0;
    virtual int entryCount(EnumEntriesRecursive recursive = NotRecursive) =0;
    virtual QTvtVocExpression* entry(int row, EnumEntriesRecursive recursive = NotRecursive) =0;

    /**
     * Removes a translation. This has to be called when a language is removed from a document.
     * @param translation
     */
    void removeTranslation(int translation);

    bool inPractice();
    void setInPractice( bool inPractice );

    /** equality operator */
    bool operator==(const QTvtVocContainer &other);

    /**
     * The type of this container. @see EnumContainerType
     * @return
     */
    QTvtVocContainer::EnumContainerType containerType();

    /**
     * Set the type of container.
     * For convenience by default this is taken over from the parent, so no need to set.
     * @param type the new type
     */
    void setContainerType(QTvtVocContainer::EnumContainerType type);


    /** get the image url for this container if it exists */
    QUrl imageUrl();

    /** set the image url for this container
     * @param url               url of the image
     */
    void setImageUrl(const QUrl &url);

    double averageGrade(int translation, EnumEntriesRecursive recursive);

    int expressionsOfGrade(int translation, grade_t grade, EnumEntriesRecursive recursive);

    /**
     * Remove grades from all entries of this lessons
     * @param translation which translation to remove. -1 for all.
     * @param recursive whether to include child lessons
     */
    void resetGrades(int translation, EnumEntriesRecursive recursive);

protected:
    QList< QTvtVocExpression * > entriesRecursive();

    /**
     * Set the child entry cache to invalid
     */
    void invalidateChildLessonEntries();

    /**
     * Recreate the cache of entries in this lesson's child lessons.
     */
    void updateChildLessonEntries();

private:
    class Private;
    Private * const d;
};

#endif
