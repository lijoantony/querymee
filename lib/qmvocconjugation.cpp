/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocconjugation.cpp
***************************************************************************/

/***************************************************************************

    C++ Implementation: qtvtvocconjugation

    -----------------------------------------------------------------------

    begin         : Di Aug 28 2007

    copyright     : (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright     : (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

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

#include "qmvocconjugation.h"
#include "qmvoccommon_p.h"
#include "qmvoctext.h"
#include "kvtml2defs.h"

#include <QtCore/QMap>
#include <QtXml/QDomDocument>
#include <QDebug>

class QmVocConjugation::Private
{
public:
    QMap<QmVocWordFlags, QmVocText> m_conjugations;
};


QmVocConjugation::QmVocConjugation()
        : d( new Private )
{}


QmVocConjugation::QmVocConjugation( const QmVocConjugation& other )
        : d( new Private )
{
    d->m_conjugations = other.d->m_conjugations;
}


QmVocConjugation::~QmVocConjugation()
{
    delete d;
}

QmVocConjugation& QmVocConjugation::operator = ( const QmVocConjugation& other )
{
    d->m_conjugations = other.d->m_conjugations;
    return *this;
}

bool QmVocConjugation::operator ==(const QmVocConjugation& other) const
{
    return d->m_conjugations == other.d->m_conjugations;
}



QmVocText& QmVocConjugation::conjugation(QmVocWordFlags flags) const
{
        return d->m_conjugations[flags & (QmVocWordFlag::persons | QmVocWordFlag::numbers | QmVocWordFlag::genders)];
}

void QmVocConjugation::setConjugation(const QmVocText& conjugation, QmVocWordFlags flags)
{
    d->m_conjugations[flags & (QmVocWordFlag::persons | QmVocWordFlag::numbers | QmVocWordFlag::genders)] = conjugation;
}

bool QmVocConjugation::isEmpty()
{
    return d->m_conjugations.isEmpty();
}

QList< QmVocWordFlags > QmVocConjugation::keys()
{
    return d->m_conjugations.keys();
}

void QmVocConjugation::toKVTML2(QDomElement & parent, const QString &tense)
{
    if (isEmpty()) {
        return;
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

    QDomDocument domDoc = parent.ownerDocument();

    // write the tense tag
    if (!tense.isEmpty()) {
        QDomElement tenseElement = domDoc.createElement( KVTML_TENSE );
        tenseElement.appendChild( domDoc.createTextNode(tense) );
        parent.appendChild(tenseElement);
    } else {
        qDebug() << "Saving conjugation with empty tense";
    }

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
                QmVocConjugation::First, num );
        QString second = conjugation.conjugation(
                QmVocConjugation::Second, num );
        QString third_male = conjugation.conjugation(
                QmVocConjugation::ThirdMale, num );
        QString third_female = conjugation.conjugation(
                QmVocConjugation::ThirdFemale, num );
        QString third_neutral = conjugation.conjugation(
                QmVocConjugation::ThirdNeutralCommon, num );

        if ( !first.isEmpty() || !second.isEmpty() || !third_female.isEmpty() ||
              !third_male.isEmpty() || !third_neutral.isEmpty() ) {
            QDomElement number;
            switch (num) {
                case QmVocConjugation::Singular:
                    number = m_domDoc.createElement( KVTML_SINGULAR );
                    break;
                case QmVocConjugation::Dual:
                    number = m_domDoc.createElement( KVTML_DUAL );
                    break;
                case QmVocConjugation::Plural:
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
    for ( QmVocWordFlag::DeclensionNumber num = QmVocWordFlag::Singular; num <= QmVocWordFlag::Plural; num = QmVocWordFlag::DeclensionNumber(num +1) ) {
        QDomElement numberElement = domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );
        for ( QmVocWordFlag::DeclensionCase dcase = QmVocWordFlag::Nominative; dcase < QmVocWordFlag::DeclensionCaseMAX; dcase = QmVocWordFlag::DeclensionCase(dcase +1) ) {
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



QmVocConjugation* QmVocConjugation::fromKVTML2(QDomElement & parent)
{
    // sanity check
    if (parent.isNull()) {
        return 0;
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


    QmVocConjugation* conjugation = new QmVocConjugation;

    for ( int num = 0; num <= 2; num++ ) {
        QDomElement numberElement = parent.firstChildElement( KVTML_GRAMMATICAL_NUMBER[num] );

        if (numberElement.hasChildNodes()) {
            for (int person = 0; person < 5; person++) {
                QDomElement personElement = numberElement.firstChildElement( KVTML_GRAMMATICAL_PERSON[person] );
                if (!personElement.isNull()) {
                    QmVocText text;
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


