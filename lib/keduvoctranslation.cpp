/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoctranslation.cpp
***************************************************************************/

/***************************************************************************
                        Vocabulary Expression Translation for KDE Edu
    -----------------------------------------------------------------------

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


#include "keduvoctranslation.h"

#include "keduvocdeclension.h"
#include "keduvocwordtype.h"
#include "keduvocleitnerbox.h"
#include "kvtml2defs.h"
#include "keduvockvtml2writer.h"
// #include <KDebug>
#include <QtCore/QMap>

class QTvtVocTranslation::QTvtVocTranslationPrivate
{
public:
    QTvtVocTranslationPrivate(QTvtVocExpression* parent);

    ~QTvtVocTranslationPrivate();

    QTvtVocExpression* m_entry;

    /// Type of a word noun, verb, adjective etc
    QTvtVocWordType* m_wordType;

    /// Leitner box of the translation.
    QTvtVocLeitnerBox* m_leitnerBox;

    /// A comment giving additional information.
    QString m_comment;
    /// A hint, to make guessing the word easier.
    QString m_hint;
    /// Paraphrase
    QString m_paraphrase;
    /// An example
    QString m_example;
    /// Pronunciation
    QString m_pronunciation;
    /// Image url
    QUrl m_imageUrl;
    /// Sound url
    QUrl m_soundUrl;

    /// When creating multiple choice tests, these are possible answers. (otherwise other words are added randomly)
    QStringList m_multipleChoice;

    /// Conjugations of a word (I go, you go, he goes... boring in english)
    QMap <QString, QTvtVocConjugation> m_conjugations;

    /// The comparison forms of adjectives and adverbs: (fast), faster, fastest
    QString m_comparative;
    QString m_superlative;

    QTvtVocDeclension* m_declension;

    // connections to other translations
    /// Synonyms for a word: sick and ill, student and pupil
    QList< QTvtVocTranslation* > m_synonyms;
    /// An antonym - the opposite: hot - cold
    QList< QTvtVocTranslation* > m_antonyms;
    /// List of false friends
    QList< QTvtVocTranslation* > m_falseFriends;
};

QTvtVocTranslation::QTvtVocTranslationPrivate::QTvtVocTranslationPrivate(QTvtVocExpression* parent)
{
    m_entry = parent;
    m_wordType = 0;
    m_leitnerBox = 0;
    m_declension = 0;
}


QTvtVocTranslation::QTvtVocTranslationPrivate::~QTvtVocTranslationPrivate()
{
    delete m_declension;
}

QTvtVocTranslation::QTvtVocTranslation(QTvtVocExpression* entry) : d( new QTvtVocTranslationPrivate(entry) )
{
}


QTvtVocTranslation::QTvtVocTranslation(QTvtVocExpression* entry, const QString &translation ) : d( new QTvtVocTranslationPrivate(entry) )
{
    setText(translation.simplified());
}

QTvtVocTranslation::QTvtVocTranslation( const QTvtVocTranslation &other )
    : QTvtVocText(other),
    // set the entry to 0, the translation will be put into a copied entry by the expression copy constructor
    d( new QTvtVocTranslationPrivate(0) )
{
    // beter no word type copy as this is pointer copying
    // will not work as this is not added to the word type container!
//  d->m_wordType = other.d->m_wordType;
//  d->m_leitnerBox = translation.d->m_leitnerBox;
    d->m_comment = other.d->m_comment;
    d->m_paraphrase = other.d->m_paraphrase;
    d->m_example = other.d->m_example;
    d->m_pronunciation = other.d->m_pronunciation;
    d->m_conjugations = other.d->m_conjugations;
    d->m_comparative = other.d->m_comparative;
    d->m_superlative = other.d->m_superlative;
    d->m_multipleChoice = other.d->m_multipleChoice;
    d->m_imageUrl = other.d->m_imageUrl;
    d->m_soundUrl = other.d->m_soundUrl;
//  no copies of the following for now. we don't know enough to also add this as synonym/etc
//  d->m_synonyms = other.d->m_synonyms;
//  d->m_antonyms = other.d->m_antonyms;
//  d->m_falseFriends = other.d->m_falseFriends;
    if (other.d->m_declension) {
        d->m_declension = new QTvtVocDeclension(*other.d->m_declension);
    }
}

QTvtVocTranslation::~QTvtVocTranslation()
{
    setWordType(0);
    setLeitnerBox(0);
    foreach (QTvtVocTranslation *synonym, d->m_synonyms) {
        synonym->removeSynonym(this);
    }
    foreach (QTvtVocTranslation *antonym, d->m_antonyms) {
        antonym->removeAntonym(this);
    }
    foreach (QTvtVocTranslation *falseFriend, d->m_falseFriends) {
        falseFriend->removeFalseFriend(this);
    }
    delete d;
}

bool QTvtVocTranslation::operator == ( const QTvtVocTranslation & translation ) const
{
    return QTvtVocText::operator==(translation) &&
        d->m_wordType == translation.d->m_wordType &&
        d->m_leitnerBox == translation.d->m_leitnerBox &&
        d->m_comment == translation.d->m_comment &&
        d->m_paraphrase == translation.d->m_paraphrase &&
        d->m_example == translation.d->m_example &&
        d->m_pronunciation == translation.d->m_pronunciation &&
        d->m_imageUrl == translation.d->m_imageUrl &&
        d->m_soundUrl == translation.d->m_soundUrl &&
        d->m_comparative == translation.d->m_comparative &&
        d->m_superlative == translation.d->m_superlative &&
        d->m_multipleChoice == translation.d->m_multipleChoice &&
        d->m_synonyms == translation.d->m_synonyms &&
        d->m_antonyms == translation.d->m_antonyms &&
        d->m_falseFriends == translation.d->m_falseFriends &&
        d->m_conjugations == translation.d->m_conjugations;
           /// @todo check and include declensions d->m_declension == translation.d->m_declension;
}

QTvtVocTranslation & QTvtVocTranslation::operator = ( const QTvtVocTranslation & translation )
{
    QTvtVocText::operator=(translation);
    d->m_entry = translation.d->m_entry;
//     d->m_wordType = translation.d->m_wordType;
//     d->m_leitnerBox = translation.d->m_leitnerBox;
    d->m_comment = translation.d->m_comment;
    d->m_paraphrase = translation.d->m_paraphrase;
    d->m_example = translation.d->m_example;
    d->m_pronunciation = translation.d->m_pronunciation;
    d->m_imageUrl = translation.d->m_imageUrl;
    d->m_soundUrl = translation.d->m_soundUrl;
    d->m_comparative = translation.d->m_comparative;
    d->m_superlative = translation.d->m_superlative;
    d->m_multipleChoice = translation.d->m_multipleChoice;
    d->m_falseFriends = translation.d->m_falseFriends;
    d->m_synonyms = translation.d->m_synonyms;
    d->m_antonyms = translation.d->m_antonyms;
    d->m_conjugations = translation.d->m_conjugations;
    if (translation.d->m_declension) {
        d->m_declension = new QTvtVocDeclension(*translation.d->m_declension);
    }

    return *this;
}


QString QTvtVocTranslation::comment() const
{
    return d->m_comment;
}


void QTvtVocTranslation::setComment( const QString & expr )
{
    d->m_comment = expr.simplified();
}


void QTvtVocTranslation::addFalseFriend( QTvtVocTranslation* falseFriend )
{
    d->m_falseFriends.append(falseFriend);
}

void QTvtVocTranslation::removeFalseFriend(QTvtVocTranslation * falseFriend)
{
    d->m_falseFriends.removeAt(d->m_falseFriends.indexOf(falseFriend));
}

QList< QTvtVocTranslation* > QTvtVocTranslation::falseFriends() const
{
    return d->m_falseFriends;
}


void QTvtVocTranslation::addSynonym( QTvtVocTranslation* synonym )
{
    d->m_synonyms.append(synonym);
}

void QTvtVocTranslation::removeSynonym(QTvtVocTranslation * synonym)
{
    d->m_synonyms.removeAt(d->m_synonyms.indexOf(synonym));
}

QList<QTvtVocTranslation*> QTvtVocTranslation::synonyms() const
{
    return d->m_synonyms;
}

void QTvtVocTranslation::addAntonym( QTvtVocTranslation* antonym )
{
    d->m_antonyms.append(antonym);
}

QList<QTvtVocTranslation*> QTvtVocTranslation::antonyms() const
{
    return d->m_antonyms;
}

void QTvtVocTranslation::removeAntonym(QTvtVocTranslation * antonym)
{
    d->m_antonyms.removeAt(d->m_antonyms.indexOf(antonym));
}

void QTvtVocTranslation::setExample( const QString & expr )
{
    d->m_example = expr.simplified();
}


QString QTvtVocTranslation::example() const
{
    return d->m_example;
}


void QTvtVocTranslation::setParaphrase( const QString & expr )
{
    d->m_paraphrase = expr.simplified();
}


QString QTvtVocTranslation::paraphrase() const
{
    return d->m_paraphrase;
}


void QTvtVocTranslation::setConjugation( const QString& tense, const QTvtVocConjugation& con )
{
    d->m_conjugations[tense] = con;
}


QTvtVocConjugation& QTvtVocTranslation::conjugation( const QString& tense )
{
    return d->m_conjugations[tense];
}


QStringList & QTvtVocTranslation::multipleChoice()
{
    return d->m_multipleChoice;
}


QString QTvtVocTranslation::pronunciation() const
{
    return d->m_pronunciation;
}


void QTvtVocTranslation::setPronunciation( const QString & expr )
{
    d->m_pronunciation = expr.simplified();
}

QStringList QTvtVocTranslation::conjugationTenses() const
{
    return d->m_conjugations.keys();
}

QMap< QString, QTvtVocConjugation > QTvtVocTranslation::conjugations() const
{
    return d->m_conjugations;
}

void QTvtVocTranslation::setConjugations(const QMap< QString, QTvtVocConjugation > & conjugations)
{
    d->m_conjugations = conjugations;
}

/** get the sound url for this translation if it exists */
QUrl QTvtVocTranslation::soundUrl()
{
    return d->m_soundUrl;
}

/** set the sound url for this translation
 * @param url               url of the sound file */
void QTvtVocTranslation::setSoundUrl(const QUrl &url)
{
    d->m_soundUrl = url;
}

/** get the image url for this translation if it exists */
QUrl QTvtVocTranslation::imageUrl()
{
    return d->m_imageUrl;
}

/** set the image url for this translation
 * @param url               url of the image
 */
void QTvtVocTranslation::setImageUrl(const QUrl &url)
{
    d->m_imageUrl = url;
}

QTvtVocWordType * QTvtVocTranslation::wordType() const
{
    if (d) {
        return d->m_wordType;
    } else {
        return 0;
    }
}

void QTvtVocTranslation::setWordType(QTvtVocWordType * wordType)
{
    if ( d->m_wordType ) {
        d->m_wordType->removeTranslation(this);
    }
    if ( wordType ) {
        wordType->addTranslation(this);
    }
    d->m_wordType = wordType;
}

QTvtVocLeitnerBox * QTvtVocTranslation::leitnerBox() const
{
    return d->m_leitnerBox;
}

void QTvtVocTranslation::setLeitnerBox(QTvtVocLeitnerBox * leitnerBox)
{
    if ( d->m_leitnerBox ) {
        d->m_leitnerBox->removeTranslation(this);
    }
    if ( leitnerBox ) {
        leitnerBox->addTranslation(this);
    }
    d->m_leitnerBox = leitnerBox;
}

QTvtVocExpression * QTvtVocTranslation::entry()
{
    return d->m_entry;
}

QString QTvtVocTranslation::comparative() const
{
    return d->m_comparative;
}

void QTvtVocTranslation::setComparative(const QString & comparative)
{
    d->m_comparative = comparative;
}

QString QTvtVocTranslation::superlative() const
{
    return d->m_superlative;
}

void QTvtVocTranslation::setSuperlative(const QString & superlative)
{
    d->m_superlative = superlative;
}

QTvtVocDeclension * QTvtVocTranslation::declension()
{
    return d->m_declension;
}

void QTvtVocTranslation::setDeclension(QTvtVocDeclension * declension)
{
    // remove the old declension object
    delete d->m_declension;
    d->m_declension = declension;
}

void QTvtVocTranslation::toKVTML2(QDomElement & parent)
{
    // text and grade
    QTvtVocText::toKVTML2(parent);

    // declension
    if (d->m_declension) {
        d->m_declension->toKVTML2(parent);
    }

    // conjugation
    foreach ( const QString &tense, conjugationTenses() ) {
        QDomElement conjugationElement = parent.ownerDocument().createElement( KVTML_CONJUGATION );
        conjugation(tense).toKVTML2(conjugationElement, tense);
        parent.appendChild( conjugationElement );
    }

    // <comment>
    QTvtVocKvtml2Writer::appendTextElement( parent, KVTML_COMMENT, comment() );

    // <pronunciation>
    QTvtVocKvtml2Writer::appendTextElement( parent, KVTML_PRONUNCIATION, pronunciation() );

    // <example>
    QTvtVocKvtml2Writer::appendTextElement( parent, KVTML_EXAMPLE, example() );

    // <paraphrase>
    QTvtVocKvtml2Writer::appendTextElement( parent, KVTML_PARAPHRASE, paraphrase() );

    ///@todo synonyms, antonyms
    ///@todo false friends
}

void QTvtVocTranslation::fromKVTML2(QDomElement & parent)
{
    QTvtVocText::fromKVTML2(parent);

    setDeclension(QTvtVocDeclension::fromKVTML2(parent));

    setComment( parent.firstChildElement( KVTML_COMMENT ).text() );

    setPronunciation( parent.firstChildElement( KVTML_PRONUNCIATION ).text() );

    //<example></example>
    setExample( parent.firstChildElement( KVTML_EXAMPLE ).text() );

    //<paraphrase></paraphrase>
    setParaphrase( parent.firstChildElement( KVTML_PARAPHRASE ).text() );

    // conjugations
    QDomElement conjugationElement = parent.firstChildElement( KVTML_CONJUGATION );
    while ( !conjugationElement.isNull() ) {
        QDomElement tenseElement = conjugationElement.firstChildElement( KVTML_TENSE );
        QString tense = tenseElement.text();
        QTvtVocConjugation *conjugation = QTvtVocConjugation::fromKVTML2(conjugationElement);
        setConjugation(tense, *conjugation);
        delete conjugation;
        conjugationElement = conjugationElement.nextSiblingElement( KVTML_CONJUGATION );
    }

    ///@todo synonyms, antonym
    ///@todo false friends
}

void QTvtVocTranslation::setEntry(QTvtVocExpression * entry)
{
    d->m_entry = entry;
}



