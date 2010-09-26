/***************************************************************************
*   this file is from kdeedu project. Filename: keduvockvtml2writer.cpp
***************************************************************************/

/***************************************************************************
                   export a QmVocDocument to a KVTML file
    -----------------------------------------------------------------------
    copyright           : (C) 2007 Jeremy Whiting <jpwhiting@kde.org>
    Copyright 2007-2010 Frederik Gladhorn <gladhorn@kde.org>
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

#include "qmvockvtml2writer.h"

#include <QtCore/QTextStream>
#include <QtCore/QFile>

// #include <KDebug>
#include <QDebug>

#include "qmvocdocument.h"
#include "qmvocexpression.h"
#include "qmvoclesson.h"
#include "qmvocleitnerbox.h"
#include "qmvocwordtype.h"
#include "kvtml2defs.h"

QmVocKvtml2Writer::QmVocKvtml2Writer( QFile *file )
{
    // the file must be already open
    m_outputFile = file;
}

bool QmVocKvtml2Writer::writeDoc( QmVocDocument *doc, const QString &generator )
{
    if (createXmlDocument(doc, generator)) {
        QTextStream ts( m_outputFile );
        m_domDoc.save( ts, 2 );
        return true;
    }
    return false;
}

QByteArray QmVocKvtml2Writer::toByteArray(QmVocDocument * doc, const QString & generator)
{
    if (createXmlDocument(doc, generator)) {
        return m_domDoc.toByteArray();
    }
    return QByteArray();
}

bool QmVocKvtml2Writer::createXmlDocument( QmVocDocument *doc, const QString &generator )
{
    m_doc = doc;

    m_domDoc = QDomDocument( "kvtml PUBLIC \"kvtml2.dtd\" \"http://edu.kde.org/kvtml/kvtml2.dtd\"" );
    m_domDoc.appendChild( m_domDoc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );
    QDomElement domElementKvtml = m_domDoc.createElement( "kvtml" );
    m_domDoc.appendChild( domElementKvtml );

    domElementKvtml.setAttribute( KVTML_VERSION, ( QString ) "2.0" );

    // information group
    QDomElement currentElement = m_domDoc.createElement( KVTML_INFORMATION );
    writeInformation( currentElement, generator );
    domElementKvtml.appendChild( currentElement );

    // identifiers
    currentElement = m_domDoc.createElement( KVTML_IDENTIFIERS );
    writeIdentifiers( currentElement );
    domElementKvtml.appendChild( currentElement );

    // entries
    currentElement = m_domDoc.createElement( KVTML_ENTRIES );
    if ( !writeEntries( currentElement ) ) {
        // at least one entry is required!
        return false;
    }
    domElementKvtml.appendChild( currentElement );

    // lessons
    currentElement = m_domDoc.createElement( KVTML_LESSONS );
    writeLessons( m_doc->lesson(), currentElement );
    if ( currentElement.hasChildNodes() ) {
        domElementKvtml.appendChild( currentElement );
    }

    // types
    currentElement = m_domDoc.createElement( KVTML_WORDTYPES );
    writeWordTypes( currentElement, m_doc->wordTypeContainer() );
    if ( currentElement.hasChildNodes() ) {
        domElementKvtml.appendChild( currentElement );
    }

    // leitner boxes
    currentElement = m_domDoc.createElement( KVTML_LEITNERBOXES );
    writeLeitnerBoxes( currentElement, m_doc->leitnerContainer() );
    if ( currentElement.hasChildNodes() ) {
        domElementKvtml.appendChild( currentElement );
    }

    writeSynonymAntonymFalseFriend(domElementKvtml);

    m_domDoc.appendChild( domElementKvtml );

    return true;
}

bool QmVocKvtml2Writer::writeInformation( QDomElement &informationElement, const QString &generator )
{
    QDomElement currentElement;
    QDomText textNode;

    // generator
    informationElement.appendChild( newTextElement( KVTML_GENERATOR, generator ) );

    // title
    if ( !m_doc->title().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_TITLE, m_doc->title() ) );
    }

    // author
    if ( !m_doc->author().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_AUTHOR, m_doc->author() ) );
    }

    // author contact (mail/homepage)
    if ( !m_doc->authorContact().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_AUTHORCONTACT, m_doc->authorContact() ) );
    }

    // license
    if ( !m_doc->license().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_LICENSE, m_doc->license() ) );
    }

    // comment
    if ( !m_doc->documentComment().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_COMMENT, m_doc->documentComment() ) );
    }

    QDate today = QDate::currentDate();
    informationElement.appendChild( newTextElement( KVTML_DATE, today.toString(QLatin1String("yyyy-MM-dd")) ) );

    // category
    if ( !m_doc->category().isEmpty() ) {
        informationElement.appendChild( newTextElement( KVTML_CATEGORY, m_doc->category() ) );
    }

    return true;
}


bool QmVocKvtml2Writer::writeIdentifiers( QDomElement &identifiersElement )
{
    for ( int i = 0; i < m_doc->identifierCount(); ++i ) {
        // create the node
        QDomElement identifier = m_domDoc.createElement( KVTML_IDENTIFIER );

        // set the id
        identifier.setAttribute( KVTML_ID, QString::number( i ) );

        // record the identifier as the locale for now
        // TODO: when support for more parts of the identifier is in the document class (name, type, etc.) store those here as well
        identifier.appendChild( newTextElement( KVTML_NAME, m_doc->identifier( i ).name() ) );

        identifier.appendChild( newTextElement( KVTML_LOCALE, m_doc->identifier( i ).locale() ) );

        // record articles
        QDomElement article = m_domDoc.createElement( KVTML_ARTICLE );
        writeArticle( article, i );
        if ( article.hasChildNodes() ) {
            identifier.appendChild( article );
        }

        // record personalpronouns
        QDomElement personalpronouns = m_domDoc.createElement( KVTML_PERSONALPRONOUNS );
        writePersonalPronoun( personalpronouns, m_doc->identifier(i).personalPronouns() );
        if ( personalpronouns.hasChildNodes() ) {
            identifier.appendChild( personalpronouns );
        }

        // tenses
        foreach(const QString &tense, m_doc->identifier(i).tenseList() ) {
            if ( !( tense.isNull() ) ) {
                identifier.appendChild( newTextElement( KVTML_TENSE, tense ) );
            }
        }
        // add this identifier to the group
        identifiersElement.appendChild( identifier );
    }
    return true;
}

bool QmVocKvtml2Writer::writeLessons( QmVocLesson *parentLesson, QDomElement &lessonsElement )
{
    // iterate over child lessons.
    // the first time this is called with the root lesson which does not have a <lesson> entry.
    for( int i = 0; i < parentLesson->childContainerCount(); i++ ) {
        QmVocLesson *lesson = static_cast<QmVocLesson*>(parentLesson->childContainer(i));
        // make lesson element
        QDomElement thisLessonElement = m_domDoc.createElement( KVTML_CONTAINER );

        // add a name
        thisLessonElement.appendChild( newTextElement( KVTML_NAME, lesson->name() ) );

        // add a inquery tag
        if ( lesson->inPractice() ) {
            thisLessonElement.appendChild( newTextElement( KVTML_INPRACTICE, KVTML_TRUE ) );
        }

        // child lessons
        writeLessons(lesson, thisLessonElement);

        // child entries
        foreach(QmVocExpression *entry, lesson->entries()) {
            QDomElement entryElement = m_domDoc.createElement( KVTML_ENTRY );
            entryElement.setAttribute( KVTML_ID, QString::number(m_allEntries.indexOf(entry)) );
            thisLessonElement.appendChild(entryElement);
        }
        lessonsElement.appendChild( thisLessonElement );
    }
    return true;
}



void QmVocKvtml2Writer::writeSynonymAntonymFalseFriend(QDomElement & parentElement)
{
    QList< QmVocTranslation* > currentList;
    QDomElement synonymElement;
    // synonym, antonym, false friend
    for(int type = QmVocTranslation::Synonym; type <= QmVocTranslation::FalseFriend; type++) {
        switch (type) {
            case QmVocTranslation::Synonym:
                synonymElement = m_domDoc.createElement( KVTML_SYNONYM );
                currentList = m_synonyms;
                break;
            case QmVocTranslation::Antonym:
                synonymElement = m_domDoc.createElement( KVTML_ANTONYM );
                currentList = m_antonyms;
                break;
            case QmVocTranslation::FalseFriend:
                synonymElement = m_domDoc.createElement( KVTML_FALSEFRIEND );
                currentList = m_falseFriends;
                break;
        }

        while (!currentList.isEmpty()) {
            // after writing a translation, remove it from the list
            QmVocTranslation* translation = currentList.takeFirst();
	    
            QDomElement relatedElement;
            QList <QmVocTranslation*> list;
            switch (type) {
            case QmVocTranslation::Synonym:
                list = translation->synonyms();
                break;
            case QmVocTranslation::Antonym:
                list = translation->antonyms();
                break;
            case QmVocTranslation::FalseFriend:
                list = translation->falseFriends();
                break;
            }
            foreach (QmVocTranslation* synonym, list) {
                // if it is not in the list it has already been written and we can move on
                if (currentList.contains(synonym)) {
                    relatedElement = m_domDoc.createElement( KVTML_PAIR );

		    // fill the entry element but only add later if it is valid
		    QDomElement entryElement = m_domDoc.createElement( KVTML_ENTRY );
		    entryElement.setAttribute( KVTML_ID, QString::number(m_allEntries.indexOf(translation->entry())) );
		    // find out which id that is... silly
		    foreach(int index, translation->entry()->translationIndices()) {
			if (translation->entry()->translation(index) == translation) {
			    // create <translation id="123">
			    QDomElement translationElement = m_domDoc.createElement( KVTML_TRANSLATION );
			    translationElement.setAttribute( KVTML_ID, QString::number(index) );
			    entryElement.appendChild(translationElement);
			    break;
			}
		    }
		    
                    relatedElement.appendChild(entryElement);


                    QDomElement partnerElement = m_domDoc.createElement( KVTML_ENTRY );
                    partnerElement.setAttribute( KVTML_ID, QString::number(m_allEntries.indexOf(synonym->entry())) );
                    // find out which id that is
                    foreach(int index, synonym->entry()->translationIndices()) {
                        if (synonym->entry()->translation(index) == synonym) {
                            // create <translation id="123">
                            QDomElement translationElement = m_domDoc.createElement( KVTML_TRANSLATION );
                            translationElement.setAttribute( KVTML_ID, QString::number(index) );
                            partnerElement.appendChild(translationElement);
                            break;
                        }
                    }
                    relatedElement.appendChild( partnerElement );
		    synonymElement.appendChild(relatedElement);
                }
            }
            
        }
        if (synonymElement.hasChildNodes()) {
            parentElement.appendChild( synonymElement );
        }
    } // iterate over types
}
/*
bool QmVocKvtml2Writer::writeRelated(QDomElement & parentElement, QList< QmVocTranslation * > relatedList)
{
    foreach (QmVocTranslation* synonym, translation->synonyms()) {
        QDomElement entryElement = m_domDoc.createElement( KVTML_ENTRY );
        entryElement.setAttribute( KVTML_ID, QString::number(m_allEntries.indexOf(translation->entry())) );

        // find out which id that is... silly
        foreach(int index, translation->entry()->translationIndices()) {
            if (translation->entry()->translation(index) == translation) {
                // create <translation id="123">
                QDomElement translationElement = m_domDoc.createElement( KVTML_TRANSLATION );
                translationElement.setAttribute( KVTML_ID, QString::number(index) );
                entryElement.appendChild(translationElement);
            }
        }
        parentElement.appendChild( entryElement );
    }
}*/

bool QmVocKvtml2Writer::writeArticle( QDomElement &articleElement, int language )
{
    ///@todo only write if not empty
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

    for (int num = 0; num <= 2; num++)
    {
        QDomElement numberElement = m_domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );

        for (int def = 0; def <= 1; def++) {
            QDomElement defElement = m_domDoc.createElement( KVTML_GRAMMATICAL_DEFINITENESS[def] );

            for (int gen = 0; gen <= 2; gen++)
            {
                QString articleString = m_doc->identifier(language).article().article(numbers[num] | genders[gen] | defs[def]);
                if ( !articleString.isEmpty() ) {
                    defElement.appendChild( newTextElement( KVTML_GRAMMATICAL_GENDER[gen], articleString ) );
                }
            }
            if ( defElement.hasChildNodes() ) {
                numberElement.appendChild( defElement );
            }
        }
        if ( numberElement.hasChildNodes() ) {
            articleElement.appendChild( numberElement );
        }
    }
    return true;
}


bool QmVocKvtml2Writer::writeWordTypes( QDomElement &typesElement, QmVocWordType* parentContainer )
{
    foreach( QmVocContainer* container, parentContainer->childContainers() ) {
        QmVocWordType* wordType = static_cast<QmVocWordType*>(container);

        QDomElement typeDefinitionElement = m_domDoc.createElement( KVTML_CONTAINER );
        typeDefinitionElement.appendChild( newTextElement( KVTML_NAME, wordType->name() ) );

        if (wordType->wordType().testFlag(QmVocWordFlag::Noun))
        {
            if (wordType->wordType().testFlag(QmVocWordFlag::Masculine))
                typeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, KVTML_SPECIALWORDTYPE_NOUN_MALE ) );

            else if (wordType->wordType().testFlag(QmVocWordFlag::Feminine))
                typeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, KVTML_SPECIALWORDTYPE_NOUN_FEMALE ) );

            else if (wordType->wordType().testFlag(QmVocWordFlag::Neuter))
                typeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, KVTML_SPECIALWORDTYPE_NOUN_NEUTRAL ) );
            else
                typeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, KVTML_SPECIALWORDTYPE_NOUN ) );
        }
        else if (wordType->wordType().testFlag(QmVocWordFlag::Verb))
            typeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, KVTML_SPECIALWORDTYPE_VERB ) );

        else if (wordType->wordType().testFlag(QmVocWordFlag::Adjective))
            typeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, KVTML_SPECIALWORDTYPE_ADJECTIVE ) );

        else if (wordType->wordType().testFlag(QmVocWordFlag::Adverb))
            typeDefinitionElement.appendChild( newTextElement( KVTML_SPECIALWORDTYPE, KVTML_SPECIALWORDTYPE_ADVERB ) );


// child entries

        // child entries
        foreach(QmVocExpression *entry, wordType->entries()) {
            QDomElement entryElement = m_domDoc.createElement( KVTML_ENTRY );
            entryElement.setAttribute( KVTML_ID, QString::number(m_allEntries.indexOf(entry)) );
            for(int translation = 0; translation<m_doc->identifierCount(); translation++) {
                if (entry->translation(translation)->wordType()== wordType) {
                    QDomElement translationElement = m_domDoc.createElement( KVTML_TRANSLATION );
                    // create <translation id="123">
                    translationElement.setAttribute( KVTML_ID, QString::number(translation) );
                    // append both
                    entryElement.appendChild(translationElement);
                }
            }
            typeDefinitionElement.appendChild( entryElement );
        }

        writeWordTypes( typeDefinitionElement, wordType );

        typesElement.appendChild( typeDefinitionElement );
    }
    return true;
}

bool QmVocKvtml2Writer::writeLeitnerBoxes( QDomElement &leitnerParentElement, QmVocLeitnerBox* parentContainer )
{
    foreach( QmVocContainer* container, parentContainer->childContainers() ) {
        QmVocLeitnerBox* leitnerBox = static_cast<QmVocLeitnerBox*>(container);

        QDomElement containerElement = m_domDoc.createElement( KVTML_CONTAINER );
        containerElement.appendChild( newTextElement( KVTML_NAME, leitnerBox->name() ) );

        // child entries
        foreach(QmVocExpression *entry, leitnerBox->entries()) {
            QDomElement entryElement = m_domDoc.createElement( KVTML_ENTRY );
            entryElement.setAttribute( KVTML_ID, QString::number(m_allEntries.indexOf(entry)) );
            for(int translation = 0; translation<m_doc->identifierCount(); translation++) {
                if (entry->translation(translation)->leitnerBox()== leitnerBox) {
                    QDomElement translationElement = m_domDoc.createElement( KVTML_TRANSLATION );
                    // create <translation id="123">
                    translationElement.setAttribute( KVTML_ID, QString::number(translation) );
                    // append both
                    entryElement.appendChild(translationElement);
                }
            }
            containerElement.appendChild( entryElement );
        }

        leitnerParentElement.appendChild( containerElement );
    }
    return true;
}

bool QmVocKvtml2Writer::writeEntries( QDomElement &entriesElement )
{
    m_allEntries = m_doc->lesson()->entries(QmVocLesson::Recursive);

    // loop through entries
    for ( int i = 0; i < m_allEntries.count(); ++i ) {
        QmVocExpression *thisEntry = m_allEntries.value(i);

        // write entry tag
        QDomElement entryElement = m_domDoc.createElement( KVTML_ENTRY );

        // add id
        entryElement.setAttribute( KVTML_ID, QString::number( i ) );

        // write deactivated
        if(!thisEntry->isActive()) {
            entryElement.appendChild( newTextElement( KVTML_DEACTIVATED, KVTML_TRUE ) );
        }

        // loop through translations
        foreach( int trans, thisEntry->translationIndices() ) {
            // write translations
            QDomElement translation = m_domDoc.createElement( KVTML_TRANSLATION );
            translation.setAttribute( KVTML_ID, QString::number( trans ) );
            writeTranslation( translation, thisEntry->translation( trans ) );
            entryElement.appendChild( translation );
        }
        // add this entry to the entriesElement
        entriesElement.appendChild( entryElement );
    }
    return true;
}


bool QmVocKvtml2Writer::writeTranslation( QDomElement &translationElement, QmVocTranslation* translation )
{
    // so far only for QmVocWord - text and grades
    translation->toKVTML2(translationElement);

    // comparison
    if ( !(translation->comparativeForm().text().isEmpty() || translation->superlativeForm().text().isEmpty())) {
        // qDebug() << "Write comp";
        QDomElement comparisonElement = m_domDoc.createElement( KVTML_COMPARISON );
        translationElement.appendChild(comparisonElement);

        QDomElement comparativeElement = m_domDoc.createElement( KVTML_COMPARATIVE );
        comparisonElement.appendChild(comparativeElement);
        translation->comparativeForm().toKVTML2(comparativeElement);

        QDomElement superlativeElement = m_domDoc.createElement( KVTML_SUPERLATIVE );
        comparisonElement.appendChild(superlativeElement);
        translation->superlativeForm().toKVTML2(superlativeElement);
    }

    if (translation->article().practiceCount() != 0) {
        QDomElement articleElement = m_domDoc.createElement( KVTML_ARTICLE );
        translation->article().toKVTML2(articleElement);
        translationElement.appendChild(articleElement);
    }

    // multiplechoice
    if ( !translation->multipleChoice().isEmpty() ) {
        QDomElement multipleChoiceElement = m_domDoc.createElement( KVTML_MULTIPLECHOICE );
        writeMultipleChoice( multipleChoiceElement, translation );
        translationElement.appendChild( multipleChoiceElement );
    }

    // image
    if ( !translation->imageUrl().isEmpty() ) {
        qCritical("Fixme: imageURL is Empty...");
//         QString urlString;
//         if ( translation->imageUrl().url().startsWith(m_doc->url().upUrl().url()) ) {
//             // try to save as relative url
//             urlString = KUrl::relativeUrl( m_doc->url() , translation->imageUrl() );
//         } else {
//             urlString =  translation->imageUrl().url();
//         }
//         translationElement.appendChild( newTextElement( KVTML_IMAGE, urlString ) );
    }

    // sound
    if ( !translation->soundUrl().isEmpty() ) {
        qCritical("Fixme: soundURL is Empty...");
//         QString urlString;
//         if ( translation->soundUrl().url().startsWith(m_doc->url().upUrl().url()) ) {
//             // try to save as relative url
//             urlString = KUrl::relativeUrl( m_doc->url() , translation->soundUrl() );
//         } else {
//             urlString =  translation->soundUrl().url();
//         }
//         translationElement.appendChild( newTextElement( KVTML_SOUND, urlString ) );
    }


    // synonym, antonym, false friend
    // add to the list if it has any, write later since we want them separate
    if (!translation->synonyms().isEmpty()) {
        m_synonyms.append(translation);
    }
    if (!translation->antonyms().isEmpty()) {
        m_antonyms.append(translation);
    }
    if (!translation->falseFriends().isEmpty()) {
        m_falseFriends.append(translation);
    }
    return true;
}



    ///@todo write false friends
    // <falsefriend fromid="0"></falsefriend>
    // loop through the identifiers
//     for ( int i = 0; i < m_doc->identifierCount(); ++i ) {
//         // see if this identifier has a falsefriend in this translation
//         QString thisFriend = translation->falseFriend( i );
//         if ( !thisFriend.isEmpty() ) {
//             // if so, create it, and set the fromid to i
//             QDomElement thisFriendElement = newTextElement( KVTML_FALSEFRIEND, thisFriend );
//             thisFriendElement.setAttribute( KVTML_FROMID, QString::number( i ) );
//             translationElement.appendChild( thisFriendElement );
//         }
//     }

bool QmVocKvtml2Writer::writeMultipleChoice( QDomElement &multipleChoiceElement, QmVocTranslation* translation )
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
    foreach ( const QString &choice, translation->multipleChoice() ) {
        multipleChoiceElement.appendChild( newTextElement( KVTML_CHOICE, choice ) );
    }
    return true;
}

QDomElement QmVocKvtml2Writer::newTextElement( const QString &elementName, const QString &text )
{
    QDomElement retval = m_domDoc.createElement( elementName );
    QDomText textNode = m_domDoc.createTextNode( text );
    retval.appendChild( textNode );
    return retval;
}

bool QmVocKvtml2Writer::writePersonalPronoun(QDomElement & pronounElement, const QmVocPersonalPronoun & pronoun)
{
    // general pronoun properties
    if ( pronoun.maleFemaleDifferent() ) {
        pronounElement.appendChild( m_domDoc.createElement( KVTML_THIRD_PERSON_MALE_FEMALE_DIFFERENT ) );
    }
    if ( pronoun.neutralExists() ) {
        pronounElement.appendChild( m_domDoc.createElement( KVTML_THIRD_PERSON_NEUTRAL_EXISTS ) );
    }
    if ( pronoun.dualExists() ) {
        pronounElement.appendChild( m_domDoc.createElement( KVTML_DUAL_EXISTS ) );
    }


    QMap<int, QmVocWordFlag::Flags> numbers;
    numbers[0] = QmVocWordFlag::Singular;
    numbers[1] = QmVocWordFlag::Dual;
    numbers[2] = QmVocWordFlag::Plural;
    QMap<int, QmVocWordFlag::Flags> persons;
    persons[0] = QmVocWordFlag::First;
    persons[1] = QmVocWordFlag::Second;
    persons[2] = (QmVocWordFlag::Flags)((int)QmVocWordFlag::Third | (int)QmVocWordFlag::Masculine);
    persons[3] = (QmVocWordFlag::Flags)((int)QmVocWordFlag::Third | (int)QmVocWordFlag::Feminine);
    persons[4] = (QmVocWordFlag::Flags)((int)QmVocWordFlag::Third | (int)QmVocWordFlag::Neuter);



    // the actual pronouns
    for ( int num = 0; num < 3; num++ ) {
        QDomElement numberElement = m_domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );
        for ( int person = 0; person < 5; person++ ) {
            QString pronounString = pronoun.personalPronoun(numbers[num] | persons[person]);
            if (!pronounString.isEmpty()) {
                numberElement.appendChild( newTextElement( KVTML_GRAMMATICAL_PERSON[person], pronounString ));
            }
        }
        if (numberElement.hasChildNodes()) {
            pronounElement.appendChild( numberElement );
        }
    }
    return true;
}

void QmVocKvtml2Writer::appendTextElement(QDomElement & parent, const QString & elementName, const QString & text)
{
    // empty will never be written
    if (text.isEmpty()) {
        return;
    }

    QDomDocument domDoc = parent.ownerDocument();
    QDomElement element = domDoc.createElement( elementName );
    parent.appendChild( element );
    QDomText textNode = domDoc.createTextNode( text );
    element.appendChild( textNode );
}


