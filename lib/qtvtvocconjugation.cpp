/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocconjugation.cpp
***************************************************************************/

/***************************************************************************

    C++ Implementation: keduvocconjugation

    -----------------------------------------------------------------------

    begin         : Di Aug 28 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>

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

#include "qtvtvocconjugation.h"
#include "qtvtvoccommon_p.h"
#include "qtvtvoctext.h"
#include "kvtml2defs.h"

#include <QtCore/QMap>
#include <QtXml/QDomDocument>

class QTvtVocConjugation::Private
{
public:
    QMap<QTvtVocWordFlags, QTvtVocText> m_conjugations;
};


QTvtVocConjugation::QTvtVocConjugation()
        : d( new Private )
{}


QTvtVocConjugation::QTvtVocConjugation( const QTvtVocConjugation& other )
        : d( new Private )
{
    d->m_conjugations = other.d->m_conjugations;
}


QTvtVocConjugation::~QTvtVocConjugation()
{
    delete d;
}

QTvtVocConjugation& QTvtVocConjugation::operator = ( const QTvtVocConjugation& other )
{
    d->m_conjugations = other.d->m_conjugations;
    return *this;
}

bool QTvtVocConjugation::operator ==(const QTvtVocConjugation& other) const
{
    return d->m_conjugations == other.d->m_conjugations;
}



QTvtVocText& QTvtVocConjugation::conjugation(QTvtVocWordFlags flags) const
{
        return d->m_conjugations[flags & (QTvtVocWordFlag::persons | QTvtVocWordFlag::numbers | QTvtVocWordFlag::genders)];
}

void QTvtVocConjugation::setConjugation(const QTvtVocText& conjugation, QTvtVocWordFlags flags)
{
    d->m_conjugations[flags & (QTvtVocWordFlag::persons | QTvtVocWordFlag::numbers | QTvtVocWordFlag::genders)] = conjugation;
}

bool QTvtVocConjugation::isEmpty()
{
    return d->m_conjugations.count() == 0;
}

QList< QTvtVocWordFlags > QTvtVocConjugation::keys()
{
    return d->m_conjugations.keys();
}



void QTvtVocConjugation::toKVTML2(QDomElement & parent, const QString &tense)
{
    if (isEmpty()) {
        return;
    }

    QMap<int, QTvtVocWordFlag::Flags> numbers;
    numbers[0] = QTvtVocWordFlag::Singular;
    numbers[1] = QTvtVocWordFlag::Dual;
    numbers[2] = QTvtVocWordFlag::Plural;
    QMap<int, QTvtVocWordFlag::Flags> persons;
    persons[0] = QTvtVocWordFlag::First;
    persons[1] = QTvtVocWordFlag::Second;
    persons[2] = (QTvtVocWordFlag::Flags)((int)QTvtVocWordFlag::Third | (int)QTvtVocWordFlag::Masculine);
    persons[3] = (QTvtVocWordFlag::Flags)((int)QTvtVocWordFlag::Third | (int)QTvtVocWordFlag::Feminine);
    persons[4] = (QTvtVocWordFlag::Flags)((int)QTvtVocWordFlag::Third | (int)QTvtVocWordFlag::Neuter);

    // write the tense tag
    QDomDocument domDoc = parent.ownerDocument();
    QDomElement tenseElement = domDoc.createElement( KVTML_TENSE );
    tenseElement.appendChild( domDoc.createTextNode(tense) );
    parent.appendChild(tenseElement);

    for ( int num = 0; num <= 2; ++num) {
        QDomElement numberElement = domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );
        for ( int person = 0; person < 5; ++person) {
            if (!conjugation(numbers[num] | persons[person]).isEmpty()) {
                QDomElement personElement = domDoc.createElement( KVTML_GRAMMATICAL_PERSON[person] );
                numberElement.appendChild(personElement);
                conjugation(persons[person] | numbers[num]).toKVTML2(personElement);
            }
        }
        if (numberElement.hasChildNodes()) {
            parent.appendChild( numberElement );
        }
    }
}

/*

        QString first = conjugation.conjugation(
                QTvtVocConjugation::First, num );
        QString second = conjugation.conjugation(
                QTvtVocConjugation::Second, num );
        QString third_male = conjugation.conjugation(
                QTvtVocConjugation::ThirdMale, num );
        QString third_female = conjugation.conjugation(
                QTvtVocConjugation::ThirdFemale, num );
        QString third_neutral = conjugation.conjugation(
                QTvtVocConjugation::ThirdNeutralCommon, num );

        if ( !first.isEmpty() || !second.isEmpty() || !third_female.isEmpty() ||
              !third_male.isEmpty() || !third_neutral.isEmpty() ) {
            QDomElement number;
            switch (num) {
                case QTvtVocConjugation::Singular:
                    number = m_domDoc.createElement( KVTML_SINGULAR );
                    break;
                case QTvtVocConjugation::Dual:
                    number = m_domDoc.createElement( KVTML_DUAL );
                    break;
                case QTvtVocConjugation::Plural:
                    number = m_domDoc.createElement( KVTML_PLURAL );
                    break;
            }

            number.appendChild( newTextElement( KVTML_1STPERSON, first ) );
            number.appendChild( newTextElement( KVTML_2NDPERSON, second ) );
            number.appendChild( newTextElement( KVTML_THIRD_MALE, third_male ) );
            number.appendChild( newTextElement( KVTML_THIRD_FEMALE, third_female ) );
            number.appendChild( newTextElement( KVTML_THIRD_NEUTRAL_COMMON, third_neutral ) );

            conjugationElement.appendChild( number );
              }
    }*/


    /*
    for ( QTvtVocWordFlag::DeclensionNumber num = QTvtVocWordFlag::Singular; num <= QTvtVocWordFlag::Plural; num = QTvtVocWordFlag::DeclensionNumber(num +1) ) {
        QDomElement numberElement = domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );
        for ( QTvtVocWordFlag::DeclensionCase dcase = QTvtVocWordFlag::Nominative; dcase < QTvtVocWordFlag::DeclensionCaseMAX; dcase = QTvtVocWordFlag::DeclensionCase(dcase +1) ) {
            QDomElement caseElement = domDoc.createElement( KVTML_DECLENSION_CASE[dcase] );
            declension(num, dcase).toKVTML2(caseElement);

            if (caseElement.hasChildNodes()) {
                numberElement.appendChild(caseElement);
            }
        }
        if (numberElement.hasChildNodes()) {
            declensionElement.appendChild(numberElement);
        }
    }

    */



QTvtVocConjugation* QTvtVocConjugation::fromKVTML2(QDomElement & parent)
{
    // sanity check
    if (parent.isNull()) {
        return 0;
    }

    QMap<int, QTvtVocWordFlag::Flags> numbers;
    numbers[0] = QTvtVocWordFlag::Singular;
    numbers[1] = QTvtVocWordFlag::Dual;
    numbers[2] = QTvtVocWordFlag::Plural;
    QMap<int, QTvtVocWordFlag::Flags> persons;
    persons[0] = QTvtVocWordFlag::First;
    persons[1] = QTvtVocWordFlag::Second;
    persons[2] = (QTvtVocWordFlag::Flags)((int)QTvtVocWordFlag::Third | (int)QTvtVocWordFlag::Masculine);
    persons[3] = (QTvtVocWordFlag::Flags)((int)QTvtVocWordFlag::Third | (int)QTvtVocWordFlag::Feminine);
    persons[4] = (QTvtVocWordFlag::Flags)((int)QTvtVocWordFlag::Third | (int)QTvtVocWordFlag::Neuter);


    QTvtVocConjugation* conjugation = new QTvtVocConjugation;

    for ( int num = 0; num <= 2; num++ ) {
        QDomElement numberElement = parent.firstChildElement( KVTML_GRAMMATICAL_NUMBER[num] );

        if (numberElement.hasChildNodes()) {
            for (int person = 0; person < 5; person++) {
                QDomElement personElement = numberElement.firstChildElement( KVTML_GRAMMATICAL_PERSON[person] );
                if (!personElement.isNull()) {
                    QTvtVocText text;
                    text.fromKVTML2(personElement);
                    if (text.text().isEmpty()) {
                        // compatibility for kde 4.0. There the text was directly below the person, not enabling grades per conjugation form.
                        text.setText(personElement.text());
                    }
                    conjugation->setConjugation(text, persons[person] | numbers[num]);
                }
            }
        }
    }
    return conjugation;
}


