/***************************************************************************
*   this file is from kdeedu project. Filename: keduvockvtml2writer.h
***************************************************************************/

/***************************************************************************
                     export a QmVocDocument to a KVTML file
    -----------------------------------------------------------------------
    copyright       : (C) 2007 Jeremy Whiting <jpwhiting@kde.org>
                      (C) 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
                      (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QMVOCKVTML2WRITER_H
#define QMVOCKVTML2WRITER_H

#include <QtXml/QDomDocument>
#include <QtCore/QList>
#include <QtCore/QFile>

#include "qmvocarticle.h"
#include "qmvocmultiplechoice.h"
#include "qmvoctranslation.h"
#include "qmvocpersonalpronoun.h"

class QmVocDocument;
class QmVocExpression;
class QmVocLesson;
class QmVocLeitnerBox;

/**
* @brief Class to write kvtml2 data files from QmVocDocument
* @author Jeremy Whiting
*/
class QmVocKvtml2Writer
{
public:
    QmVocKvtml2Writer( QFile *file );

    bool writeDoc( QmVocDocument *doc, const QString &generator );

    QByteArray toByteArray( QmVocDocument *doc, const QString &generator );

    /**
     * Helper function, appends a new element AND a text child to @p parent
     * Only appends if @p text is NOT empty.
     * @param parent
     * @param elementName
     * @param text
     */
    static void appendTextElement( QDomElement &parent, const QString &elementName, const QString &text );

private:

    bool createXmlDocument( QmVocDocument *doc, const QString &generator );

    /** write information entries
     * @param informationElement QDomElement information to write to
     * @param generator text describing generator
     */
    bool writeInformation( QDomElement &informationElement, const QString &generator );

    /** write identifiers group
     * @param identifiersElement QDomElement identifiers to write to
     */
    bool writeIdentifiers( QDomElement &identifiersElement );

    /** write article
     * @param articleElement QDomElement article to write to
     * @param article the article number to write
     */
    bool writeArticle( QDomElement &articleElement, int article );

    bool writePersonalPronoun( QDomElement &pronounElement, const QmVocPersonalPronoun &pronoun);
    /** write types
     * @param typesElement QDomElement types to write to
     */
    bool writeWordTypes( QDomElement &typesElement, QmVocWordType* parentContainer );

    /**
     * write the leitner boxes @see QmVocLeitnerBox
     * @param leitnerParentElement parent dom element
     * @param parentContainer parent of the QmVocLeitnerBoxes to write
     * @return success
     */
    bool writeLeitnerBoxes( QDomElement &leitnerParentElement, QmVocLeitnerBox* parentContainer );

    /** write entries
     * @param entriesElement QDomElement entries to write to
     */
    bool writeEntries( QDomElement &entriesElement );

    /** write a translation
     * @param translationElement QDomElement translation to write to, with id pre-set
     * @param translation object to write
     */
    bool writeTranslation( QDomElement &translationElement, QmVocTranslation* translation );

    /**
     * Used to write synonym, antonym and false friend lists
     * @param typesElement 
     * @param parentContainer 
     * @return 
     */
    bool writeRelated( QDomElement &parentElement, QList<QmVocTranslation*> relatedList );

    /** write the lesson group
     * @param parentLesson the parent lesson of the current lesson
     * @param lessonsElement QDomElement lessons to write to
     */
    bool writeLessons( QmVocLesson *parentLesson, QDomElement &lessonsElement );

    void writeSynonymAntonymFalseFriend(QDomElement & parentElement);

    /** write multiple choice choices
     * @param multipleChoiceElement QDomElement multiplechoice to write to
     * @returns success
     */
    bool writeMultipleChoice( QDomElement &multipleChoiceElement, QmVocTranslation* translation );

    QDomElement newTextElement( const QString &elementName, const QString &text );

    QFile *m_outputFile;
    QmVocDocument *m_doc;

    QList<QmVocExpression*>  m_allEntries;
    QList<QmVocTranslation*> m_synonyms;
    QList<QmVocTranslation*> m_antonyms;
    QList<QmVocTranslation*> m_falseFriends;

    QDomDocument m_domDoc;
};

#endif
