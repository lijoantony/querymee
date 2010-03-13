
#include <QtTest/QtTest>


#include "keduvocdocument.h"
#include "keduvoclesson.h"
#include "keduvocexpression.h"

#include <QTemporaryFile>


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
        
        KEduVocDocument doc;
        doc.setAuthor( author );
        doc.setLicense( license );
        doc.setDocumentComment( comment );
        doc.setCategory( category );
        doc.setTitle( title );
        
        doc.appendIdentifier();
        doc.appendIdentifier();
        doc.identifier(0).setName("0");
        doc.identifier(1).setName("1");
        
        KEduVocLesson* lesson = new KEduVocLesson("lesson", doc.lesson());
        doc.lesson()->appendChildContainer(lesson);
        lesson->appendEntry(new KEduVocExpression);
        lesson->entry(0)->setTranslation(0, "0");
        lesson->entry(0)->setTranslation(1, "1");
        
        doc.saveAs(fileName, KEduVocDocument::Kvtml, generator);
        
        KEduVocDocument docRead;
        docRead.open(fileName);
        
        QCOMPARE( docRead.generator(), generator );
        QCOMPARE( docRead.author(), author );
        QCOMPARE( docRead.license(), license );
        QCOMPARE( docRead.documentComment(), comment );
        QCOMPARE( docRead.category(), category );
        QCOMPARE( docRead.title(), title );
        QCOMPARE(doc.identifierCount(), 2);
        QCOMPARE(lesson->entry(0)->translationIndices().size(), 2);
        QCOMPARE(lesson->entry(0)->translation(0)->text(), QString("0"));
        QCOMPARE(lesson->entry(0)->translation(1)->text(), QString("1"));
        
    }
    
};

QTEST_MAIN(RmzTest)

#include "tests.moc"