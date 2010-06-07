/***************************************************************************
*   this file is from kdeedu project. Filename: 
***************************************************************************/

/***************************************************************************
                     read a QmVocDocument from a KVTML file
    -----------------------------------------------------------------------
    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005 Eric Pignet <eric at erixpage.com>
                     (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                     (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

#ifndef QTVTVOCKVTMLREADER_H
#define QTVTVOCKVTMLREADER_H

#include <QtXml/QDomDocument>
#include <QtCore/QList>


#include "qtvtvocexpression.h"
#include "qtvtvocarticle.h"
#include "qtvtvocmultiplechoice.h"
#include "qtvtvockvtmlcompability.h"
#include "qtvtvocpersonalpronoun.h"

class QIODevice;
class QmVocDocument;

/**
@author Eric Pignet
*/
class QmVocKvtmlReader : public QObject
{
    Q_OBJECT
public:
    QmVocKvtmlReader( QIODevice *file );

    bool readDoc( QmVocDocument *doc );

    /**
     * Attempt to add a language/locale. Language/locale are set to the same value.
     * No error if already there with the same value.
     * @param languageId idenifier number
     * @param language name
     * @return true if successful
     */
    bool addLanguage( int languageId, const QString& language);

    bool readLesson( QDomElement &domElementParent );
    bool readArticle( QDomElement &domElementParent );
    bool readPersonalPronouns( QDomElement &domElementParent, QmVocPersonalPronoun& pronouns );
    bool readConjugation( QDomElement &domElementParent, QmVocConjugation &conjugation );
    bool readTranslationConjugations( QDomElement &domElementParent, QmVocTranslation* translation );
    bool readType( QDomElement &domElementParent );
    bool readTense( QDomElement &domElementParent );
    bool readUsage( QDomElement &domElementParent );
    bool readComparison( QDomElement &domElementParent, QmVocTranslation* translation );
    bool readMultipleChoice( QDomElement &domElementParent, QmVocTranslation* translation );
    bool readExpressionChildAttributes( QDomElement &domElementExpressionChild,
                                        QString &lang,
                                        grade_t &grade, grade_t &rev_grade,
                                        int &count, int &rev_count,
                                        QDateTime &date, QDateTime &rev_date,
                                        QString &remark,
                                        int &bcount, int &rev_bcount,
                                        QString &query_id,
                                        QString &pronunciation,
                                        int &width,
                                        QString &type,
                                        QString &faux_ami_f,
                                        QString &faux_ami_t,
                                        QString &synonym,
                                        QString &example,
                                        QString &antonym,
                                        QSet<QString> &usage,
                                        QString &paraphrase );
    bool readExpression( QDomElement &domElementParent );
    bool readBody( QDomElement &domElementParent );

    QString errorMessage() const
    {
        return m_errorMessage;
    }

private:
    QIODevice *m_inputFile;
    QmVocDocument *m_doc;
    QString m_errorMessage;
    int m_cols;
    int m_lines;
    QStringList m_oldSelfDefinedTypes;

    QmVocKvtmlCompability m_compability;
};

#endif
