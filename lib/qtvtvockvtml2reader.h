/***************************************************************************
*   this file is from kdeedu project. Filename: keduvockvtml2reader.h
***************************************************************************/

/***************************************************************************
                     read a QTvtVocDocument from a KVTML2 file
    -----------------------------------------------------------------------
    copyright      : (C) 2007 Jeremy Whiting <jpwhiting@kde.org>
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

#ifndef KEDUVOCKVTML2READER_H
#define KEDUVOCKVTML2READER_H

#include <QtXml/QDomDocument>
#include <QList>
#include <QFile>

#include "qtvtvocexpression.h"
#include "qtvtvocpersonalpronoun.h"
#include "qtvtvocarticle.h"
#include "qtvtvocmultiplechoice.h"

class QIODevice;
class QTvtVocDocument;
class QTvtVocWordType;

/**
* @brief class to read kvtml2 data files into keduvocdocument
* @author Jeremy Whiting
*/
class QTvtVocKvtml2Reader : public QObject
{
    Q_OBJECT
public:
    /** default constructor
     * @param file file to read from
     */
    QTvtVocKvtml2Reader( QFile *file );

    /** read the document
     * @param doc document object to store the data in
     */
    bool readDoc( QTvtVocDocument *doc );

    /** get the errormessage string
     * @returns the errormessage string
     */
    QString errorMessage() const
    {
        return m_errorMessage;
    }

private:
    /** read information entries
     * @param informationElement QDomElement information
     */
    bool readInformation( QDomElement &informationElement );

    /** read group elements: identifiers, entries, types, usages, lessons */
    bool readGroups( QDomElement &domElementParent );

    /** read an identifier
     * @param identifierElement QDomElement for the identifier to read
     */
    bool readIdentifier( QDomElement &identifierElement );

    /** read an identifiers articles
     * @param articleElement QDomElement for the article group
     * @param identifierNum number of the identifier this article is inside of
     */
    bool readArticle( QDomElement &articleElement, int identifierNum );

    bool readPersonalPronoun( QDomElement &conjugElement, QTvtVocPersonalPronoun &pronoun );

    bool readPersonalPronounChild(QDomElement & personElement, QTvtVocPersonalPronoun &pronoun, QTvtVocWordFlags flags);

    /** read the types
     * @param typesElement QDomElement for the types group
     */
    bool readWordType( QTvtVocWordType* parentContainer, QDomElement &typesElement );

    /**
     * Read a leitner box container.
     * This is a grading system where the vocabulary are kept in boxes and promoted/demoted during the learning.
     * Be aware that leitner boxes are a list only and no sub boxes will ever be read or written.
     * While reusing the lesson class is quite easy for this a proper subclass of QTvtVocContainer would be the better solution.
     * @param parentContainer the parent to append the new leitner container to
     * @param leitnerElement the element in the dom
     * @return success
     */
    bool readLeitner( QTvtVocLeitnerBox* parentContainer, QDomElement &leitnerElement );

    /**
     * Read all <container> tags within a word type definition.
     * @param parentContainer
     * @param lessonElement
     * @return
     */
    bool readChildWordTypes( QTvtVocWordType* parentContainer, QDomElement &lessonElement );

    /** read the tenses
     * @param tensesElement QDomElement for the tenses group
     */
    QStringList readTenses( QDomElement &tensesElement );

    /** read the usages
     * @param usagesElement QDomElement for the usages group
     */
    bool readUsages( QDomElement &usagesElement );

    /** read an entry
     * @param entryElement QDomElement for the entry to read
     */
    bool readEntry( QDomElement &entryElement );

    /** read a translation
     * @param translationElement QDomElement for the translation to read
     */
    bool readTranslation( QDomElement &translationElement, QTvtVocExpression *expr, int index );

    /** read a comparison
     * @param comparisonElement comparison group element
     * @param comp comparison object to read into
     */
    bool readComparison( QDomElement &comparisonElement, QTvtVocTranslation *translation );

    /** read a multiple choice group
     * @param multipleChoiceElement element to read from
     * @param mc QTvtVocMultipleChoice object to read to
     */
    bool readMultipleChoice( QDomElement &multipleChoiceElement, QTvtVocTranslation* translation );

    /**
     * Read <lesson> tags.
     * @param parentLesson
     * @param lessonElement
     * @return
     */
    bool readChildLessons( QTvtVocLesson* parentLesson, QDomElement &lessonElement );

    /** read a lesson, and append it to the document
     * @param lessonElement element to read from
     */
    bool readLesson( QTvtVocLesson* parentLesson, QDomElement &lessonElement );

    bool readSynonymsAntonymsFalseFriends( QDomElement &rootElement );

    /** pre-opened QIODevice to read from */
    QIODevice *m_inputFile;

    /** QTvtVocDocument to read to */
    QTvtVocDocument *m_doc;

    /** because we read the entries first, we store them here temporarily.
     * later we read the lessons and put the entries there based on the key (their id) */
    QMap<int, QTvtVocExpression*> m_allEntries;

    /** error message */
    QString m_errorMessage;
};

#endif
