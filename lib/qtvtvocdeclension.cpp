/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocdeclension.cpp
***************************************************************************/

/***************************************************************************

    C++ Implementation: keduvocdeclension

    -----------------------------------------------------------------------

    begin         : Do Sep 20 2007

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

#include "qtvtvocdeclension.h"

#include "kvtml2defs.h"
#include "qtvtvockvtml2writer.h"
#include <QtXml/QDomDocument>

#include <QtCore/QMap>

class QTvtVocDeclension::Private
{
public:
    QMap<QTvtVocWordFlags, QTvtVocText> m_declensions;
};

QTvtVocDeclension::QTvtVocDeclension()
    :d (new Private)
{
}

QTvtVocDeclension::QTvtVocDeclension(const QTvtVocDeclension & other)
    :d (new Private)
{
    d->m_declensions = other.d->m_declensions;
}

QTvtVocDeclension & QTvtVocDeclension::operator =(const QTvtVocDeclension & other)
{
    d->m_declensions = other.d->m_declensions;
    return *this;
}

QTvtVocDeclension::~QTvtVocDeclension()
{
    delete d;
}

QTvtVocText& QTvtVocDeclension::declension(QTvtVocWordFlags flags)
{
    return d->m_declensions[flags];
}

void QTvtVocDeclension::setDeclension(const QTvtVocText & declension, QTvtVocWordFlags flags)
{
    d->m_declensions[flags] = declension;
}

bool QTvtVocDeclension::isEmpty()
{
    return d->m_declensions.isEmpty();
}

void QTvtVocDeclension::toKVTML2(QDomElement & parent)
{
    if (isEmpty()) {
        return;
    }
    QDomDocument domDoc = parent.ownerDocument();
    QDomElement declensionElement = domDoc.createElement( KVTML_DECLENSION );

    QMap<int, QTvtVocWordFlags> numbers;
    numbers[0] = QTvtVocWordFlag::Singular;
    numbers[1] = QTvtVocWordFlag::Dual;
    numbers[2] = QTvtVocWordFlag::Plural;

    QMap<int, QTvtVocWordFlags> cases;
    cases[0] = QTvtVocWordFlag::Nominative;
    cases[1] = QTvtVocWordFlag::Genitive;
    cases[2] = QTvtVocWordFlag::Dative;
    cases[3] = QTvtVocWordFlag::Accusative;
    cases[4] = QTvtVocWordFlag::Ablative;
    cases[5] = QTvtVocWordFlag::Locative;
    cases[6] = QTvtVocWordFlag::Vocative;



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

QTvtVocDeclension* QTvtVocDeclension::fromKVTML2(QDomElement & parent)
{
    QDomElement declensionElement = parent.firstChildElement( KVTML_DECLENSION );
    // we don't create empty objects, if necessary, create later on demand.
    if (declensionElement.isNull()) {
        return 0;
    }


    QMap<int, QTvtVocWordFlags> numbers;
    numbers[0] = QTvtVocWordFlag::Singular;
    numbers[1] = QTvtVocWordFlag::Dual;
    numbers[2] = QTvtVocWordFlag::Plural;

    QMap<int, QTvtVocWordFlags> cases;
    cases[0] = QTvtVocWordFlag::Nominative;
    cases[1] = QTvtVocWordFlag::Genitive;
    cases[2] = QTvtVocWordFlag::Dative;
    cases[3] = QTvtVocWordFlag::Accusative;
    cases[4] = QTvtVocWordFlag::Ablative;
    cases[5] = QTvtVocWordFlag::Locative;
    cases[6] = QTvtVocWordFlag::Vocative;

    QTvtVocDeclension* declension = new QTvtVocDeclension;

    for ( int num = 0; num <= 2; ++num ) {
        QDomElement numberElement = declensionElement.firstChildElement( KVTML_GRAMMATICAL_NUMBER[num] );
        if (!numberElement.isNull()) {
            for ( int dcase = 0; dcase <= 6; ++dcase) {
                QDomElement caseElement = numberElement.firstChildElement( KVTML_DECLENSION_CASE[dcase] );
                if (!caseElement.isNull()) {
                    QTvtVocText text;
                    text.fromKVTML2(caseElement);
                    declension->setDeclension(text, numbers[num] | cases[dcase]);
                }
            }
        }
    }
    return declension;
}



