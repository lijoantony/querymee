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

#include "libqmvocdocument_export.h"
#include "qmvocconjugation.h"
#include "qmvoctext.h"
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

    /** Sets the pronunciation of this expression
    * @param expression       pronunciation of this index
    */
    void setPronunciation( const QString & expression );

    /** Returns the pronunciation of this expression
    * @return                 pronunciation or "" if none available
    */
    QString pronunciation() const;

    /** Returns comments of this expression
    * @return                 comment or "" if no comment available
    */
    QString comment() const;

    /** Sets comment of this expression
    * @param expr             comment of this index
    */
    void setComment( const QString& expr );

    /** Sets example this expression
    * @param expression       example of this index
    */
    void setExample( const QString & expression );

    /** Returns example of this expression
    * @return                 example or "" if no string available
    */
    QString example() const;

    /** Sets paraphrase of this expression
    * @param expression       paraphrase of this index
    */
    void setParaphrase( const QString & expression );

    /** Returns paraphrase of this expression
    * @return                 paraphrase or "" if no string available
    */
    QString paraphrase() const;

    /** Sets antonym this expression
    * @param expression       antonym of this index
    */
    void setAntonym( const QString & expression );

    /** Returns antonym of this expression
    * @return                 antonym or "" if no string available
    */
    QString antonym() const;

    /**
     * Sets an irregular plural form.
     * @param plural plural of the word
     */
    void setIrregularPlural( const QString& plural );

    /**
     * Gets an irregular plural form.
     * @return plural of the word
     */
    QString irregularPlural() const;

    /** Returns the word type of this expression, you will get a 0 pointer
    *   if wordtype is not set for the translation
    *
    * @return                 type or "" if no type available
    */
    QmVocWordType* wordType() const;

    /** Sets the word type of this expression
    * @param type             type of this expression ("" = none)
    */
    void setWordType( QmVocWordType* wordType );

    /** Returns the leitner box of this translation
     * @return the box
     */
    QmVocLeitnerBox* leitnerBox() const;

    /** Sets the leitner box of this translation
     * @param leitnerBox the box
     */
    void setLeitnerBox( QmVocLeitnerBox* leitnerBox );

    /**
     * Returns a conjugation if available
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
    KDE_DEPRECATED QString comparative() const;
    KDE_DEPRECATED void setComparative(const QString& comparative);
    // TODO rename to comparative and remove the deprecated function
    QmVocText comparativeForm() const;
    void setComparativeForm(const QmVocText& comparative);

    // KDE_DEPRECATED QString superlative() const;
    // KDE_DEPRECATED void setSuperlative(const QString& superlative);
    QString superlative() const;
    void setSuperlative(const QString& superlative);
    QmVocText superlativeForm() const;
    void setSuperlativeForm(const QmVocText& superlative);

    QmVocText article() const;
    void setArticle(const QmVocText& article);

    /** Returns multiple choice if available
      */
    QStringList & multipleChoice();

    /** Sets multiple choice
     * @param mc               multiple choice block
      */
//     void setMultipleChoice( const QStringList &mc );

    /** Get the sound url for this translation if it exists */
    QUrl soundUrl();

    /** Set the sound url for this translation
     * @param url               url of the sound file */
    void setSoundUrl(const QUrl &url);

    /** Get the image url for this translation if it exists */
    QUrl imageUrl();

    /** Set the image url for this translation
     * @param url               url of the image
     */
    void setImageUrl(const QUrl &url);

    /**
     * Add a false friend
     * @param falseFriend false friend of this index
     */
    void addFalseFriend( QmVocTranslation* falseFriend );

    /**
     * Remove a false friend
     * @param falseFriend false friend of this index
     */
    void removeFalseFriend( QmVocTranslation* falseFriend );

    /**
     * Returns false friends of this expression
     * @return list of false friends
     */
    QList<QmVocTranslation*> falseFriends() const;

    /**
     * Add a synonym
     * @param synonym
     */
    void addSynonym( QmVocTranslation* synonym );

    /**
     * Remove a synonym
     * @param synonym
     */
    void removeSynonym( QmVocTranslation* synonym );

    /**
     * Returns synonyms of this expression
     * @return synonyms
     */
    QList<QmVocTranslation*> synonyms() const;

    /**
     * Add a antonym
     * @param antonym
         */
    void addAntonym( QmVocTranslation* antonym );

    /**
     * Remove a antonym
     * @param antonym
     */
    void removeAntonym( QmVocTranslation* antonym );

    /**
     * Returns antonyms of this expression
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
