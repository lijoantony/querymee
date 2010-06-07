/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocdeclension.cpp
***************************************************************************/

/***************************************************************************

    C++ Implementation: qtvtvocdeclension

    -----------------------------------------------------------------------

    begin         : Do Sep 20 2007

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

#include "qtvtvocdeclension.h"

#include "kvtml2defs.h"
#include "qtvtvockvtml2writer.h"
#include <QtXml/QDomDocument>

#include <QtCore/QMap>

class QmVocDeclension::Private
{
public:
    QMap<QmVocWordFlags, QmVocText> m_declensions;
};

QmVocDeclension::QmVocDeclension()
    :d (new Private)
{
}

QmVocDeclension::QmVocDeclension(const QmVocDeclension & other)
    :d (new Private)
{
    d->m_declensions = other.d->m_declensions;
}

QmVocDeclension & QmVocDeclension::operator =(const QmVocDeclension & other)
{
    d->m_declensions = other.d->m_declensions;
    return *this;
}

QmVocDeclension::~QmVocDeclension()
{
    delete d;
}

QmVocText& QmVocDeclension::declension(QmVocWordFlags flags)
{
    return d->m_declensions[flags];
}

void QmVocDeclension::setDeclension(const QmVocText & declension, QmVocWordFlags flags)
{
    d->m_declensions[flags] = declension;
}

bool QmVocDeclension::isEmpty()
{
    return d->m_declensions.isEmpty();
}

void QmVocDeclension::toKVTML2(QDomElement & parent)
{
    if (isEmpty()) {
        return;
    }
    QDomDocument domDoc = parent.ownerDocument();
    QDomElement declensionElement = domDoc.createElement( KVTML_DECLENSION );

    QMap<int, QmVocWordFlags> numbers;
    numbers[0] = QmVocWordFlag::Singular;
    numbers[1] = QmVocWordFlag::Dual;
    numbers[2] = QmVocWordFlag::Plural;

    QMap<int, QmVocWordFlags> cases;
    cases[0] = QmVocWordFlag::Nominative;
    cases[1] = QmVocWordFlag::Genitive;
    cases[2] = QmVocWordFlag::Dative;
    cases[3] = QmVocWordFlag::Accusative;
    cases[4] = QmVocWordFlag::Ablative;
    cases[5] = QmVocWordFlag::Locative;
    cases[6] = QmVocWordFlag::Vocative;



    for ( int num = 0; num <= 2; ++num) {
        QDomElement numberElement = domDoc.createElement( KVTML_GRAMMATICAL_NUMBER[num] );
        for ( int dcase = 0; dcase <= 6; ++dcase ) {
            QDomElement caseElement = domDoc.createElement( KVTML_DECLENSION_CASE[dcase] );
            declension(numbers[num] | cases[dcase]).toKVTML2(caseElement);

            if (caseElement.hasChildNodes()) {
                numberElement.appendChild(caseElement);
            }
        }
        if (numberElement.hasChildNodes()) {
            declensionElement.appendChild(numberElement);
        }
    }
    if (declensionElement.hasChildNodes()) {
        parent.appendChild(declensionElement);
    }
}

QmVocDeclension* QmVocDeclension::fromKVTML2(QDomElement & parent)
{
    QDomElement declensionElement = parent.firstChildElement( KVTML_DECLENSION );
    // we don't create empty objects, if necessary, create later on demand.
    if (declensionElement.isNull()) {
        return 0;
    }


    QMap<int, QmVocWordFlags> numbers;
    numbers[0] = QmVocWordFlag::Singular;
    numbers[1] = QmVocWordFlag::Dual;
    numbers[2] = QmVocWordFlag::Plural;

    QMap<int, QmVocWordFlags> cases;
    cases[0] = QmVocWordFlag::Nominative;
    cases[1] = QmVocWordFlag::Genitive;
    cases[2] = QmVocWordFlag::Dative;
    cases[3] = QmVocWordFlag::Accusative;
    cases[4] = QmVocWordFlag::Ablative;
    cases[5] = QmVocWordFlag::Locative;
    cases[6] = QmVocWordFlag::Vocative;

    QmVocDeclension* declension = new QmVocDeclension;

    for ( int num = 0; num <= 2; ++num ) {
        QDomElement numberElement = declensionElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[num] );
        if (!numberElement.isNull()) {
            for ( int dcase = 0; dcase <= 6; ++dcase) {
                QDomElement caseElement = numberElement.firstChildElement( KVTML_DECLENSION_CASE[dcase] );
                if (!caseElement.isNull()) {
                    QmVocText text;
                    text.fromKVTML2(caseElement);
                    declension->setDeclension(text, numbers[num] | cases[dcase]);
                }
            }
        }
    }
    return declension;
}



