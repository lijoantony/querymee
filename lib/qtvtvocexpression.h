/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocexpression.h
***************************************************************************/

/***************************************************************************
                        Vocabulary Expression for KDE Edu
    -----------------------------------------------------------------------
    copyright            : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                           (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
    Copyright 2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
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

#ifndef QTVTVOCEXPRESSION_H
#define QTVTVOCEXPRESSION_H

#include "libqtvtvocdocument_export.h"

#include <QtCore/QDateTime>
#include <QMap>

#include "qtvtvoclesson.h"
#include "qtvtvocarticle.h"
#include "qtvtvocmultiplechoice.h"
#include "qtvtvoctranslation.h"

class QTvtVocLesson;

/**
  This class contains one vocabulary expression as an original with one or more
  translations
  */
class QTVTVOCDOCUMENT_EXPORT QTvtVocExpression
{
public:

    /** default constructor for an empty vocabulary expression
     */
    explicit QTvtVocExpression();

    /** Constructor for a vocabulary expression with one translation
     *
     * @param expression       translation
     * @param lesson           lesson number
     */
    explicit QTvtVocExpression( const QString & expression );

    /** Constructor for a vocabulary expression with an original and one or more translations
     *
     * @param expression       expression
     * @param separator        expression will be split into an original and one or more translations using separator
     * @param lesson           lesson number, 0 for none
     */
    explicit QTvtVocExpression( const QStringList & translations );

    QTvtVocExpression(const QTvtVocExpression& other);

    ~QTvtVocExpression();

    /** return the lesson
     */
    QTvtVocLesson * lesson() const;


    /** reset all grades of the entry
     * @param index     identifier (language)
     */
    void resetGrades( int index );

    /** returns flag if entry is activated for queries
     */
    bool isActive() const;

    /** set entry active (enabled for queries)
     */
    void setActive( bool flag = true );

    int sizeHint() const;
    void setSizeHint( int sizeHint );

    void setTranslation( int index, QTvtVocTranslation* translation );
    /**
     * Add a translation to this expression
     * @param index            number of translation = the identifier
     * @param expression       the translation
     */
    void setTranslation( int index, const QString &expression );

    /** removes a translation
     *
     * @param index            number of translation 1..x
     */
    void removeTranslation( int index );

    /**
     * Get a pointer to the translation
     * @param index of the language identifier
     * @return the translation
     */
    QTvtVocTranslation* translation( int index );
    QTvtVocTranslation* translation( int index ) const;

    QList<int> translationIndices() const;

    QTvtVocExpression& operator= ( const QTvtVocExpression &expression );
    bool operator== ( const QTvtVocExpression &expression ) const;

private:
    class QTvtVocExpressionPrivate;
    QTvtVocExpressionPrivate* const d;

    /** only called by lesson to add itself to the lesson list
     */
    void setLesson( QTvtVocLesson * l );

    friend class QTvtVocLesson;
};

#endif // QTVTVOCEXPRESSION_H
