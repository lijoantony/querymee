/***************************************************************************
*   this file is from kdeedu project. Filename: keduvockvtml2writer.h
***************************************************************************/

/***************************************************************************
                     export a QTvtVocDocument to a KVTML file
    -----------------------------------------------------------------------
    copyright       : (C) 2007 Jeremy Whiting <jpwhiting@kde.org>
                      (C) 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QTVTVOCKVTML2WRITER_H
#define QTVTVOCKVTML2WRITER_H

#include <QtXml/QDomDocument>
#include <QtCore/QList>
#include <QtCore/QFile>

#include "qtvtvocarticle.h"
#include "qtvtvocmultiplechoice.h"
#include "qtvtvoctranslation.h"
#include "qtvtvocpersonalpronoun.h"

class QTvtVocDocument;
class QTvtVocExpression;
class QTvtVocLesson;
class QTvtVocLeitnerBox;

/**
* @brief Class to write kvtml2 data files from QTvtVocDocument
* @author Jeremy Whiting
*/
class QTvtVocKvtml2Writer
{
public:
    QTvtVocKvtml2Writer( QFile *file );

    bool writeDoc( QTvtVocDocument *doc, const QString &generator );

    QByteArray toByteArray( QTvtVocDocument *doc, const QString &generator );

    /**
     * Helper function, appends a new element AND a text child to @p parent
     * Only appends if @p text is NOT empty.
     * @param parent
     * @param elementName
     * @param text
     */
    static void appendTextElement( QDomElement &parent, const QString &elementName, const QString &text );

private:

    bool createXmlDocument( QTvtVocDocument *doc, const QString &generator );

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

    bool writePersonalPronoun( QDomElement &pronounElement, const QTvtVocPersonalPronoun &pronoun);
    /** write types
     * @param typesElement QDomElement types to write to
     */
    bool writeWordTypes( QDomElement &typesElement, QTvtVocWordType* parentContainer );

    /**
     * write the leitner boxes @see QTvtVocLeitnerBox
     * @param leitnerParentElement parent dom element
     * @param parentContainer parent of the QTvtVocLeitnerBoxes to write
     * @return success
     */
    bool writeLeitnerBoxes( QDomElement &leitnerParentElement, QTvtVocLeitnerBox* parentContainer );

    /** write entries
     * @param entriesElement QDomElement entries to write to
     */
    bool writeEntries( QDomElement &entriesElement );

    /** write a translation
     * @param translationElement QDomElement translation to write to, with id pre-set
     * @param translation object to write
     */
    bool writeTranslation( QDomElement &translationElement, QTvtVocTranslation* translation );

    /**
     * Used to write synonym, antonym and false friend lists
     * @param typesElement 
     * @param parentContainer 
     * @return 
     */
    bool writeRelated( QDomElement &parentElement, QList<QTvtVocTranslation*> relatedList );

    /** write the lesson group
     * @param parentLesson the parent lesson of the current lesson
     * @param lessonsElement QDomElement lessons to write to
     */
    bool writeLessons( QTvtVocLesson *parentLesson, QDomElement &lessonsElement );

    
    void writeSynonymAntonymFalseFriend(QDomElement & parentElement);
            
    /** write a comparison
     * @param comparisonElement QDomElement comparison to write to
     * @param comparison object to write
     * @returns success
     */
    bool writeComparison( QDomElement &comparisonElement, QTvtVocTranslation *translation );

    /** write multiple choice choices
     * @param multipleChoiceElement QDomElement multiplechoice to write to
     * @returns success
     */
    bool writeMultipleChoice( QDomElement &multipleChoiceElement, QTvtVocTranslation* translation );

    QDomElement newTextElement( const QString &elementName, const QString &text );

    QFile *m_outputFile;
    QTvtVocDocument *m_doc;

    QList<QTvtVocExpression*>  m_allEntries;
    QList<QTvtVocTranslation*> m_synonyms;
    QList<QTvtVocTranslation*> m_antonyms;
    QList<QTvtVocTranslation*> m_falseFriends;

    QDomDocument m_domDoc;
};

#endif
