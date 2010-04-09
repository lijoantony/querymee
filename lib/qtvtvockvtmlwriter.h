/***************************************************************************
*   this file is from kdeedu project. Filename: keduvockvtmlwriter.h
***************************************************************************/

/***************************************************************************
                     export a QTvtVocDocument to a KVTML file
    -----------------------------------------------------------------------
    copyright       : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                      (C) 2005 Eric Pignet <eric at erixpage.com>
                      (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
                      (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QTVTVOCKVTMLWRITER_H
#define QTVTVOCKVTMLWRITER_H

#include <QtXml/QDomDocument>
#include <QtCore/QList>
#include <QtCore/QFile>

#include "qtvtvocarticle.h"
#include "qtvtvocpersonalpronoun.h"
#include "qtvtvocconjugation.h"
#include "qtvtvocmultiplechoice.h"
#include "qtvtvockvtmlcompability.h"
#include "qtvtvoctranslation.h"

class QTvtVocDocument;

/**
@author Eric Pignet
*/
class QTvtVocKvtmlWriter
{
public:
    QTvtVocKvtmlWriter( QFile *file );

    bool writeDoc( QTvtVocDocument *doc, const QString &generator );

private:
    bool writeLesson( QDomElement &domElementParent );
    bool writeType( QDomElement &domElementParent );
    bool writeTense( QDomElement &domElementParent );
    bool writeUsage( QDomElement &domElementParent );
    bool writeOption( QDomElement &domElementParent );
    bool writeArticle( QDomElement &domElementParent );

    bool writePersonalPronouns( QDomElement &domElementParent, QList<QTvtVocPersonalPronoun> &curr_conjug );
    bool writePersonalPronounChild( QDomElement &domElementParent, const QTvtVocPersonalPronoun &curr_conjug );

    bool writeConjugHeader( QDomElement &domElementParent, QList<QTvtVocConjugation> &curr_conjug );
    bool writeConjug( QDomElement &domElementParent, const QTvtVocConjugation &curr_conjug, const QString &type );
    bool writeConjugEntry( QDomElement &domElementParent, QTvtVocTranslation &translation  );
    bool writeComparison( QDomElement &domElementParent, QTvtVocTranslation* translation );
    bool writeMultipleChoice( QDomElement &domElementParent, const QTvtVocMultipleChoice &mc );

    QDomElement newTextElement( const QString &elementName, const QString &text );

    QFile *m_outputFile;
    QTvtVocDocument *m_doc;

    QDomDocument m_domDoc;

    QTvtVocKvtmlCompability m_compability;
};

#endif
