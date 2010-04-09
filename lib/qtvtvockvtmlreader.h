/***************************************************************************
*   this file is from kdeedu project. Filename: 
***************************************************************************/

/***************************************************************************
                     read a QTvtVocDocument from a KVTML file
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
class QTvtVocDocument;

/**
@author Eric Pignet
*/
class QTvtVocKvtmlReader : public QObject
{
    Q_OBJECT
public:
    QTvtVocKvtmlReader( QIODevice *file );

    bool readDoc( QTvtVocDocument *doc );

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
    bool readPersonalPronouns( QDomElement &domElementParent, QTvtVocPersonalPronoun& pronouns );
    bool readConjugation( QDomElement &domElementParent, QTvtVocConjugation &conjugation );
    bool readTranslationConjugations( QDomElement &domElementParent, QTvtVocTranslation* translation );
    bool readType( QDomElement &domElementParent );
    bool readTense( QDomElement &domElementParent );
    bool readUsage( QDomElement &domElementParent );
    bool readComparison( QDomElement &domElementParent, QTvtVocTranslation* translation );
    bool readMultipleChoice( QDomElement &domElementParent, QTvtVocTranslation* translation );
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
    QTvtVocDocument *m_doc;
    QString m_errorMessage;
    int m_cols;
    int m_lines;
    QStringList m_oldSelfDefinedTypes;

    QTvtVocKvtmlCompability m_compability;
};

#endif
