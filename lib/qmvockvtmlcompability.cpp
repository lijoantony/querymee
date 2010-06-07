/***************************************************************************
*   this file is from kdeedu project. Filename: keduvockvtmlcompability.cpp
***************************************************************************/

/***************************************************************************

    C++ Implementation: qtvtvockvtml1compability_p

    -----------------------------------------------------------------------

    begin         : Di Aug 28 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
                    (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

    -----------------------------------------------------------------------

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qtvtvockvtmlcompability.h"
#include "qtvtvocwordtype.h"

// #include <KLocale>
// #include <KDebug>
#include <QDebug>
#include <QStringList>

const QString QmVocKvtmlCompability::KVTML_1_USER_DEFINED = QString( "#" );
const QString QmVocKvtmlCompability::KVTML_1_SEPERATOR = QString( ":" );


QmVocKvtmlCompability::QmVocKvtmlCompability()
{
    m_userdefinedTenseCounter = 0;
    m_userdefinedTypeCounter = 0;

    initOldTypeLists();
    initOldTenses();
}


////////////////// TYPES /////////////////////////////////////////
void QmVocKvtmlCompability::initOldTypeLists()
{
    m_oldMainTypeNames.clear();
    m_oldMainTypeNames.insert( "v", "Verb" );
    m_oldMainTypeNames.insert( "n", "Noun" );
    m_oldMainTypeNames.insert( "nm", "Name" );
    m_oldMainTypeNames.insert( "ar", "Article" );
    m_oldMainTypeNames.insert( "aj", "Adjective" );
    m_oldMainTypeNames.insert( "av", "Adverb" );
    m_oldMainTypeNames.insert( "pr", "Pronoun" );
    m_oldMainTypeNames.insert( "ph", "Phrase" );
    m_oldMainTypeNames.insert( "num", "Numeral" );
    m_oldMainTypeNames.insert( "con", "Conjunction" );
    m_oldMainTypeNames.insert( "pre", "Preposition" );
    m_oldMainTypeNames.insert( "qu",  "Question" );


    m_oldSubTypeNames.clear();
    m_oldSubTypeNames.insert( "ord", "Ordinal" );
    m_oldSubTypeNames.insert( "crd", "Cardinal" );
    m_oldSubTypeNames.insert( "def", "Definite" );
    m_oldSubTypeNames.insert( "ind", "Indefinite" );
    m_oldSubTypeNames.insert( "re",  "Regular" );
    m_oldSubTypeNames.insert( "ir",  "Irregular" );
    m_oldSubTypeNames.insert( "pos", "Possessive" );
    m_oldSubTypeNames.insert( "per", "Personal" );
    m_oldSubTypeNames.insert( "m",   "Male" );
    m_oldSubTypeNames.insert( "f",   "Female" );
    m_oldSubTypeNames.insert( "s",   "Neutral" );
}


QmVocWordType* QmVocKvtmlCompability::typeFromOldFormat(QmVocWordType* parent, const QString & typeSubtypeString ) const
{
    // check if it's user defined
    if ( typeSubtypeString.length() >= 2 && typeSubtypeString.left( 1 ) == QM_USER_TYPE ) {
        // they started counting at 1, we need to know which index we are dealing with:
        int selfDefinedTypeIndex = typeSubtypeString.right( typeSubtypeString.count()-1 ).toInt() -1;
        return static_cast<QmVocWordType*>(parent->childContainer(selfDefinedTypeIndex));
    }

    // assume the parent is set up to contain the old types correctly
    QString mainType;
    QString subType;
    int i;

    if (( i = typeSubtypeString.indexOf( KVTML_1_SEPERATOR ) ) >= 0 ) {
        mainType = typeSubtypeString.left( i );
        subType = typeSubtypeString.right( i+1 );
    } else {
        mainType = typeSubtypeString;
    }

    // convert from pre-0.5 versions (I guess we can just leave that in here.
    // I seriously doubt that any such documents exist...
    if ( mainType == "1" ) {
        mainType = QM_VERB;
    } else if ( mainType == "2" ) {
        mainType = QM_NOUN;
    } else if ( mainType == "3" ) {
        mainType = QM_NAME;
    }

    QString typeName = m_oldMainTypeNames.value( mainType );
    if ( typeName.isEmpty() ) {
        qDebug() << "Unknown old maintype: " << typeSubtypeString;
        return 0;
    }

    QString subTypeName = m_oldSubTypeNames.value( subType );

    foreach (QmVocContainer* wordType, parent->childContainers()) {
        if (wordType->name() == typeName) {
            if (subType.isEmpty()) {
                return static_cast<QmVocWordType*>(wordType);
            } else {
                foreach (QmVocContainer* subWordType, wordType->childContainers()) {
                    if (subWordType->name() == subTypeName) {
                        return static_cast<QmVocWordType*>(subWordType);
                    }
                }
            }
        }
    }

    return 0;
}






/*
if ( type.length() >= 2 && type.left( 1 ) == QM_USER_TYPE ) {
                // they started counting at 1, we need to know which index we are dealing with:
                int selfDefinedTypeIndex = type.right( type.count()-1 ).toInt() -1;

                // append invented types (do we not trust our own writer?)
                if ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                    while ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                        m_oldSelfDefinedTypes.append( i18n( "User defined word type %1", m_oldSelfDefinedTypes.count() - 1 ) );
                    }
                }
                type = m_oldSelfDefinedTypes.value( selfDefinedTypeIndex );
            } else {
                type = m_compability.mainTypeFromOldFormat( oldType );
                subType = m_compability.subTypeFromOldFormat( oldType );
            } // not user defined - preset types


if ( oldType.length() >= 2 && type.left( 1 ) == QM_USER_TYPE ) {
            // they started counting at 1
            int selfDefinedTypeIndex = oldType.right( type.count()-1 ).toInt() -1;
            // append invented types (do we not trust our own writer?)
            if ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                while ( selfDefinedTypeIndex >= m_oldSelfDefinedTypes.count() ) {
                    m_oldSelfDefinedTypes.append( i18n( "User defined word type %1", m_oldSelfDefinedTypes.count() - 1 ) );
                }
            }
            type = m_oldSelfDefinedTypes.value( selfDefinedTypeIndex );
        } else {
            type = m_compability.mainTypeFromOldFormat( oldType );
            subType = m_compability.subTypeFromOldFormat( oldType );
        } // not user defined - preset types
    }
*/



void QmVocKvtmlCompability::initOldTenses()
{
    m_oldTenses["PrSi"] =  "Simple Present" ;
    m_oldTenses["PrPr"] =  "Present Progressive" ;
    m_oldTenses["PrPe"] =  "Present Perfect" ;
    m_oldTenses["PaSi"] =  "Simple Past" ;
    m_oldTenses["PaPr"] =  "Past Progressive" ;
    m_oldTenses["PaPa"] =  "Past Participle" ;
    m_oldTenses["FuSi"] =  "Future" ;
}


void QmVocKvtmlCompability::addUserdefinedTense(const QString & tense)
{
    m_userdefinedTenseCounter++;
    m_oldTenses[KVTML_1_USER_DEFINED + QString::number( m_userdefinedTenseCounter )] = tense;
    m_tenses.insert(tense);

    qDebug() << " Add tense: " << KVTML_1_USER_DEFINED + QString::number( m_userdefinedTenseCounter ) << " - " << tense;
}


QString QmVocKvtmlCompability::tenseFromKvtml1(const QString & oldTense)
{
    // in case the document got chaged, at least make up something as tense
    if (!m_oldTenses.keys().contains(oldTense)) {
        m_oldTenses[oldTense] = oldTense;
        qDebug() << "Warning, tense " << oldTense << " not found in document!";
    }
    m_tenses.insert(m_oldTenses.value(oldTense));
    return m_oldTenses.value(oldTense);
}


QStringList QmVocKvtmlCompability::documentTenses() const
{
    return m_tenses.values();
}


QString QmVocKvtmlCompability::oldTense(const QString & tense)
{
///@todo writing of the user defined tenses is probably messed up
    if ( !m_oldTenses.values().contains(tense) ) {
        m_userdefinedTenseCounter++;
        m_oldTenses[KVTML_1_USER_DEFINED + QString::number( m_userdefinedTenseCounter )] = tense;
    }
    return m_oldTenses.key(tense);
}

void QmVocKvtmlCompability::setupWordTypes(QmVocWordType * parent)
{
    QStringList wordTypeNames;
    wordTypeNames
        <<  "Verb"  // 0
        <<  "Noun"  // 1
        <<  "Name" 
        <<  "Article"  // 3
        <<  "Adjective"  // 4
        <<  "Adverb"  // 5
        <<  "Pronoun"  // 6
        <<  "Phrase" 
        <<  "Numeral"  // 8
        <<  "Conjunction" 
        <<  "Preposition" 
        <<  "Question" ;

    foreach (const QString &typeName, wordTypeNames) {
        QmVocWordType* wordType = new QmVocWordType(typeName, parent);
        parent->appendChildContainer(wordType);
        m_userdefinedTypeCounter++;
    }
    static_cast<QmVocWordType*>(parent->childContainer(4))->setWordType(QmVocWordFlag::Adjective);
    static_cast<QmVocWordType*>(parent->childContainer(5))->setWordType(QmVocWordFlag::Adverb);

    QmVocWordType* numeral = static_cast<QmVocWordType*>(parent->childContainer(8));
    QmVocWordType* wordType = new QmVocWordType( "Ordinal", numeral);
    wordType->setWordType(QmVocWordFlag::Adjective);
    numeral->appendChildContainer(wordType);
    wordType = new QmVocWordType( "Cardinal" , numeral);

    wordType->setWordType(QmVocWordFlag::Adjective);
    numeral->appendChildContainer(wordType);

    QmVocWordType* article = static_cast<QmVocWordType*>(parent->childContainer(3));
    wordType = new QmVocWordType( "Definite" , article);
    wordType->setWordType(QmVocWordFlag::Article | QmVocWordFlag::Definite);
    article->appendChildContainer(wordType);
    wordType = new QmVocWordType( "Indefinite" , article);
    wordType->setWordType(QmVocWordFlag::Article | QmVocWordFlag::Indefinite);
    article->appendChildContainer(wordType);

    QmVocWordType* verb = static_cast<QmVocWordType*>(parent->childContainer(0));
    verb->setWordType(QmVocWordFlag::Verb);
    wordType = new QmVocWordType( "Regular" , verb);
    wordType->setWordType(QmVocWordFlag::Verb | QmVocWordFlag::Regular);
    verb->appendChildContainer(wordType);
    wordType = new QmVocWordType( "Irregular" , verb);
    verb->appendChildContainer(wordType);
    wordType->setWordType(QmVocWordFlag::Verb | QmVocWordFlag::Irregular);

    QmVocWordType* noun = static_cast<QmVocWordType*>(parent->childContainer(1));
    noun->setWordType(QmVocWordFlag::Noun);
    wordType = new QmVocWordType( "Male" , noun);
    noun->appendChildContainer(wordType);
    wordType->setWordType(QmVocWordFlag::Noun | QmVocWordFlag::Masculine);
    wordType = new QmVocWordType( "Female" , noun);
    noun->appendChildContainer(wordType);
    wordType->setWordType(QmVocWordFlag::Noun | QmVocWordFlag::Feminine);
    wordType = new QmVocWordType( "Neutral" , noun);
    noun->appendChildContainer(wordType);
    wordType->setWordType(QmVocWordFlag::Noun | QmVocWordFlag::Neuter);


    QmVocWordType* pronoun = static_cast<QmVocWordType*>(parent->childContainer(6));
    wordType = new QmVocWordType( "Possessive" , pronoun);
    wordType->setWordType(QmVocWordFlag::Pronoun);
    pronoun->appendChildContainer(wordType);
    wordType = new QmVocWordType( "Personal" , pronoun);
    wordType->setWordType(QmVocWordFlag::Pronoun);
    pronoun->appendChildContainer(wordType);
}

