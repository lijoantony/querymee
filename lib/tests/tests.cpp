/***************************************************************************
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

#include <QtTest/QtTest>


#include "qmvocdocument.h"
#include "qmvoclesson.h"
#include "qmvocexpression.h"
#include "qmvocleitnerbox.h"

#include <QTemporaryFile>
#include <QDebug>
#include <QList>

class RmzTest : public QObject
{
    Q_OBJECT
       
private slots:
    void rmzTestDocumentAboutInfo()
    {
        QTemporaryFile file("XXXXXXXX.kvtml");
        file.open();
        // qDebug( QString("Temp file name is: %1").arg( file.fileName() ) );
        QUrl fileName = QUrl(file.fileName());
        // file.setAutoRemove(0);
        file.close();
        
        const QString generator = QString::fromLatin1( "Validator Unit Tests" );
        const QString author = QString::fromLatin1( "Validator Test" );
        const QString license = QString::fromLatin1( "test license" );
        const QString comment = QString::fromLatin1( "comment" );
        const QString category = QString::fromLatin1( "test document" );
        const QString title = QString::fromLatin1( "Validator Test Title" );
        const QString lang1 = QString::fromLatin1( "English" );
        const QString lang2 = QString::fromLatin1( "German" );
        const QString locale1 = QString::fromLatin1( "en" );
        const QString locale2 = QString::fromLatin1( "de" );
        const QString translation1 = QString::fromLatin1( "this is English" );
        const QString translation2 = QString::fromLatin1( "das ist Deutsch" );
        const QString lessonName = QString::fromLatin1( "Lesson No 1" );
        
        QmVocDocument doc;
        doc.setAuthor( author );
        doc.setLicense( license );
        doc.setDocumentComment( comment );
        doc.setCategory( category );
        doc.setTitle( title );
        
        doc.appendIdentifier();
        doc.appendIdentifier();
        doc.identifier(0).setName( lang1 );
        doc.identifier(0).setLocale( locale1 );
        doc.identifier(1).setName( lang2 );
        doc.identifier(1).setLocale( locale2 );

        QmVocLesson* lesson = new QmVocLesson(lessonName, doc.lesson());
        doc.lesson()->appendChildContainer(lesson);
        lesson->appendEntry(new QmVocExpression);
        lesson->entry(0)->setTranslation(0, translation1);
        lesson->entry(0)->setTranslation(1, translation2);

        doc.saveAs(fileName, QmVocDocument::Kvtml, generator);

        QmVocDocument docRead;
        docRead.open(fileName);


        
        QList<QmVocContainer *>  lessons = docRead.lesson()->childContainers();
        
        QmVocLesson *m_lesson;
        
        foreach(QmVocContainer * c, lessons) {
            if (c->containerType() == QmVocLesson::Lesson) {
                    m_lesson = static_cast<QmVocLesson *>(c);
            }
        }

        QCOMPARE( docRead.generator(), generator );
        QCOMPARE( docRead.author(), author );
        QCOMPARE( docRead.license(), license );
        QCOMPARE( docRead.documentComment(), comment );
        QCOMPARE( docRead.category(), category );
        QCOMPARE( docRead.title(), title );
        QCOMPARE( docRead.identifierCount(), 2);
        QCOMPARE( docRead.identifier(0).name(), lang1);
        QCOMPARE( docRead.identifier(1).name(), lang2);
        QCOMPARE( docRead.identifier(0).locale(), locale1);
        QCOMPARE( docRead.identifier(1).locale(), locale2);
        QCOMPARE( m_lesson->name(), lessonName);
        QCOMPARE( m_lesson->entry(0)->translationIndices().size(), 2);
        QCOMPARE( m_lesson->entry(0)->translation(0)->text(), translation1);
        QCOMPARE( m_lesson->entry(0)->translation(1)->text(), translation2);
    }
    
};

QTEST_MAIN(RmzTest)

#include "tests.moc"