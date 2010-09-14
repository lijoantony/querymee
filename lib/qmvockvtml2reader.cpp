/***************************************************************************
*   this file is from kdeedu project. Filename: keduvockvtml2reader.cpp
***************************************************************************/

/***************************************************************************
                     read a QmVocDocument from a KVTML file
    -----------------------------------------------------------------------
    copyright           : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                          (C) 2005 Eric Pignet <eric at erixpage.com>
                          (C) 2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
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

#include "qmvockvtml2reader.h"

#include <QtCore/QTextStream>
#include <QtCore/QList>
// #include <QtCore/QIODevice>
#include <QDebug>
#include <QFile>

// #include <klocale.h>

#include "qmvocdocument.h"
#include "qmvoclesson.h"
#include "qmvocleitnerbox.h"
#include "qmvocwordtype.h"
#include "kvtml2defs.h"
#include "qmvockvtmlreader.h"
#include "qmvoccommon_p.h"

// #include <KDebug>

QmVocKvtml2Reader::QmVocKvtml2Reader( QFile *file )
        : m_inputFile( file )
{
    // the file must be already open
    if ( !m_inputFile->isOpen() ) {
        m_errorMessage =  "file must be opened first" ;
    }
}


bool QmVocKvtml2Reader::readDoc( QmVocDocument *doc )
{
    m_doc = doc;

    QDomDocument domDoc( "KEduVocDocument" );

    if ( !domDoc.setContent( m_inputFile, &m_errorMessage ) )
        return false;

    QDomElement domElementKvtml = domDoc.documentElement();
    if ( domElementKvtml.tagName() != KVTML_TAG ) {
        m_errorMessage =  "This is not a KDE Vocabulary document." ;
        return false;
    }

    if ( domElementKvtml.attribute( KVTML_VERSION ).toFloat() < 2.0 ) {
        // read the file with the old format

        // first reset the file to the beginning
        m_inputFile->seek( 0 );
        QmVocKvtmlReader oldFormat( m_inputFile );

        // get the return value
        bool retval = oldFormat.readDoc( doc );

        // pass the errormessage up
        m_errorMessage = oldFormat.errorMessage();
        return retval;
    }

    //-------------------------------------------------------------------------
    // Information
    //-------------------------------------------------------------------------

    QDomElement info = domElementKvtml.firstChildElement( KVTML_INFORMATION );
    if ( !info.isNull() ) {
        if ( !readInformation( info ) )
            return false;
    }

    bool result = readGroups( domElementKvtml ); // read sub-groups

    return result;
}

bool QmVocKvtml2Reader::readInformation( QDomElement &informationElement )
{
    // read the generator
    QDomElement currentElement = informationElement.firstChildElement( KVTML_GENERATOR );
    if ( !currentElement.isNull() ) {
        m_doc->setGenerator( currentElement.text() );
        // add the version if it's there
        int pos = m_doc->generator().lastIndexOf( KVD_VERS_PREFIX );
        if ( pos >= 0 ) {
            m_doc->setVersion( m_doc->generator().remove( 0, pos + 2 ) );
        }
    }

    // read the title
    currentElement = informationElement.firstChildElement( KVTML_TITLE );
    if ( !currentElement.isNull() ) {
        m_doc->setTitle( currentElement.text() );
    }

    // read the author
    currentElement = informationElement.firstChildElement( KVTML_AUTHOR );
    if ( !currentElement.isNull() ) {
        m_doc->setAuthor( currentElement.text() );
    }

    currentElement = informationElement.firstChildElement( KVTML_AUTHORCONTACT );
    if ( !currentElement.isNull() ) {
        m_doc->setAuthorContact( currentElement.text() );
    }

    // read the license
    currentElement = informationElement.firstChildElement( KVTML_LICENSE );
    if ( !currentElement.isNull() ) {
        m_doc->setLicense( currentElement.text() );
    }

    // read the comment
    currentElement = informationElement.firstChildElement( KVTML_COMMENT );
    if ( !currentElement.isNull() ) {
        m_doc->setDocumentComment( currentElement.text() );
    }

    // read the category
    currentElement = informationElement.firstChildElement( KVTML_CATEGORY );
    if ( !currentElement.isNull() ) {
        m_doc->setCategory( currentElement.text() );
    }

    return true;
}

bool QmVocKvtml2Reader::readGroups( QDomElement &domElementParent )
{
    bool result = false;

    QDomElement groupElement = domElementParent.firstChildElement( KVTML_IDENTIFIERS );

    QDomElement currentElement;

    // ensure backwards compatibility - in kde 4.1 and earlier tenses were direct properties of the document class.
    // now they are moved into the individual identifiers
    QStringList tensesCompability;
    groupElement = groupElement.firstChildElement( KVTML_TENSES );
    if ( !groupElement.isNull() ) {
        tensesCompability = readTenses( groupElement );
    }

    groupElement = domElementParent.firstChildElement( KVTML_IDENTIFIERS );
    if ( !groupElement.isNull() ) {
        QDomNodeList entryList = groupElement.elementsByTagName( KVTML_IDENTIFIER );
        if ( entryList.length() <= 0 ) {
            m_errorMessage =  "missing identifier elements from identifiers tag" ;
            return false;
        }

        for ( int i = 0; i < entryList.count(); ++i ) {
            currentElement = entryList.item( i ).toElement();
            if ( currentElement.parentNode() == groupElement ) {
                result = readIdentifier( currentElement );
                if ( !result ) {
                    return false;
                }
                if (!tensesCompability.isEmpty()) {
                    m_doc->identifier(i).setTenseList(tensesCompability);
                }
            }
        }
    }

    groupElement = domElementParent.firstChildElement( KVTML_ENTRIES );
    if ( !groupElement.isNull() ) {
        QDomNodeList entryList = groupElement.elementsByTagName( KVTML_ENTRY );
        for ( int i = 0; i < entryList.count(); ++i ) {
            currentElement = entryList.item( i ).toElement();
            if ( currentElement.parentNode() == groupElement ) {
                result = readEntry( currentElement );
                if ( !result )
                    return false;
            }
        }
    }

    readSynonymsAntonymsFalseFriends( domElementParent );

    groupElement = domElementParent.firstChildElement( KVTML_WORDTYPES );
    if ( !groupElement.isNull() ) {
        readChildWordTypes( m_doc->wordTypeContainer(), groupElement );
    }

    groupElement = domElementParent.firstChildElement( KVTML_LEITNERBOXES );
    if ( !groupElement.isNull() ) {
        readLeitner( m_doc->leitnerContainer(), groupElement );
    }

    groupElement = domElementParent.firstChildElement( KVTML_LESSONS );
    if ( !groupElement.isNull() ) {
        readChildLessons(m_doc->lesson(), groupElement);
    }

    // Additional cleanup: Put orphaned entries without a lesson into a default lesson.
    QmVocLesson *defaultLesson = new QmVocLesson("Default Lesson", m_doc->lesson());

    // now make sure we don't have any orphan entries
    foreach (QmVocExpression * entry, m_allEntries) {
        if (!entry->lesson())
        {
            defaultLesson->appendEntry(entry);
        }
    }

    if (defaultLesson->entryCount() > 0)
    {
        m_doc->lesson()->appendChildContainer(defaultLesson);
    } else {
        delete defaultLesson;
    }

    return true;
}


bool QmVocKvtml2Reader::readIdentifier( QDomElement &identifierElement )
{
    bool result = true;
    int id = identifierElement.attribute( KVTML_ID ).toInt( &result );
    if ( !result ) {
        m_errorMessage =  "identifier missing id" ;
        return false;
    }

    // generate empty identifiers in the doc
    for ( int i = m_doc->identifierCount(); i <= id; i++ ) {
        m_doc->appendIdentifier( QmVocIdentifier() );
    }

    // the first element, create the identifier, even if empty
    QDomElement currentElement = identifierElement.firstChildElement( KVTML_NAME );
    m_doc->identifier(id).setName( currentElement.text() );

    currentElement = identifierElement.firstChildElement( KVTML_LOCALE );
    m_doc->identifier(id).setLocale( currentElement.text() );

    currentElement = identifierElement.firstChildElement( KVTML_IDENTIFIERTYPE );
    if ( !currentElement.isNull() ) {
        // TODO: do something with the type
    }

    // read sub-parts
    currentElement = identifierElement.firstChildElement( KVTML_ARTICLE );
    if ( !currentElement.isNull() ) {
        readArticle( currentElement, id );
    }

    currentElement = identifierElement.firstChildElement( KVTML_PERSONALPRONOUNS );
    if ( !currentElement.isNull() ) {
        QmVocPersonalPronoun personalPronoun;
        readPersonalPronoun( currentElement, personalPronoun );
        m_doc->identifier(id).setPersonalPronouns( personalPronoun );
    }

    QStringList tenses = readTenses(identifierElement);

    m_doc->identifier(id).setTenseList(tenses);

    return result;
}

bool QmVocKvtml2Reader::readEntry( QDomElement &entryElement )
{
    QDomElement currentElement;
    bool result = true;

    // get entry id
    int id = entryElement.attribute( KVTML_ID ).toInt( &result );
    if ( !result ) {
        m_errorMessage =  "entry missing id" ;
        return false;
    }

    QmVocExpression *expr = new QmVocExpression;

    // read info tags: inactive, inquery, and sizehint
    currentElement = entryElement.firstChildElement( KVTML_DEACTIVATED );
    if ( !currentElement.isNull() ) {
        // set the active state of the expression
        if ( currentElement.text() == KVTML_TRUE ) {
            expr->setActive( false );
        } else {
            expr->setActive( true );
        }
    }

    // read translation children
    QDomNodeList translationList = entryElement.elementsByTagName( KVTML_TRANSLATION );

    for ( int i = 0; i < translationList.count(); ++i ) {
        currentElement = translationList.item( i ).toElement();
        if ( currentElement.parentNode() == entryElement ) {
            result = readTranslation( currentElement, expr, i );
            if ( !result )
                return false;
        }
    }

    if ( expr->translationIndices().size() == 0 ) {
        qDebug() << "Found entry with no words in it." << id;
        expr->setTranslation(0, QString());
    }

    Q_ASSERT(expr);

    // TODO: probably should insert at id position with a check to see if it exists
    // may be useful for detecting corrupt documents
    m_allEntries[id] = expr;
    return result;
}

bool QmVocKvtml2Reader::readTranslation( QDomElement &translationElement,
        QmVocExpression *expr, int index )
{
    // read the text, grade, declension and conjugation
    expr->translation(index)->fromKVTML2(translationElement);
    QDomElement currentElement;

    // article grade
    currentElement = translationElement.firstChildElement( KVTML_ARTICLE );
    if ( !currentElement.isNull() ) {
        KEduVocText article;
        article.fromKVTML2(currentElement);
        expr->translation(index)->setArticle(article);
    }

    // comparisons
    currentElement = translationElement.firstChildElement( KVTML_COMPARISON );
    if ( !currentElement.isNull() ) {
        readComparison( currentElement, expr->translation(index) );
    }

    // multiple choice
    currentElement = translationElement.firstChildElement( KVTML_MULTIPLECHOICE );
    if ( !currentElement.isNull() ) {
        readMultipleChoice( currentElement, expr->translation(index) );
    }

    // image
    currentElement = translationElement.firstChildElement( KVTML_IMAGE );
    if ( !currentElement.isNull() ) {
        qCritical("Fixme: get image path...");
        // expr->translation(index)->setImageUrl( KUrl( m_doc->url(), currentElement.text() ) );
    }

    // sound
    currentElement = translationElement.firstChildElement( KVTML_SOUND );
    if ( !currentElement.isNull() ) {
        qCritical("Fixme: get sound path...");
        //  expr->translation(index)->setSoundUrl( KUrl( m_doc->url(), currentElement.text() ) );
    }

    return true;
}

bool QmVocKvtml2Reader::readChildLessons( QmVocLesson* parentLesson, QDomElement &lessonElement )
{
    QDomElement currentElement = lessonElement.firstChildElement( KVTML_CONTAINER );
    while ( !currentElement.isNull() ) {
        readLesson(parentLesson, currentElement);
        currentElement = currentElement.nextSiblingElement( KVTML_CONTAINER );
    }
    return true;
}

bool QmVocKvtml2Reader::readLesson( QmVocLesson* parentLesson, QDomElement &lessonElement )
{
    //<name>Lesson name</name>
    QDomElement currentElement = lessonElement.firstChildElement( KVTML_NAME );
    QmVocLesson * lesson = new QmVocLesson(currentElement.text(), parentLesson);
    parentLesson->appendChildContainer( lesson );

    readChildLessons( lesson, lessonElement );

    //<query>true</query>
    currentElement = lessonElement.firstChildElement( KVTML_INPRACTICE );
    lesson->setInPractice(currentElement.text() == KVTML_TRUE);

    //<entry id="123"/>
    currentElement = lessonElement.firstChildElement( KVTML_ENTRY );
    while ( !currentElement.isNull() ) {
        bool result = false;
        int entryId = currentElement.attribute( KVTML_ID ).toInt( &result );
        if(result) {
            if (m_allEntries[entryId]) {
                lesson->appendEntry( m_allEntries[entryId] );
            }
        }
        currentElement = currentElement.nextSiblingElement( KVTML_ENTRY );
    }
    return true;
}

bool QmVocKvtml2Reader::readSynonymsAntonymsFalseFriends( QDomElement &rootElement )
{
    QDomElement pairElement;
    for(int type = QmVocTranslation::Synonym; type <= QmVocTranslation::FalseFriend; type++) {
        switch (type) {
        case QmVocTranslation::Synonym:
            pairElement= rootElement.firstChildElement( KVTML_SYNONYM );
            break;
        case QmVocTranslation::Antonym:
            pairElement= rootElement.firstChildElement( KVTML_ANTONYM );
            break;
        case QmVocTranslation::FalseFriend:
            pairElement= rootElement.firstChildElement( KVTML_FALSEFRIEND );
            break;
        }
        // pair
        pairElement = pairElement.firstChildElement( KVTML_PAIR );
        while ( !pairElement.isNull() ) {
            //<entry id="123"/>
            QDomElement entryElement = pairElement.firstChildElement( KVTML_ENTRY );
            int firstEntryId = entryElement.attribute( KVTML_ID ).toInt();

            QDomElement translationElement = entryElement.firstChildElement( KVTML_TRANSLATION );
            int firstTranslationId = translationElement.attribute( KVTML_ID ).toInt();

            // second entry
            entryElement = entryElement.nextSiblingElement( KVTML_ENTRY );
            int secondEntryId = entryElement.attribute( KVTML_ID ).toInt();
            translationElement = entryElement.firstChildElement( KVTML_TRANSLATION );
            int secondTranslationId = translationElement.attribute( KVTML_ID ).toInt();

            // pair them up
            QmVocTranslation *first = m_allEntries[firstEntryId]->translation(firstTranslationId);
            QmVocTranslation *second = m_allEntries[secondEntryId]->translation(secondTranslationId);

            switch (type) {
            case QmVocTranslation::Synonym:
                first->addSynonym(second);
                second->addSynonym(first);
                break;
            case QmVocTranslation::Antonym:
                first->addAntonym(second);
                second->addAntonym(first);
                break;
            case QmVocTranslation::FalseFriend:
                first->addFalseFriend(second);
                second->addFalseFriend(first);
                break;
            }
            pairElement = pairElement.nextSiblingElement( KVTML_PAIR );
        }
    }
    return true;
}

bool QmVocKvtml2Reader::readArticle( QDomElement &articleElement, int identifierNum )
/*
 <article>
  <singlular>
    <definite>
        <male>der</male>
        <female>die</female>
        <neutral>das</neutral>
    </definite>
    <indefinite>
        <male>ein</male>
        <female>eine</female>
        <neutral>ein</neutral>
    </indefinite>
  </singular>
  <dual>
  </dual>
 </article>
*/
{
    QMap<int, QmVocWordFlag::Flags> numbers;
    numbers[0] = QmVocWordFlag::Singular;
    numbers[1] = QmVocWordFlag::Dual;
    numbers[2] = QmVocWordFlag::Plural;
    QMap<int, QmVocWordFlag::Flags> genders;
    genders[0] = QmVocWordFlag::Masculine;
    genders[1] = QmVocWordFlag::Feminine;
    genders[2] = QmVocWordFlag::Neuter;
    QMap<int, QmVocWordFlag::Flags> defs;
    defs[0] = QmVocWordFlag::Definite;
    defs[1] = QmVocWordFlag::Indefinite;

    for ( int num = 0; num <= 2; ++num) {
        QDomElement numberElement = articleElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[num] );
        if (!numberElement.isNull()) {
            // definite
            for ( int def = 0; def <= 1; ++def ) {
                QDomElement defElement = numberElement.firstChildElement( KVTML_GRAMMATICAL_DEFINITENESS[def] );
                if (!defElement.isNull()) {
                    // male
                    for ( int gen = 0; gen <= 2; ++gen ) {
                        QDomElement genderElement = defElement.firstChildElement( KVTML_GRAMMATICAL_GENDER[gen] );
                        if (!genderElement.isNull()) {
                            m_doc->identifier(identifierNum).article().setArticle( genderElement.text(), numbers[num] | defs[def] | genders[gen]);
                        }
                    }
                }
            }
        }
    }

    return true;
}


bool QmVocKvtml2Reader::readChildWordTypes(QmVocWordType* parentContainer, QDomElement &lessonElement)
{
    QDomElement currentElement = lessonElement.firstChildElement( KVTML_CONTAINER );
    while ( !currentElement.isNull() ) {
        readWordType(parentContainer, currentElement);
        currentElement = currentElement.nextSiblingElement( KVTML_CONTAINER );
    }
    return true;
}

bool QmVocKvtml2Reader::readLeitner( QmVocLeitnerBox* parentContainer, QDomElement &leitnerParentElement )
{
    QDomElement leitnerElement = leitnerParentElement.firstChildElement( KVTML_CONTAINER );
    while ( !leitnerElement.isNull() ) {
        QString name = leitnerElement.firstChildElement( KVTML_NAME ).text();

        QmVocLeitnerBox * leitner = new QmVocLeitnerBox(name, parentContainer);
        parentContainer->appendChildContainer(leitner);
        // for leitner we only allow a flat list, no sub boxes.

        // read entries
        QDomElement entryElement = leitnerElement.firstChildElement( KVTML_ENTRY );
        while ( !entryElement.isNull() ) {
            // read <entry id="123"></entryid>
            int entryId = entryElement.attribute( KVTML_ID ).toInt();
            QDomElement translationElement = entryElement.firstChildElement( KVTML_TRANSLATION );
            while( !translationElement.isNull() ) {
                // <translation id="234"/>
                int translationId = translationElement.attribute( KVTML_ID ).toInt();
                m_allEntries.value(entryId)->translation(translationId)->setLeitnerBox(leitner);
                translationElement = translationElement.nextSiblingElement( KVTML_TRANSLATION );
            }
            entryElement = entryElement.nextSiblingElement( KVTML_ENTRY );
        }
        leitnerElement = leitnerElement.nextSiblingElement( KVTML_CONTAINER );
    }
    return true;
}

bool QmVocKvtml2Reader::readWordType( QmVocWordType* parentContainer, QDomElement &typeElement )
{
    // set type and specialtype
    QString typeName =
        typeElement.firstChildElement( KVTML_NAME ).text();

    QmVocWordType * wordTypeContainer = new QmVocWordType(typeName, parentContainer);
    parentContainer->appendChildContainer(wordTypeContainer);

    QString specialType = typeElement.firstChildElement( KVTML_SPECIALWORDTYPE ).text();
    if ( !specialType.isEmpty() ) {
        // get the localized version
        if ( specialType == KVTML_SPECIALWORDTYPE_NOUN ) {
            wordTypeContainer->setWordType(QmVocWordFlag::Noun);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_VERB ) {
            wordTypeContainer->setWordType(QmVocWordFlag::Verb);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_ADVERB ) {
            wordTypeContainer->setWordType(QmVocWordFlag::Adverb);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_ADJECTIVE ) {
            wordTypeContainer->setWordType(QmVocWordFlag::Adjective);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_NOUN_MALE ) {
            wordTypeContainer->setWordType(QmVocWordFlag::Noun | QmVocWordFlag::Masculine);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_NOUN_FEMALE ) {
            wordTypeContainer->setWordType(QmVocWordFlag::Noun | QmVocWordFlag::Feminine);
        }
        if ( specialType == KVTML_SPECIALWORDTYPE_NOUN_NEUTRAL ) {
            wordTypeContainer->setWordType(QmVocWordFlag::Noun| QmVocWordFlag::Neuter);
        }
    } // special type

    // read entries
    QDomElement entryElement = typeElement.firstChildElement( KVTML_ENTRY );
    while ( !entryElement.isNull() ) {
        // read <entry id="123"></entryid>
        int entryId = entryElement.attribute( KVTML_ID ).toInt();
        QDomElement translationElement = entryElement.firstChildElement( KVTML_TRANSLATION );
        while( !translationElement.isNull() ) {
            // <translation id="234"/>
            int translationId = translationElement.attribute( KVTML_ID ).toInt();
            m_allEntries.value(entryId)->translation(translationId)->setWordType(wordTypeContainer);
            translationElement = translationElement.nextSiblingElement( KVTML_TRANSLATION );
        }
        entryElement = entryElement.nextSiblingElement( KVTML_ENTRY );
    }

    readChildWordTypes(wordTypeContainer, typeElement);

    return true;
}

QStringList QmVocKvtml2Reader::readTenses( QDomElement &tensesElement )
{
    QStringList tenses;

    QDomNodeList tenseNodes = tensesElement.elementsByTagName( KVTML_TENSE );
    for ( int i = 0; i < tenseNodes.count(); ++i ) {
        QDomElement currentElement = tenseNodes.item( i ).toElement();
        if ( currentElement.parentNode() == tensesElement ) {
            tenses.append( currentElement.text() );
        }
    }

    return tenses;
}

bool QmVocKvtml2Reader::readComparison( QDomElement &domElementParent, QmVocTranslation* translation )
/*
 <comparison>
   <comparative>better</comparative>
   <superlative>best</superlative>
 </comparison>
*/
{
    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KVTML_COMPARATIVE );
    if ( !currentElement.isNull() )
    {
        QmVocText comparative;
        comparative.fromKVTML2(currentElement);

        // be compatible for KDE < 4.5
        if (comparative.text().isEmpty()) {
            comparative.setText(currentElement.text());
        }
        translation->setComparativeForm(comparative);
    }

    currentElement = domElementParent.firstChildElement( KVTML_SUPERLATIVE );
    if ( !currentElement.isNull() )
    {
        QmVocText superlative;
        superlative.fromKVTML2(currentElement);

        // be compatible for KDE < 4.5
        if (superlative.text().isEmpty()) {
            superlative.setText(currentElement.text());
        }
        translation->setSuperlativeForm(superlative);
    }
    return true;
}

bool QmVocKvtml2Reader::readMultipleChoice( QDomElement &multipleChoiceElement, QmVocTranslation* translation )
/*
 <multiplechoice>
   <choice>good</choice>
   <choice>better</choice>
   <choice>best</choice>
   <choice>best 2</choice>
   <choice>best 3</choice>
 </multiplechoice>
*/
{
    QDomElement currentElement;
    QDomNodeList choiceNodes = multipleChoiceElement.elementsByTagName( KVTML_CHOICE );
    for ( int i = 0; i < choiceNodes.count(); ++i )
    {
        currentElement = choiceNodes.item( i ).toElement();
        if ( currentElement.parentNode() == multipleChoiceElement ) {
            translation->multipleChoice().append( currentElement.text() );
        }
    }
    return true;
}


bool QmVocKvtml2Reader::readPersonalPronoun(QDomElement & pronounElement, QmVocPersonalPronoun & pronoun)
{
    pronoun.setMaleFemaleDifferent(!pronounElement.firstChildElement(
        KVTML_THIRD_PERSON_MALE_FEMALE_DIFFERENT).isNull());
    pronoun.setNeutralExists( !pronounElement.firstChildElement(
        KVTML_THIRD_PERSON_NEUTRAL_EXISTS).isNull() );
    pronoun.setDualExists( !pronounElement.firstChildElement(
        KVTML_DUAL_EXISTS).isNull() );

    QDomElement personElement = pronounElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[0] );
    if ( !personElement.isNull() ) {
        readPersonalPronounChild( personElement, pronoun, QmVocWordFlag::Singular );
    }

    personElement = pronounElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[1] );
    if ( !personElement.isNull() ) {
        readPersonalPronounChild( personElement, pronoun, QmVocWordFlag::Dual );
    }

    personElement = pronounElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[2] );
    if ( !personElement.isNull() ) {
        readPersonalPronounChild( personElement, pronoun, QmVocWordFlag::Plural );
    }
    return true;
}


bool QmVocKvtml2Reader::readPersonalPronounChild(QDomElement & personElement, QmVocPersonalPronoun & pronoun, QmVocWordFlags number)
{
    QMap<int, QmVocWordFlag::Flags> persons;
    persons[0] = QmVocWordFlag::First;
    persons[1] = QmVocWordFlag::Second;
    persons[2] = (QmVocWordFlag::Flags)((int)QmVocWordFlag::Third | (int)QmVocWordFlag::Masculine);
    persons[3] = (QmVocWordFlag::Flags)((int)QmVocWordFlag::Third | (int)QmVocWordFlag::Feminine);
    persons[4] = (QmVocWordFlag::Flags)((int)QmVocWordFlag::Third | (int)QmVocWordFlag::Neuter);



    for (int person = 0; person < 5; person++) {
        QDomElement currentElement = personElement.firstChildElement( KVTML_GRAMMATICAL_PERSON[person] );
        pronoun.setPersonalPronoun( currentElement.text(), persons[person] | number );
    }

    return true;
}


