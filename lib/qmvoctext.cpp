/***************************************************************************
*   this file is from kdeedu project. Filename: keduvoctext.cpp
***************************************************************************/

/***************************************************************************
    Copyright 2007-2008 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
    Copyright (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qmvoctext.h"

#include "kvtml2defs.h"
#include "qmvockvtml2writer.h"

#include <QtXml/QDomDocument>

class QmVocText::QmVocTextPrivate
{
public:
    /// This is the word itself. The vocabulary. This is what it is all about.
    QString m_text;

    grade_t m_grade;
    count_t m_totalPracticeCount;
    count_t m_badCount;
    QDateTime m_practiceDate;
};

QmVocText::QmVocText(const QString& text)
        :d( new QmVocTextPrivate )
{
    d->m_text = text;
    resetGrades();
}

QmVocText::QmVocText( const QmVocText &other )
        :d( new QmVocTextPrivate )
{
    d->m_text = other.d->m_text;
    setGrade( other.grade() );
    setPracticeCount( other.practiceCount() );
    setBadCount( other.badCount() );
    setPracticeDate( other.practiceDate() );
}

QmVocText::~QmVocText()
{
    delete d;
}

QString QmVocText::text() const
{
    return d->m_text;
}

void QmVocText::setText( const QString & expr )
{
    d->m_text = expr.simplified();
}

void QmVocText::resetGrades()
{
    d->m_grade = KV_NORM_GRADE;
    d->m_totalPracticeCount = 0;
    d->m_badCount = 0;

    QDateTime dt;
    dt.setTime_t( 0 );
    d->m_practiceDate = dt;
}


grade_t QmVocText::grade() const
{
    return d->m_grade;
}


void QmVocText::setGrade( grade_t grade )
{
    if ( grade > KV_MAX_GRADE ) {
        grade = KV_MAX_GRADE;
    }
    d->m_grade = grade;
}


void QmVocText::incGrade()
{
    // setGrade( qMax<grade_t>(grade(), KV_LEV1_GRADE ) + 1 );
    setGrade( grade() + 1 );
}


void QmVocText::decGrade()
{
    if ( grade() == KV_MIN_GRADE ) {
        return;
    }
    setGrade( grade() - 1 );
}


count_t QmVocText::practiceCount()  const
{
    return d->m_totalPracticeCount;
}


void QmVocText::incPracticeCount()
{
    setPracticeCount( practiceCount() + 1 );
}


void QmVocText::incBadCount()
{
    setBadCount( badCount() + 1 );
}


void QmVocText::setPracticeCount( count_t count )
{
    d->m_totalPracticeCount = count;
}


count_t QmVocText::badCount() const
{
    return d->m_badCount;
}


void QmVocText::setBadCount( count_t count )
{
    d->m_badCount = count;
}


QDateTime QmVocText::practiceDate() const
{
    return d->m_practiceDate;
}


void QmVocText::setPracticeDate( const QDateTime & date )
{
    d->m_practiceDate = date;
}

QmVocText & QmVocText::operator =(const QmVocText & other)
{
    d->m_text = other.d->m_text;
    d->m_grade = other.d->m_grade;
    d->m_totalPracticeCount = other.d->m_totalPracticeCount;
    d->m_badCount = other.d->m_badCount;
    d->m_practiceDate = other.d->m_practiceDate;

    return *this;
}

bool QmVocText::operator ==(const QmVocText & other) const
{
    return
        d->m_text == other.d->m_text &&
        d->m_grade == other.d->m_grade &&
        d->m_totalPracticeCount == other.d->m_totalPracticeCount &&
        d->m_badCount == other.d->m_badCount &&
        d->m_practiceDate == other.d->m_practiceDate;
}

void QmVocText::toKVTML2(QDomElement& parent)
{
    QDomDocument domDoc = parent.ownerDocument();
    if (d->m_text.isEmpty() && d->m_totalPracticeCount == 0) {
        return;
    }

    // the text
    QmVocKvtml2Writer::appendTextElement( parent, KVTML_TEXT, text() );

    // grades
    if ( d->m_totalPracticeCount > 0 ) {
        QDomElement gradeElement = domDoc.createElement( KVTML_GRADE );

            //<currentgrade>2</currentgrade>
        QmVocKvtml2Writer::appendTextElement( gradeElement, KVTML_CURRENTGRADE, QString::number( grade() ) );

            //<count>6</count>
        QmVocKvtml2Writer::appendTextElement( gradeElement, KVTML_COUNT, QString::number( practiceCount() ) );

            //<errorcount>1</errorcount>
        QmVocKvtml2Writer::appendTextElement( gradeElement, KVTML_ERRORCOUNT, QString::number( badCount() ) );

            //<date>949757271</date>
        QmVocKvtml2Writer::appendTextElement( gradeElement, KVTML_DATE,  practiceDate().toString( Qt::ISODate ) );

        parent.appendChild( gradeElement );
    }
}

void QmVocText::fromKVTML2(QDomElement & parent)
{
    setText( parent.firstChildElement( KVTML_TEXT ).text() );

    // grade element
    const QDomElement& gradeElement = parent.firstChildElement( KVTML_GRADE );
    if ( !gradeElement.isNull() ) {

        setGrade( gradeElement.firstChildElement(KVTML_CURRENTGRADE).text().toInt() );

        setPracticeCount( gradeElement.firstChildElement(KVTML_COUNT).text().toInt() );

        setBadCount( gradeElement.firstChildElement(KVTML_ERRORCOUNT).text().toInt() );

        QString dateString = gradeElement.firstChildElement(KVTML_DATE).text();
        if ( !dateString.isEmpty() ) {
            QDateTime value = QDateTime::fromString( dateString, Qt::ISODate );
            setPracticeDate( value );
        }
    }
}

bool QmVocText::isEmpty()
{
    return d->m_text.isEmpty();
}
