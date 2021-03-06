/***************************************************************************
*   this file is from kdeedu project. Filename: keduvockvtmlreader.cpp
***************************************************************************/

/***************************************************************************
                     read a QmVocDocument from a KVTML file
    -----------------------------------------------------------------------
    copyright           : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

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

#include "qmvockvtmlreader.h"

#include <QtCore/QTextStream>
#include <QtCore/QList>
#include <QtCore/QIODevice>

// #include <kdebug.h>
#include <QDebug>
// #include <klocale.h>
// #include <kglobal.h>

#include "qmvocdocument.h"
#include "qmvoclesson.h"
#include "qmvocwordtype.h"
#include "kvtmldefs.h"
#include "qmvoccommon_p.h"

QmVocKvtmlReader::QmVocKvtmlReader( QIODevice *file )
{
    // the file must be already open
    m_inputFile = file;
    m_errorMessage = "";
    qDebug() << "QmVocKvtmlReader for kvtml version 1 files started.";
}


bool QmVocKvtmlReader::readDoc( QmVocDocument *doc )
{
    m_doc = doc;
    m_cols = 0;
    m_lines = 0;

    QDomDocument domDoc( "KEduVocDocument" );

    if ( !domDoc.setContent( m_inputFile, &m_errorMessage ) )
        return false;

    QDomElement domElementKvtml = domDoc.documentElement();
    if ( domElementKvtml.tagName() != KV_DOCTYPE ) {
        m_errorMessage =  "This is not a KDE Vocabulary document." ;
        return false;
    }

    //-------------------------------------------------------------------------
    // Attributes
    //-------------------------------------------------------------------------

    QDomAttr documentAttribute;
    documentAttribute = domElementKvtml.attributeNode( KV_ENCODING );
    if ( !documentAttribute.isNull() ) {
        // TODO handle old encodings
        // Qt DOM API autodetects encoding, so is there anything to do ?
    }

    documentAttribute = domElementKvtml.attributeNode( KV_TITLE );
    if ( !documentAttribute.isNull() )
        m_doc->setTitle( documentAttribute.value() );

    documentAttribute = domElementKvtml.attributeNode( KV_AUTHOR );
    if ( !documentAttribute.isNull() )
        m_doc->setAuthor( documentAttribute.value() );

    documentAttribute = domElementKvtml.attributeNode( KV_LICENSE );
    if ( !documentAttribute.isNull() )
        m_doc->setLicense( documentAttribute.value() );

    documentAttribute = domElementKvtml.attributeNode( KV_DOC_REM );
    if ( !documentAttribute.isNull() )
        m_doc->setDocumentComment( documentAttribute.value() );

    documentAttribute = domElementKvtml.attributeNode( KV_GENERATOR );
    if ( !documentAttribute.isNull() ) {
        m_doc->setGenerator( documentAttribute.value() );
        int pos = m_doc->generator().lastIndexOf( KVD_VERS_PREFIX );
        if ( pos >= 0 )
            m_doc->setVersion( m_doc->generator().remove( 0, pos + 2 ) );
    }

    documentAttribute = domElementKvtml.attributeNode( KV_COLS );
    if ( !documentAttribute.isNull() )
        m_cols = documentAttribute.value().toInt(); ///currently not used anywhere

    documentAttribute = domElementKvtml.attributeNode( KV_LINES );
    if ( !documentAttribute.isNull() )
        m_lines = documentAttribute.value().toInt();

    //-------------------------------------------------------------------------
    // Children
    //-------------------------------------------------------------------------

    bool result = readBody( domElementKvtml ); // read vocabulary

    return result;
}


bool QmVocKvtmlReader::readBody( QDomElement &domElementParent )
{
    bool result = false;

    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KV_LESS_GRP );
    if ( !currentElement.isNull() ) {
        result = readLesson( currentElement );
        if ( !result )
            return false;
    }

    currentElement = domElementParent.firstChildElement( KV_ARTICLE_GRP );
    if ( !currentElement.isNull() ) {
        result = readArticle( currentElement );
        if ( !result )
            return false;
    }

    currentElement = domElementParent.firstChildElement( KV_CONJUG_GRP );
    if ( !currentElement.isNull() ) {
        int count = 0;

        QDomElement domElementConjugChild = currentElement.firstChildElement(KV_CON_ENTRY);
        while ( !domElementConjugChild.isNull() ) {
            QString lang;
            QDomAttr domAttrLang = domElementConjugChild.attributeNode( KV_LANG ); // "l"
            // make sure, the identifier is there
            if (!addLanguage(count, domAttrLang.value())) {
                return false;
            }

            QmVocPersonalPronoun pronouns;
            if (! readPersonalPronouns( domElementConjugChild, pronouns ) ) {
                return false;
            }
            m_doc->identifier(count).setPersonalPronouns( pronouns );

            count ++;

            domElementConjugChild = domElementConjugChild.nextSiblingElement( KV_CON_ENTRY );
        }
    }

    // initialize the list of predefined types
    m_compability.setupWordTypes(m_doc->wordTypeContainer());

    currentElement = domElementParent.firstChildElement( KV_TYPE_GRP );
    if ( !currentElement.isNull() ) {
        result = readType( currentElement );
        if ( !result )
            return false;
    }

    currentElement = domElementParent.firstChildElement( KV_TENSE_GRP );
    if ( !currentElement.isNull() ) {
        result = readTense( currentElement );
        if ( !result )
            return false;
    }

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_EXPR );
    if ( entryList.length() <= 0 )
        return false;

    for ( int i = 0; i < entryList.count(); ++i ) {
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            result = readExpression( currentElement );
            if ( !result )
                return false;
        }
    }

    for(int i = 0; i < m_doc->identifierCount(); i++) {
        m_doc->identifier(i).setTenseList(m_compability.documentTenses());
    }

    return true;
}


bool QmVocKvtmlReader::readLesson( QDomElement &domElementParent )
{
    QString s;
    QDomAttr attribute;
    QDomElement currentElement;


    //-------------------------------------------------------------------------
    // Children
    //-------------------------------------------------------------------------

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_LESS_DESC );
    if ( entryList.length() <= 0 )
        return false;

    for ( int i = 0; i < entryList.count(); ++i ) {
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            int no = -1;

            attribute = currentElement.attributeNode( KV_LESS_NO );
            if ( !attribute.isNull() ) {
                no = attribute.value().toInt();
            }

            bool inQuery = false;
            attribute = currentElement.attributeNode( KV_LESS_QUERY );
            if ( !attribute.isNull() ) {
                inQuery = (attribute.value().toInt() != 0);
            }

            s = currentElement.text();
            QmVocLesson* lesson = new QmVocLesson(s, m_doc->lesson());
            lesson->setInPractice(inQuery);
            m_doc->lesson()->appendChildContainer( lesson );
            if ( m_doc->lesson()->childContainerCount() != no-1 ) {
                qDebug() << "Warning! Lesson order may be confused. Are all lessons in order in the file?";
            }
        }
    }

    return true;
}


bool QmVocKvtmlReader::readArticle( QDomElement &domElementParent )
/*
 <article>
  <e l="de">        lang determines also lang order in entries !!
   <fi>eine</fi>    which must NOT differ
   <fd>die</fd>
   <mi>ein</mi>
   <md>der</md>
   <ni>ein</ni>
   <nd>das</nd>
  </e>
 </article>
*/
{

    QString s;
    QDomAttr attribute;
    QDomElement currentElement;
    QDomElement article;

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_ART_ENTRY );
    if ( entryList.length() <= 0 )
        return false;

    for ( int i = 0; i < entryList.count(); ++i ) {

//kDebug() << "QmVocKvtmlReader::readArticle() read " << entryList.count() << " articles. ";
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            QString lang;
            attribute = currentElement.attributeNode( KV_LANG );

            if (!addLanguage(i, attribute.value())) {
                return false;
            }

            //---------
            // Children

            QString fem_def = "";
            QString mal_def = "";
            QString nat_def = "";
            QString fem_indef = "";
            QString mal_indef = "";
            QString nat_indef = "";

            article = currentElement.firstChildElement( KV_ART_FD );
            if ( !article.isNull() ) {
                fem_def = article.text();
                if ( fem_def.isNull() )
                    fem_def = "";
            }

            article = currentElement.firstChildElement( KV_ART_FI );
            if ( !article.isNull() ) {
                fem_indef = article.text();
                if ( fem_indef.isNull() )
                    fem_indef = "";
            }

            article = currentElement.firstChildElement( KV_ART_MD );
            if ( !article.isNull() ) {
                mal_def = article.text();
                if ( mal_def.isNull() )
                    mal_def = "";
            }

            article = currentElement.firstChildElement( KV_ART_MI );
            if ( !article.isNull() ) {
                mal_indef = article.text();
                if ( mal_indef.isNull() )
                    mal_indef = "";
            }

            article = currentElement.firstChildElement( KV_ART_ND );
            if ( !article.isNull() ) {
                nat_def = article.text();
                if ( nat_def.isNull() )
                    nat_def = "";
            }

            article = currentElement.firstChildElement( KV_ART_NI );
            if ( !article.isNull() ) {
                nat_indef = article.text();
                if ( nat_indef.isNull() )
                    nat_indef = "";
            }

            m_doc->identifier(i).setArticle( QmVocArticle( fem_def, fem_indef, mal_def, mal_indef, nat_def, nat_indef ) );
        }
    }

    return true;
}


bool QmVocKvtmlReader::readTranslationConjugations( QDomElement &domElementParent, QmVocTranslation* translation )
{
    QString tense;

    QDomElement domElementConjugChild = domElementParent.firstChildElement(KV_CON_TYPE);
    while ( !domElementConjugChild.isNull() )
    {
        // "n" == is the type is the tense
        QDomAttr domAttrLang = domElementConjugChild.attributeNode( KV_CON_NAME );
        QString oldShortTense = domAttrLang.value();

        tense = m_compability.tenseFromKvtml1( oldShortTense );
        QmVocConjugation conjugation;
        readConjugation(domElementConjugChild, conjugation);
        translation->setConjugation(tense, conjugation);

        domElementConjugChild = domElementConjugChild.nextSiblingElement( KV_CON_TYPE );
    } // while -> next tense, count++
    return true;
}

bool QmVocKvtmlReader::readConjugation( QDomElement &domElementParent, QmVocConjugation& conjugation )
/*
 <conjugation>        used in header for definiton of "prefix"
  <e l="de">          lang determines also lang order in entries !!
   <s1>I</s1>         which must NOT differ
   <s2>you<2>
   <s3f>he</s3f>
   <s3m>she</s3m>
   <s3n>it</s3n>
   <p1>we</p1>
   <p2>you</p2>
   <p3f>they</p3f>
   <p3m>they</p3m>
   <p3n>they</p3n>
  </e>
 </conjugation>

 <conjugation>        and in entry for definition of tenses of (irreg.) verbs
  <t n="sipa">
   <s1>go</s1>
   <s2>go</s2>
   <s3f>goes</s3f>
   <s3m>goes</s3m>
   <s3n>goes</s3n>
   <p1>go</p1>
   <p2>go</p2>
   <p3f>go</p3f>
   <p3m>go</p3m>
   <p3n>go</p3n>
  </t>
 </conjugation>
*/
{
//     QString s;
    bool p3_common;
    bool s3_common;
    QString pers1_sing;
    QString pers2_sing;
    QString pers3_m_sing;
    QString pers3_f_sing;
    QString pers3_n_sing;
    QString pers1_plur;
    QString pers2_plur;
    QString pers3_m_plur;
    QString pers3_f_plur;
    QString pers3_n_plur;

    p3_common = false;
    s3_common = false;

    // get the individual entries for persons...
    QDomElement domElementConjugGrandChild = domElementParent.firstChild().toElement();
    while ( !domElementConjugGrandChild.isNull() ) {
        if ( domElementConjugGrandChild.tagName() == KV_CON_P1S ) {
            pers1_sing = domElementConjugGrandChild.text();
        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P2S ) {
            pers2_sing = domElementConjugGrandChild.text();
        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SF ) {
            QDomAttr domAttrCommon = domElementConjugGrandChild.attributeNode( KV_CONJ_COMMON );
            if ( !domAttrCommon.isNull() )
                s3_common = domAttrCommon.value().toInt();  // returns 0 if the conversion fails
            pers3_f_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SM ) {
            pers3_m_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SN ) {
            pers3_n_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P1P ) {
            pers1_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P2P ) {
            pers2_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PF ) {
            QDomAttr domAttrCommon = domElementConjugGrandChild.attributeNode( KV_CONJ_COMMON );
            if ( !domAttrCommon.isNull() )
                p3_common = domAttrCommon.value().toInt();  // returns 0 if the conversion fails

            pers3_f_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PM ) {
            pers3_m_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PN ) {
            pers3_n_plur = domElementConjugGrandChild.text();

        } else {
            return false;
        }

        domElementConjugGrandChild = domElementConjugGrandChild.nextSibling().toElement();
    } // while - probably to be sure, because the persons could be in any order.
    // I guess this goes over only one set, such as:
    // <s1>traigo</s1><s2>traes</s2><s3fcommon="1">trae</s3f>
    // <p1>traemos</p1><p2>traÃÂ©is</p2><p3f common="1">traen</p3f>
    // until no elements are left in that soup.

    // now set the data: [count] - number of conjug?
    // type - the tense?
    // finally the person

    const QmVocWordFlags numS = QmVocWordFlag::Singular;
    const QmVocWordFlags numP = QmVocWordFlag::Plural;

    conjugation.setConjugation( pers1_sing, QmVocWordFlag::First | numS);
    conjugation.setConjugation( pers2_sing, QmVocWordFlag::Second | numS);
    conjugation.setConjugation( pers1_plur, QmVocWordFlag::First | numP);
    conjugation.setConjugation( pers2_plur, QmVocWordFlag::Second | numP);

    if ( s3_common ) {
        conjugation.setConjugation( pers3_f_sing, QmVocWordFlag::Third | QmVocWordFlag::Neuter | QmVocWordFlag::Singular );
    } else  {
        conjugation.setConjugation( pers3_m_sing,
            QmVocWordFlag::Third | QmVocWordFlag::Masculine | QmVocWordFlag::Singular );
        conjugation.setConjugation( pers3_f_sing,
            QmVocWordFlag::Third | QmVocWordFlag::Feminine | QmVocWordFlag::Singular );
        conjugation.setConjugation( pers3_n_sing,
            QmVocWordFlag::Third | QmVocWordFlag::Neuter |  QmVocWordFlag::Singular );
    }

    if ( p3_common ) {
        conjugation.setConjugation( pers3_f_plur, QmVocWordFlag::Third | QmVocWordFlag::Neuter | QmVocWordFlag::Plural );
    } else  {
        conjugation.setConjugation( pers3_m_plur,
            QmVocWordFlag::Third | QmVocWordFlag::Masculine | QmVocWordFlag::Plural );
        conjugation.setConjugation( pers3_f_plur,
            QmVocWordFlag::Third | QmVocWordFlag::Feminine | QmVocWordFlag::Plural );
        conjugation.setConjugation( pers3_n_plur,
            QmVocWordFlag::Third | QmVocWordFlag::Neuter | QmVocWordFlag::Plural );
    }

    return true;
}




bool QmVocKvtmlReader::readPersonalPronouns( QDomElement &domElementParent, QmVocPersonalPronoun& pronouns )
{
//     QString s;
    bool p3_common;
    bool s3_common;
    QString pers1_sing;
    QString pers2_sing;
    QString pers3_m_sing;
    QString pers3_f_sing;
    QString pers3_n_sing;
    QString pers1_plur;
    QString pers2_plur;
    QString pers3_m_plur;
    QString pers3_f_plur;
    QString pers3_n_plur;

    p3_common = false;
    s3_common = false;

    // get the individual entries for persons...
    QDomElement domElementConjugGrandChild = domElementParent.firstChild().toElement();
    while ( !domElementConjugGrandChild.isNull() ) {
        if ( domElementConjugGrandChild.tagName() == KV_CON_P1S ) {
            pers1_sing = domElementConjugGrandChild.text();
        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P2S ) {
            pers2_sing = domElementConjugGrandChild.text();
        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SF ) {
            QDomAttr domAttrCommon = domElementConjugGrandChild.attributeNode( KV_CONJ_COMMON );
            if ( !domAttrCommon.isNull() )
                s3_common = domAttrCommon.value().toInt();  // returns 0 if the conversion fails
            pers3_f_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SM ) {
            pers3_m_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3SN ) {
            pers3_n_sing = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P1P ) {
            pers1_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P2P ) {
            pers2_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PF ) {
            QDomAttr domAttrCommon = domElementConjugGrandChild.attributeNode( KV_CONJ_COMMON );
            if ( !domAttrCommon.isNull() )
                p3_common = domAttrCommon.value().toInt();  // returns 0 if the conversion fails

            pers3_f_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PM ) {
            pers3_m_plur = domElementConjugGrandChild.text();

        } else if ( domElementConjugGrandChild.tagName() == KV_CON_P3PN ) {
            pers3_n_plur = domElementConjugGrandChild.text();

        } else {
            return false;
        }

        domElementConjugGrandChild = domElementConjugGrandChild.nextSibling().toElement();
    } // while - probably to be sure, because the persons could be in any order.
    // I guess this goes over only one set, such as:
    // <s1>traigo</s1><s2>traes</s2><s3fcommon="1">trae</s3f>
    // <p1>traemos</p1><p2>traÃÂ©is</p2><p3f common="1">traen</p3f>
    // until no elements are left in that soup.

    // now set the data: [count] - number of conjug?
    // type - the tense?
    // finally the person

    QmVocWordFlags numS = QmVocWordFlag::Singular;
    pronouns.setMaleFemaleDifferent(false);
    pronouns.setPersonalPronoun( pers1_sing, QmVocWordFlag::First | numS );
    pronouns.setPersonalPronoun( pers2_sing, QmVocWordFlag::Second | numS );

    // used to have common in female
    if ( s3_common ) {
        pronouns.setPersonalPronoun( pers3_f_sing, QmVocWordFlag::Third | QmVocWordFlag::Neuter | numS );
    } else  {
        pronouns.setPersonalPronoun( pers3_m_sing,
            QmVocWordFlag::Third | QmVocWordFlag::Masculine | numS );
        pronouns.setPersonalPronoun( pers3_f_sing,
            QmVocWordFlag::Third | QmVocWordFlag::Feminine | numS );
        pronouns.setPersonalPronoun( pers3_n_sing,
            QmVocWordFlag::Third | QmVocWordFlag::Neuter | numS );
        pronouns.setMaleFemaleDifferent(true);
    }

    QmVocWordFlags numP = QmVocWordFlag::Plural;

    pronouns.setPersonalPronoun( pers1_plur, QmVocWordFlag::First | numP );
    pronouns.setPersonalPronoun( pers2_plur, QmVocWordFlag::Second | numP );
    if ( p3_common ) {
        pronouns.setPersonalPronoun( pers3_f_plur, QmVocWordFlag::Third | QmVocWordFlag::Neuter | numP );
    } else  {
        pronouns.setPersonalPronoun( pers3_m_plur,
            QmVocWordFlag::Third | QmVocWordFlag::Masculine | numP );
        pronouns.setPersonalPronoun( pers3_f_plur,
            QmVocWordFlag::Third | QmVocWordFlag::Feminine | numP );
        pronouns.setPersonalPronoun( pers3_n_plur,
            QmVocWordFlag::Third | QmVocWordFlag::Neuter | numP );
        pronouns.setMaleFemaleDifferent(true);
    }

    return true;
}


bool QmVocKvtmlReader::readType( QDomElement &domElementParent )
{
    QString s;
    QDomElement currentElement;

    QDomNodeList entryList = domElementParent.elementsByTagName( KV_TYPE_DESC );
    if ( entryList.length() <= 0 )
        return false;

    for ( int i = 0; i < entryList.count(); ++i ) {
        currentElement = entryList.item( i ).toElement();
        if ( currentElement.parentNode() == domElementParent ) {
            // We need to even add empty elements since the old system relied on
            // the order. So "type1" "" "type2" should be just like that.

            qDebug() << "Adding old self defined type: " << currentElement.text();
            // add the type to the list of available types
            QmVocWordType* type = new QmVocWordType(currentElement.text(), m_doc->wordTypeContainer());
            m_doc->wordTypeContainer()->appendChildContainer( type );

            // from this the #1 are transformed to something sensible again
            m_oldSelfDefinedTypes.append( currentElement.text() );
        }
    }

    return true;
}


bool QmVocKvtmlReader::readTense( QDomElement &domElementParent )
{
    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KV_TENSE_DESC );
    while ( !currentElement.isNull() ) {
        qDebug() << "Reading user defined tense description: " << currentElement.text();
        m_compability.addUserdefinedTense( currentElement.text() );
        currentElement = currentElement.nextSiblingElement( KV_TENSE_DESC );
    }
    return true;
}


bool QmVocKvtmlReader::readComparison( QDomElement &domElementParent, QmVocTranslation * translation )
/*
 <comparison>
   <l1>good</l1> --- this one is dead as it always has to be the word itself
   <l2>better</l2>
   <l3>best</l3>
 </comparison>
*/
{
    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KV_COMP_L2 );
    translation->setComparative(currentElement.text());

    currentElement = domElementParent.firstChildElement( KV_COMP_L3 );
    translation->setSuperlative(currentElement.text());

    return true;
}


bool QmVocKvtmlReader::readMultipleChoice( QDomElement &domElementParent, QmVocTranslation* translation )
/*
 <multiplechoice>
   <mc1>good</mc1>
   <mc2>better</mc2>
   <mc3>best</mc3>
   <mc4>best 2</mc4>
   <mc5>best 3</mc5>
 </multiplechoice>
*/

{
    QDomElement currentElement;

    currentElement = domElementParent.firstChildElement( KV_MC_1 );
    if ( !currentElement.isNull() ) {
        translation->multipleChoice().append( currentElement.text() );
    }

    currentElement = domElementParent.firstChildElement( KV_MC_2 );
    if ( !currentElement.isNull() ) {
        translation->multipleChoice().append( currentElement.text() );
    }

    currentElement = domElementParent.firstChildElement( KV_MC_3 );
    if ( !currentElement.isNull() ) {
        translation->multipleChoice().append( currentElement.text() );
    }

    currentElement = domElementParent.firstChildElement( KV_MC_4 );
    if ( !currentElement.isNull() ) {
        translation->multipleChoice().append( currentElement.text() );
    }

    currentElement = domElementParent.firstChildElement( KV_MC_5 );
    if ( !currentElement.isNull() ) {
        translation->multipleChoice().append( currentElement.text() );
    }

    return true;
}


bool QmVocKvtmlReader::readExpressionChildAttributes( QDomElement &domElementExpressionChild,
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
        QSet<QString> &usages,
        QString &paraphrase )
{
    Q_UNUSED(usages)
    int pos;
    QDomAttr attribute;

    lang = "";
    attribute = domElementExpressionChild.attributeNode( KV_LANG );
    if ( !attribute.isNull() )
        lang = attribute.value();

    width = -1;
    attribute = domElementExpressionChild.attributeNode( KV_SIZEHINT );
    if ( !attribute.isNull() )
        width = attribute.value().toInt();

    grade = KV_NORM_GRADE;
    rev_grade = KV_NORM_GRADE;
    attribute = domElementExpressionChild.attributeNode( KV_GRADE );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            grade = s.left( pos ).toInt();
            rev_grade = s.mid( pos + 1, s.length() ).toInt();
        } else
            grade = s.toInt();
    }

    count = 0;
    rev_count = 0;
    attribute = domElementExpressionChild.attributeNode( KV_COUNT );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            count = s.left( pos ).toInt();
            rev_count = s.mid( pos + 1, s.length() ).toInt();
        } else
            count = s.toInt();
    }

    bcount = 0;
    rev_bcount = 0;
    attribute = domElementExpressionChild.attributeNode( KV_BAD );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            bcount = s.left( pos ).toInt();
            rev_bcount = s.mid( pos + 1, s.length() ).toInt();
        } else
            bcount = s.toInt();
    }

    date.setTime_t( 0 );
    rev_date.setTime_t( 0 );
    attribute = domElementExpressionChild.attributeNode( KV_DATE );
    if ( !attribute.isNull() ) {
        QString s = attribute.value();
        if (( pos = s.indexOf( ';' ) ) >= 1 ) {
            date.setTime_t( s.left( pos ).toInt() );
            rev_date.setTime_t( s.mid( pos + 1, s.length() ).toInt() );
        } else
            date.setTime_t( s.toInt() );
    }

    attribute = domElementExpressionChild.attributeNode( KV_DATE2 );
    if ( !attribute.isNull() ) {
        //this format is deprecated and ignored.
    }

    remark = "";
    attribute = domElementExpressionChild.attributeNode( KV_REMARK );
    if ( !attribute.isNull() )
        remark = attribute.value();

    faux_ami_f = "";
    attribute = domElementExpressionChild.attributeNode( KV_FAUX_AMI_F );
    if ( !attribute.isNull() )
        faux_ami_f = attribute.value();

    faux_ami_t = "";
    attribute = domElementExpressionChild.attributeNode( KV_FAUX_AMI_T );
    if ( !attribute.isNull() )
        faux_ami_t = attribute.value();

    synonym = "";
    attribute = domElementExpressionChild.attributeNode( KV_SYNONYM );
    if ( !attribute.isNull() )
        synonym = attribute.value();

    example = "";
    attribute = domElementExpressionChild.attributeNode( KV_EXAMPLE );
    if ( !attribute.isNull() )
        example = attribute.value();

    paraphrase = "";
    attribute = domElementExpressionChild.attributeNode( KV_PARAPHRASE );
    if ( !attribute.isNull() )
        paraphrase = attribute.value();

    antonym = "";
    attribute = domElementExpressionChild.attributeNode( KV_ANTONYM );
    if ( !attribute.isNull() )
        antonym = attribute.value();

    // this is all done by reference - so we have to care about "type" :(
    attribute = domElementExpressionChild.attributeNode( KV_EXPRTYPE );
    if ( !attribute.isNull() ) {
        type = attribute.value();
    }

    pronunciation = "";
    attribute = domElementExpressionChild.attributeNode( KV_PRONUNCE );
    if ( !attribute.isNull() )
        pronunciation = attribute.value();

    query_id = "";
    attribute = domElementExpressionChild.attributeNode( KV_QUERY );
    if ( !attribute.isNull() )
        query_id = attribute.value();

    return true;
}


bool QmVocKvtmlReader::readExpression( QDomElement &domElementParent )
{
    grade_t                   grade;
    grade_t                   r_grade;
    int                       qcount;
    int                       r_qcount;
    int                       bcount;
    int                       r_bcount;
    QString                   remark;
    QString                   pronunciation;
    QDateTime                 qdate;
    QDateTime                 r_qdate;
    bool                      inquery;
    bool                      active;
    QString                   lang;
    QString                   textstr;
    QString                   q_org;
    QString                   q_trans;
    QString                   query_id;
    int                       width;
    QString                   type;
    QString                   faux_ami_f;
    QString                   faux_ami_t;
    QString                   synonym;
    QString                   example;
    QString                   antonym;
    QSet<QString>             usage;
    QString                   paraphrase;

    QDomAttr                  attribute;
    QDomElement               currentElement;
    QDomElement               currentChild;

    int lessonNumber = -1;

    //-------------------------------------------------------------------------
    // Attributes
    //-------------------------------------------------------------------------

    attribute = domElementParent.attributeNode( KV_LESS_MEMBER );
    if ( !attribute.isNull() ) {
        // we start conting from 0 in new documents
        lessonNumber = attribute.value().toInt() - 1;
        if ( lessonNumber > m_doc->lesson()->childContainerCount() ) {
            ///@todo can this happen? does it need a while loop?
            // it's from a lesson that hasn't been added yet
            // so make sure this lesson is in the document
            qDebug() << "Warning: lesson > m_doc->lessonCount() in readExpression.";

            QmVocLesson* lesson = new QmVocLesson( QString("Lesson %1").arg(lessonNumber) , m_doc->lesson());
            m_doc->lesson()->appendChildContainer(lesson);
        }
    }

    attribute = domElementParent.attributeNode( KV_SELECTED );
    if ( !attribute.isNull() )
        inquery = attribute.value() == "1" ? true : false;
    else
        inquery = false;

    attribute = domElementParent.attributeNode( KV_INACTIVE );
    if ( !attribute.isNull() )
        active = attribute.value() == "1" ? false : true;
    else
        active = true;

    // this is all done by reference - so we have to care about "type" :(
    attribute = domElementParent.attributeNode( KV_EXPRTYPE );
    if ( !attribute.isNull() ) {
        type = attribute.value();
    }






    //-------------------------------------------------------------------------
    // Children 'Translation'
    //-------------------------------------------------------------------------

    //QDomNodeList translationList = domElementParent.elementsByTagName(KV_TRANS);

    // count which translation we are on
    int i=0;

    // kvtml 1: we always have an original element (required)
    currentElement = domElementParent.firstChildElement( KV_ORG );
    if ( currentElement.isNull() ) { // sanity check
        m_errorMessage =  "Data for original language missing" ;
        return false;
    }

    QmVocExpression* entry = 0;

    while ( !currentElement.isNull() ) {

        //-----------
        // Attributes
        //-----------

        // read attributes - the order of the query grades is interchanged!
        if ( i == 0 && !readExpressionChildAttributes( currentElement, lang, grade, r_grade, qcount, r_qcount, qdate, r_qdate, remark, bcount, r_bcount, query_id,
                pronunciation, width, type, faux_ami_t, faux_ami_f, synonym, example, antonym, usage, paraphrase ) ) {
            return false;
        }

        if ( i != 0 && !readExpressionChildAttributes( currentElement, lang, grade, r_grade, qcount, r_qcount, qdate, r_qdate, remark, bcount, r_bcount, query_id,
                pronunciation, width, type, faux_ami_f, faux_ami_t, synonym, example, antonym, usage, paraphrase ) ) {
            return false;
        }


        //---------
        // Children

        textstr = currentElement.lastChild().toText().data();

        if ( i == 0 ) {
            entry = new QmVocExpression( textstr );
            entry->setActive( active );
            if ( lessonNumber != -1 ) {
                static_cast<QmVocLesson*>(m_doc->lesson()->childContainer(lessonNumber))->appendEntry(entry);
            } else {
                m_doc->lesson()->appendEntry(entry);
            }
        } else {
            entry->setTranslation( i, textstr );
        }

        if ( m_doc->lesson()->entries(QmVocLesson::Recursive).count() == 1 ) { // this is because in kvtml the languages are saved in the FIRST ENTRY ONLY.
            // new translation
            if (!addLanguage(i, lang)) {
                return false;
            }
        }

        // better make sure, translation(i) already exists...
        currentChild = currentElement.firstChildElement( KV_CONJUG_GRP );
        if ( !currentChild.isNull() ) {
            if ( !readTranslationConjugations( currentChild, entry->translation(i) ) ) {
                return false;
            }
        }

        currentChild = currentElement.firstChildElement( KV_MULTIPLECHOICE_GRP );
        if ( !currentChild.isNull() ) {
            if ( !readMultipleChoice( currentChild, entry->translation(i) ) ) {
                return false;
            }
        }

        currentChild = currentElement.firstChildElement( KV_COMPARISON_GRP );
        if ( !currentChild.isNull() ) {
            if ( !readComparison( currentChild, entry->translation(i) ) ) {
                return false;
            }
        }

        if ( !type.isEmpty() ) {
            QmVocWordType* wordType = m_compability.typeFromOldFormat(m_doc->wordTypeContainer(), type);
            entry->translation(i)->setWordType(wordType);
        }

        if ( !remark.isEmpty() )
            entry->translation( i )->setComment( remark );
        if ( !pronunciation.isEmpty() )
            entry->translation( i )->setPronunciation( pronunciation );

        ///@todo include false friends from kvtml-1 again?
//         if ( !faux_ami_f.isEmpty() )
//             entry->translation( i )->setFalseFriend( 0, faux_ami_f );
//         if ( !faux_ami_t.isEmpty() )
//             entry->translation( 0 )->setFalseFriend( i, faux_ami_t );
        ///@todo include synonyms from kvtml-1 again?
//         if ( !synonym.isEmpty() )
//             entry->translation( i )->setSynonym( synonym );
//         if ( !antonym.isEmpty() )
//             entry->translation( i )->setAntonym( antonym );

        if ( !example.isEmpty() )
            entry->translation( i )->setExample( example );
        if ( !paraphrase.isEmpty() )
            entry->translation( i )->setParaphrase( paraphrase );

        if ( i != 0 ) {
            entry->translation( i )->setGrade( grade );
            entry->translation( 0 )->setGrade( r_grade );
            entry->translation( i )->setPracticeCount( qcount );
            entry->translation( 0 )->setPracticeCount( r_qcount );
            entry->translation( i )->setBadCount( bcount );
            entry->translation( 0 )->setBadCount( r_bcount );
            entry->translation( i )->setPracticeDate( qdate );
            entry->translation( 0 )->setPracticeDate( r_qdate );
        }

        // Next translation
        currentElement = currentElement.nextSiblingElement( KV_TRANS );
        i++;
    }

    return true;
}


bool QmVocKvtmlReader::addLanguage( int languageId, const QString& locale)
{
    if ( m_doc->identifierCount() <= languageId ) {
        m_doc->appendIdentifier();
        // first entry
        if ( !locale.isEmpty() ) {      // no definition in first entry
            m_doc->identifier(languageId).setLocale(locale);

            QString languageName;
//             if (KGlobal::locale()) {
//                 // when using from qt-only apps this would crash (converter)
//                 languageName = KGlobal::locale()->languageCodeToName(locale);
//             }
            if ( languageName.isEmpty() ) {
                languageName = locale;
            }

            m_doc->identifier(languageId).setName(languageName);
            qDebug() << "addLanguage( " << languageId << ", " << locale << "): " << languageName;

        }
    } else {
        if ( !locale.isEmpty() ) {
            if ( locale != m_doc->identifier(languageId).locale() ) {
                // different originals ?
                m_errorMessage =  "Ambiguous definition of language code" ;
                return false;
            }
        }
    }
    return true;
}


