/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoctranslation.h
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


#ifndef QMVOCTRANSLATION_H
#define QMVOCTRANSLATION_H

#include "libqtvtvocdocument_export.h"
#include "qtvtvocarticle.h"
#include "qtvtvocconjugation.h"
#include "qtvtvoctext.h"
// #include <KDE/KUrl>
#include <QUrl>
#include <QtCore/QString>

class QmVocExpression;
// class KEduVocString;
class QmVocWordType;
class QmVocLeitnerBox;
class QmVocDeclension;

/**
 @author Frederik Gladhorn <frederik.gladhorn@kdemail.net>
*/
class QTVTVOCDOCUMENT_EXPORT QmVocTranslation
    :public QmVocText
{
public:
    enum Related {
        Synonym,
        Antonym,
        FalseFriend
    };

    /**
     * Default constructor for an empty translation.
     */
    QmVocTranslation(QmVocExpression* entry);

    /**
        * Constructor
        * @param translation is used as translation
        */
    QmVocTranslation(QmVocExpression* entry, const QString &translation );

    /** copy constructor for d-pointer safety */
    QmVocTranslation( const QmVocTranslation &other );

    /**
     * Destructor
     */
    ~QmVocTranslation();

    QmVocExpression* entry();

    /** sets the pronunciation of this expression
    * @param expression       pronunciation of this index
    */
    void setPronunciation( const QString & expression );

    /** returns the pronunciation of this expression
    * @return                 pronunciation or "" if none available
    */
    QString pronunciation() const;

    /** returns comments of this expression
    * @return                 comment or "" if no comment available
    */
    QString comment() const;

    /** sets comment of this expression
    * @param expr             comment of this index
    */
    void setComment( const QString & expr );

    /** sets example this expression
    * @param expression       example of this index
    */
    void setExample( const QString & expression );

    /** returns example of this expression
    * @return                 example or "" if no string available
    */
    QString example() const;

    /** sets paraphrase of this expression
    * @param expression       paraphrase of this index
    */
    void setParaphrase( const QString & expression );

    /** returns paraphrase of this expression
    * @return                 paraphrase or "" if no string available
    */
    QString paraphrase() const;

    /** sets antonym this expression
    * @param expression       antonym of this index
    */
    void setAntonym( const QString & expression );

    /** returns antonym of this expression
    * @return                 antonym or "" if no string available
    */
    QString antonym() const;

    /**
     * Set a irregular plural form.
     * @param plural plural of the word
     */
    void setIrregularPlural( const QString& plural );

    /**
     * Get a irregular plural form.
     * @return plural of the word
     */
    QString irregularPlural() const;

    /** returns the word type of this expression, you will get a 0 pointer
    *   if wordtype isn't set for the translation
    *
    * @return                 type or "" if no type available
    */
    QmVocWordType* wordType() const;

    /** sets the word type of this expression
    * @param type             type of this expression ("" = none)
    */
    void setWordType( QmVocWordType* wordType );

    /** returns the leitner box of this translation
     * @return the box
     */
    QmVocLeitnerBox* leitnerBox() const;

    /** sets the leitner box of this translation
     * @param leitnerBox the box
     */
    void setLeitnerBox( QmVocLeitnerBox* leitnerBox );

    /**
     * returns a conjugation if available
     * @param tense tense of the requested conjugation
     * @return the conjugation
     */
    QmVocConjugation& conjugation( const QString& tense );

    /** adds conjugations or replaces them, if they exist.
    * @param conjugation      conjugation
    */
    void setConjugation( const QString& tense, const QmVocConjugation & conjugation );

    /**
     * Returns a pointer to the declension object of this translation.
     * Returns 0 if no declension object exists!
     * @return the declension
     */
    QmVocDeclension* declension();

    /**
     * Set a new declension for a translation
     * @param declension
     */
    void setDeclension(QmVocDeclension* declension);

    /**
     * Bad, only used for tense entry page, will be deleted later. Deprecated.
     * @param conjugation
     */
    void setConjugations( const QMap<QString, QmVocConjugation>& conjugations );

    QStringList conjugationTenses() const;

    /**
     * Bad, only compatibility. Deprecated.
     * @return
     */
    QMap <QString, QmVocConjugation> conjugations() const;

    /**
     * Comparison forms of adjectives/adverbs.
     */
    QString comparative() const;
    void setComparative(const QString& comparative);
    QString superlative() const;
    void setSuperlative(const QString& superlative);

    /** returns multiple choice if available
      */
    QStringList & multipleChoice();

    /** sets multiple choice
     * @param mc               multiple choice block
      */
//     void setMultipleChoice( const QStringList &mc );

    /** get the sound url for this translation if it exists */
    QUrl soundUrl();

    /** set the sound url for this translation
     * @param url               url of the sound file */
    void setSoundUrl(const QUrl &url);

    /** get the image url for this translation if it exists */
    QUrl imageUrl();

    /** set the image url for this translation
     * @param url               url of the image
     */
    void setImageUrl(const QUrl &url);

    /**
     * add a false friend
     * @param falseFriend false friend of this index
     */
    void addFalseFriend( QmVocTranslation* falseFriend );

    /**
     * remove a false friend
     * @param falseFriend false friend of this index
     */
    void removeFalseFriend( QmVocTranslation* falseFriend );

    /**
     * returns false friends of this expression
     * @return list of false friends
     */
    QList<QmVocTranslation*> falseFriends() const;

    /**
     * add a synonym
     * @param synonym
     */
    void addSynonym( QmVocTranslation* synonym );

    /**
     * remove a synonym
     * @param synonym
     */
    void removeSynonym( QmVocTranslation* synonym );

    /**
     * returns synonyms of this expression
     * @return synonyms
     */
    QList<QmVocTranslation*> synonyms() const;

    /**
     * add a antonym
     * @param antonym
         */
    void addAntonym( QmVocTranslation* antonym );

    /**
     * remove a antonym
     * @param antonym
     */
    void removeAntonym( QmVocTranslation* antonym );

    /**
     * returns antonyms of this expression
     * @return antonyms
     */
    QList<QmVocTranslation*> antonyms() const;

    /**
     * Equal operator to assing a translation to another one.
     * @param translation translation to be copied
     * @return reference to the new translation
     */
    QmVocTranslation& operator= ( const QmVocTranslation &translation );

    /**
     * Compare two translations, including word type etc.
     * @param translation
     * @return true if equal
     */
    bool operator== ( const QmVocTranslation &translation ) const;

    void fromKVTML2(QDomElement& parent);
    void toKVTML2(QDomElement& parent);

private:
    class QmVocTranslationPrivate;
    QmVocTranslationPrivate* const d;

    // for the copy constructor
    void setEntry(QmVocExpression* entry);
    friend class QmVocExpression;
};

#endif
