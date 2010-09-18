/***************************************************************************
*   this file is from kdeedu project. Filename: keduvocdocument.cpp
***************************************************************************/

/***************************************************************************
                        Vocabulary Document for KDE Edu
    -----------------------------------------------------------------------
    copyright      : (C) 1999-2001 Ewald Arnold <kvoctrain@ewald-arnold.de>

                     (C) 2005-2007 Peter Hedlund <peter.hedlund@kdemail.net>
                     (C) 2007 Frederik Gladhorn <frederik.gladhorn@kdemail.net>
                     (C) 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qmvocdocument.h"

#include <QtCore/QFileInfo>
// #include <QtCore/QTextStream>
#include <QtCore/QtAlgorithms>
#include <QtCore/QIODevice>
#include <QTextStream>
#include <QFile>

// #include <klocale.h>
// #include <kdebug.h>
#include <QDebug>
// #include <kio/netaccess.h>
// #include <krandomsequence.h>
// #include <kfilterdev.h>

#include "qmvocexpression.h"
#include "qmvoclesson.h"
#include "qmvocleitnerbox.h"
#include "qmvocwordtype.h"
#include "qmvockvtmlwriter.h"
#include "qmvockvtml2writer.h"
#include "qmvoccsvreader.h"
#include "qmvoccsvwriter.h"
#include "qmvockvtml2reader.h"
#include "qmvocwqlreader.h"
#include "qmvocpaukerreader.h"
#include "qmvocvokabelnreader.h"
#include "qmvocxdxfreader.h"

#define WQL_IDENT      "WordQuiz"

#define KVTML_EXT        "kvtml"
#define CSV_EXT          "csv"
#define TXT_EXT          "txt"
#define WQL_EXT          "wql"

class QmVocDocument::QmVocDocumentPrivate
{
public:
    QmVocDocumentPrivate( QmVocDocument* qq )
            : q( qq )
    {
        m_lessonContainer = 0;
        m_wordTypeContainer = 0;
        m_leitnerContainer = 0;
        init();
    }

    ~QmVocDocumentPrivate();

    void init();

    QmVocDocument* q;

    bool                      m_dirty;
    QUrl                      m_url;

    // save these to document
    QList<QmVocIdentifier>  m_identifiers;

    QList<int>                m_extraSizeHints;
    QList<int>                m_sizeHints;

    QString                   m_generator;
    QString                   m_queryorg;
    QString                   m_querytrans;

    QStringList               m_tenseDescriptions;
    QSet<QString>             m_usages;

    QString                   m_title;
    QString                   m_author;
    QString                   m_authorContact;
    QString                   m_license;
    QString                   m_comment;
    QString                   m_version;
    QString                   m_csvDelimiter;

    /** Categories that can later be used to sork kvtml files:
      * language, music, children, anatomy
      */
    QString                   m_category;

    QmVocLesson * m_lessonContainer;
    QmVocWordType * m_wordTypeContainer;
    QmVocLeitnerBox * m_leitnerContainer;
};

QmVocDocument::QmVocDocumentPrivate::~QmVocDocumentPrivate()
{
    delete m_lessonContainer;
    delete m_wordTypeContainer;
    delete m_leitnerContainer;
}

void QmVocDocument::QmVocDocumentPrivate::init()
{
    delete m_lessonContainer;
    m_lessonContainer = new QmVocLesson( "Document Lesson");
    m_lessonContainer->setContainerType(QmVocLesson::Lesson);
    delete m_wordTypeContainer;
    m_wordTypeContainer = new QmVocWordType( "Word types" );

    if ( m_leitnerContainer ) {
        delete m_leitnerContainer;
    }
    m_leitnerContainer = new QmVocLeitnerBox( "Leitner Box" );

    m_tenseDescriptions.clear();
    m_identifiers.clear();
    m_extraSizeHints.clear();
    m_sizeHints.clear();
    m_dirty = false;
    m_queryorg = "";
    m_querytrans = "";
//    m_url.setFileName( "Untitled" );
    // FIXME: is this what we need?
    m_url.setPath( "Untitled" );
    m_author = "";
    m_title = "";
    m_comment = "";
    m_version = "";
    m_generator = "";
    m_csvDelimiter = QString( '\t' );
    m_usages.clear();
    m_license.clear();
    m_category.clear();
}


QmVocDocument::QmVocDocument( QObject *parent )
        : QObject( parent ), d( new QmVocDocumentPrivate( this ) )
{

}


QmVocDocument::~QmVocDocument()
{
    delete d;
}


void QmVocDocument::setModified( bool dirty )
{
    d->m_dirty = dirty;
    emit docModified( d->m_dirty );
}


QmVocDocument::FileType QmVocDocument::detectFileType( const QString &fileName )
{
//     QIODevice * f = KFilterDev::deviceForFile( fileName );
    QFile f(fileName);
    if ( !f.open( QIODevice::ReadOnly ) ) {
        qDebug() << "Warning, could not open QIODevice for file: " << fileName;
        // FIXME: do we need to delete f????
        // delete f;
        return Csv;
    }

    QTextStream ts( &f );
    QString line1;
    QString line2;

    line1 = ts.readLine();
    if ( !ts.atEnd() ) {
        line2 = ts.readLine();
    }

    /*
     * Vokabeln.de files:
    The header seems to be something like this:

    "Name
    Lang1 - Lang2",123,234,456
    0

    or something longer:

    "Name
    Lang1 - Lang2
    [..]
    Blah, blah, blah...",123,234,456
    0
    */

    QString tmp;

    if ( line1.startsWith(QChar::fromLatin1('"'))) {
        ts.seek(0);
        tmp = ts.readLine();
        // There shouldn't be headers longer than 10 lines.
        for ( int x=0; x < 10; x++) {
            if (tmp.contains( "\"," )) {
                tmp = ts.readLine();
                if (tmp.endsWith('0')) {
                    f.close();
                    // FIXME: delete f ???
                    // delete f;
                    return Vokabeln;
                }
            }
            tmp = ts.readLine();
        }
    }
    f.close();
    // FIXME: delete f ???
    // delete f;


    if ( line1.startsWith(QString::fromLatin1("<?xml")) ) {
        if ( line2.indexOf( "pauker", 0 ) >  0 ) {
            return Pauker;
        }
        if ( line2.indexOf( "xdxf", 0 ) >  0 ) {
            return Xdxf;
        } else {
            return Kvtml;
        }
    }

    if ( line1 == WQL_IDENT ) {
        return Wql;
    }

     return Csv;
}


int QmVocDocument::open( const QUrl& url )
{
    // temp solution for KUrl -> QString
    QString myFile = url.path();
    
    // save csv delimiter to preserve it in case this is a csv document
    QString csv = d->m_csvDelimiter;
    // clear all other properties
    d->init();
    if ( !url.isEmpty() ) {
        d->m_url = url;
    }
    d->m_csvDelimiter = csv;

    bool read = false;
    QString errorMessage = QString("<qt>Cannot open file<br /><b>%1</b></qt>").arg(url.path()) ;

//    QString temporaryFile;
//     if ( KIO::NetAccess::download( url, temporaryFile, 0 ) ) {


//         QIODevice * f = KFilterDev::deviceForFile( temporaryFile );
// 
    QFile *f = new QFile(myFile);
    if ( !f->open( QIODevice::ReadOnly ) ) {
        qCritical() << errorMessage;
        delete f;
        return FileCannotRead;
    }
// 
//         FileType ft = detectFileType( temporaryFile );
    FileType ft = detectFileType(myFile);

        switch ( ft ) {
            case Kvtml: {
                QmVocKvtml2Reader kvtmlReader( f );
                read = kvtmlReader.readDoc( this );
                if ( !read ) {
                    errorMessage = kvtmlReader.errorMessage();
                }
            }
            break;

            // FIXME: maybe we enable them later
            
            case Wql: {
                qDebug() << "Reading WordQuiz (WQL) document...";
                qCritical ("WordQuiz (WQL) document not yet supported...");
                errorMessage = "WordQuiz (WQL) document not yet supported...";
//                 QmVocWqlReader wqlReader( f );
//                 d->m_url.setFileName( "Untitled" );
//                 read = wqlReader.readDoc( this );
//                 if ( !read ) {
//                     errorMessage = wqlReader.errorMessage();
//                 }
            }
            break;

            case Pauker: {
                qDebug() << "Reading Pauker document...";
                qCritical ("Pauker document not yet supported...");
                errorMessage = "Pauker document not yet supported...";
//                 QmVocPaukerReader paukerReader( this );
//                 d->m_url.setFileName( "Untitled" );
//                 read = paukerReader.read( f );
//                 if ( !read ) {
//                     errorMessage = "Parse error at line %1, column %2:\n%3", paukerReader.lineNumber(), paukerReader.columnNumber(), paukerReader.errorString();
//                 }
            }
            break;

            case Vokabeln: {
                qDebug() << "Reading Vokabeln document...";
//                qCritical ("Vokabeln document not yet supported...");
//                errorMessage = "Vokabeln document not yet supported...";
                 QmVocVokabelnReader vokabelnReader( f );
                 // d->m_url.setFileName( "Untitled" );
                 read = vokabelnReader.readDoc( this );
                 if ( !read ) {
                     errorMessage = vokabelnReader.errorMessage();
                 }
            }
            break;

            case Csv: {
                qDebug() << "Reading CVS document...";
//                qCritical ("CVS document not yet supported...");
//                errorMessage = "CVS document not yet supported...";
                 QmVocCsvReader csvReader( f );
                 read = csvReader.readDoc( this );
                 if ( !read ) {
                     errorMessage = csvReader.errorMessage();
                 }
            }
            break;

            case Xdxf: {
                qDebug() << "Reading XDXF document...";
                qCritical ("XDXF document not yet supported...");
                errorMessage = "XDXF document not yet supported...";
//                 QmVocXdxfReader xdxfReader( this );
//                 d->m_url.setFileName(  "Untitled"  );
//                 read = xdxfReader.read( f );
//                 if ( !read ) {
//                     errorMessage = "Parse error at line %1, column %2:\n%3", xdxfReader.lineNumber(), xdxfReader.columnNumber(), xdxfReader.errorString() ;
//                 }
            }
            break;

            default: {
                QmVocKvtml2Reader kvtmlReader( f );
                read = kvtmlReader.readDoc( this );
                if ( !read ) {
                    errorMessage = kvtmlReader.errorMessage();
                }
            }
        }

        if ( !read ) {
            QString msg =  QString("Could not open or properly read \"%1\"\n(Error reported: %2)").arg(url.path()).arg(errorMessage);
            qCritical() << msg <<  "Error Opening File" ;
            ///@todo make the readers return int, pass on the error message properly
            delete f;
            return FileReaderFailed;
        }

        f->close();
        delete f;
        
//         KIO::NetAccess::removeTempFile( temporaryFile );


//     } // close if

    if ( !read ) {
        return FileReaderFailed;
    }

    setModified(false);
    return 0;
}


int QmVocDocument::saveAs( const QUrl & url, FileType ft, const QString & generator )
{
    QUrl tmp( url );

    if ( ft == Automatic ) {
        if ( tmp.path().right( strlen( "." KVTML_EXT ) ) == "." KVTML_EXT )
            ft = Kvtml;
        else if ( tmp.path().right( strlen( "." CSV_EXT ) ) == "." CSV_EXT )
            ft = Csv;
        else {
            return FileTypeUnknown;
        }
    }

    QFile f( tmp.path() );

    if ( !f.open( QIODevice::WriteOnly ) ) {
        qCritical() <<  QString("Cannot write to file %1").arg(tmp.path()) ;
        return FileCannotWrite;
    }

    bool saved = false;

    switch ( ft ) {
        case Kvtml: {
            // write version 2 file
            QmVocKvtml2Writer kvtmlWriter( &f );
            saved = kvtmlWriter.writeDoc( this, generator );
        }
        break;
        ///@todo port me
//         case Kvtml1: {
//             // write old version 1 file
//             QmVocKvtmlWriter kvtmlWriter( &f );
//             saved = kvtmlWriter.writeDoc( this, generator );
//         }
//         break;
        case Csv: {
            QmVocCsvWriter csvWriter( &f );
            saved = csvWriter.writeDoc( this, generator );
        }
        break;
        default: {
            qCritical() << "kvcotrainDoc::saveAs(): unknown filetype" << endl;
        }
        break;
    } // switch

    f.close();

    if ( !saved ) {
        qCritical() << "Error Saving File" << tmp.path();
        return FileWriterFailed;
    }

    d->m_url = tmp;
    setModified( false );
    return 0;
}

QByteArray QmVocDocument::toByteArray(const QString &generator)
{
    // no file needed
    QmVocKvtml2Writer kvtmlWriter(0);
    return kvtmlWriter.toByteArray( this, generator );
}

void QmVocDocument::merge( QmVocDocument *docToMerge, bool matchIdentifiers )
{
    Q_UNUSED(docToMerge)
    Q_UNUSED(matchIdentifiers)
    qDebug() << "Merging of docs is not implemented"; /// @todo IMPLEMENT ME
    // This code was really horribly broken.
    // Now with the new classes we could attempt to reactivate it.
    // A rewrite might be easier.

    /*
      if (docToMerge) {

        QStringList new_names = docToMerge->lessonDescriptions();

        QStringList new_types = docToMerge->typeDescriptions();

        QStringList new_tenses = docToMerge->tenseDescriptions();

        QList<int> old_in_query = lessonsInPractice();
        QList<int> new_in_query = docToMerge->lessonsInPractice();

        QStringList new_usages = docToMerge->usageDescriptions();

        int lesson_offset = d->m_lessonDescriptions.count();
        for (int i = 0; i < new_names.count(); i++) {
          d->m_lessonDescriptions.append(new_names[i]);
        }

        for (int i = 0; i < new_in_query.count(); i++)
          old_in_query.append(new_in_query[i] + lesson_offset);
        setLessonsInPractice(old_in_query);

        int types_offset = d->m_typeDescriptions.count();
        for (int i = 0; i < new_types.count(); i++) {
          d->m_typeDescriptions.append(new_types[i]);
        }

        int tenses_offset = d->m_tenseDescriptions.count();
        for (int i = 0; i < new_tenses.count(); i++) {
          d->m_tenseDescriptions.append(new_tenses[i]);
        }

        int usages_offset = d->m_usageDescriptions.count();
        for (int i = 0; i < new_usages.count(); i++) {
          d->m_usageDescriptions.append(new_usages[i]);
        }

        bool equal = true;
        if (originalIdentifier() != docToMerge->originalIdentifier())
          equal = false;
        for (int i = 1; i < identifierCount(); i++)
          if (identifier(i) != docToMerge->identifier(i))
            equal = false;

        if (!matchIdentifiers)
          equal = true; ///@todo massive cheating, problem if docToMerge has more identifiers than this

        if (equal) {   // easy way: same language codes, just append

          for (int i = 0; i < docToMerge->entryCount(); i++) {
            QmVocExpression *expr = docToMerge->entry(i);

            expr->setLesson(expr->lesson() + lesson_offset);

            for (int lang = 0; lang <= expr->translationCount(); lang++) {
              QString t = expr->translation(lang).type();
              // adjust type offset
              if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
                QString t2;
                t.remove(0, 1);
                t2.setNum(t.toInt() + types_offset);
                t2.prepend(QM_USER_TYPE);
                expr->translation(lang).setType (t2);
              }

              t = expr->translation(lang).usageLabel();
              // adjust usage offset
              QString tg;
              if (!t.isEmpty()) {
                QString t2;
                while (t.left(strlen(":")) == UL_USER_USAGE) {
                  QString n;
                  t.remove(0, 1);
                  int next;
                  if ((next = t.indexOf(":")) >= 0) {
                    n = t.left(next);
                    t.remove(0, next + 1);
                  }
                  else {
                    n = t;
                    t = "";
                  }

                  t2.setNum(n.toInt() + usages_offset);
                  t2.prepend(UL_USER_USAGE);
                  if (tg.length() == 0)
                    tg = t2;
                  else
                    tg += ':' + t2;
                }

                if (tg.length() == 0)
                  tg = t;
                else if (t.length() != 0)
                  tg += ':' + t;

                expr->translation(lang).setUsageLabel (tg);
              }

              QmVocConjugation conj = expr->translation(lang).conjugation();
              bool condirty = false;
              for (int ci = 0; ci < conj.entryCount(); ci++) {
                t = conj.getType(ci);
                if (!t.isEmpty() && t.left(1) == UL_USER_TENSE) {
                  t.remove(0, strlen(UL_USER_TENSE));
                  QString t2;
                  t2.setNum(t.toInt() + tenses_offset);
                  t2.prepend(UL_USER_TENSE);
                  conj.setType(ci, t2);
                  condirty = true;
                }
                if (condirty)
                  expr->translation(lang).setConjugation(conj);
              }
            }

            appendEntry(expr);
          }
          setModified();
        }
        else { // hard way: move entries around, most attributes get lost
          QList<int> move_matrix;
          QList<bool> cs_equal;
          QString s;

          for (int i = 0; i < qMax (identifierCount(), docToMerge->identifierCount()); i++)
            cs_equal.append(false);

          move_matrix.append(docToMerge->indexOfIdentifier(originalIdentifier()));
          for (int i = 1; i < identifierCount(); i++)
            move_matrix.append(docToMerge->indexOfIdentifier(identifier(i)));

          for (int j = 0; j < docToMerge->entryCount(); j++) {
            QmVocExpression new_expr;
            QmVocExpression *expr = docToMerge->entry(j);
            new_expr.setLesson(expr->lesson()+lesson_offset);

            for (int i = 0; i < move_matrix.count(); i++) {
              int lpos = move_matrix[i];
              if (lpos >= 0) {

                if (lpos == 0)
                  s = expr->original();
                else
                  s = expr->translation(lpos);

                if (!cs_equal[lpos]) {
                  cs_equal[lpos] = true;
                  QString id = lpos == 0 ? originalIdentifier() : identifier(lpos);
                }

                if (i == 0)
                  new_expr.setOriginal(s);
                else
                  new_expr.setTranslation(i, s);
                QString r = expr->remark(lpos);
                new_expr.setRemark (i, r);

                QString t = expr->type(lpos);
                if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
                  QString t2;
                  t.remove(0, 1);
                  t2.setNum(t.toInt() + types_offset);
                  t2.prepend(QM_USER_TYPE);
                  new_expr.setType(i, t2);
                }

                t = expr->usageLabel(lpos);
                if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
                  QString t2;
                  t.remove(0, 1);
                  t2.setNum(t.toInt() + usages_offset);
                  t2.prepend(QM_USER_TYPE);
                  new_expr.setUsageLabel(i, t2);
                }

                QmVocConjugation conj = expr->conjugation(lpos);
                for (int ci = 0; ci < conj.entryCount(); ci++) {
                  t = conj.getType(ci);
                  if (!t.isEmpty() && t.left(1) == QM_USER_TYPE) {
                    t.remove (0, strlen(QM_USER_TYPE));
                    QString t2;
                    t2.setNum(t.toInt() + tenses_offset);
                    t2.prepend(QM_USER_TYPE);
                    conj.setType(ci, t2);
                  }
                }

              }
            }
            // only append if entries are used
            bool used = !new_expr.original().isEmpty();
            for (int i = 1; i < identifierCount(); i++)
              if (!new_expr.translation(i).isEmpty())
                used = true;

            if (used) {
              appendEntry(&new_expr);
              setModified();
            }
          }
        }
      }
    */
}

const QmVocIdentifier& QmVocDocument::identifier( int index ) const
{
    if ( index < 0 || index >= d->m_identifiers.size() ) {
        qCritical() << " Error: Invalid identifier index: " << index;
    }
    return d->m_identifiers[index];
}

QmVocIdentifier& QmVocDocument::identifier( int index )
{
    if ( index < 0 || index >= d->m_identifiers.size() ) {
        qCritical() << " Error: Invalid identifier index: " << index;
    }
    return d->m_identifiers[index];
}

void QmVocDocument::setIdentifier( int idx, const QmVocIdentifier &id )
{
    if ( idx >= 0 && idx < d->m_identifiers.size() ) {
        d->m_identifiers[idx] = id;
    }
    setModified(true);
}

// works if const is removed
int QmVocDocument::indexOfIdentifier( const QString &name ) const
{
    for (int i = 0; i < identifierCount(); i++)
        if (identifier(i).locale() == name)
            return i;
    return -1;
}

void QmVocDocument::removeIdentifier( int index )
{
    if ( index < d->m_identifiers.size() && index >= 0 ) {
        d->m_identifiers.removeAt( index );
        d->m_lessonContainer->removeTranslation( index );
    }
}


bool QmVocDocument::isModified() const
{
    return d->m_dirty;
}


int QmVocDocument::identifierCount() const
{
    return d->m_identifiers.count();  // number of translations
}

int QmVocDocument::appendIdentifier( const QmVocIdentifier& id )
{
    int i = d->m_identifiers.size();
    d->m_identifiers.append( id );
    if ( id.name().isEmpty() ) {
        if ( i == 0 ) {
            identifier(i).setName( "Original");
        } else {
            identifier(i).setName( QString("Translation %1").arg(i) );
        }
    }


    return i;
}

QmVocLesson * QmVocDocument::lesson()
{
    return d->m_lessonContainer;
}

QmVocWordType * QmVocDocument::wordTypeContainer()
{
    return d->m_wordTypeContainer;
}

QmVocLeitnerBox * QmVocDocument::leitnerContainer()
{
    return d->m_leitnerContainer;
}

QUrl QmVocDocument::url() const
{
    return d->m_url;
}

void QmVocDocument::setUrl( const QUrl& url )
{
    d->m_url = url;
}

QString QmVocDocument::title() const
{
    if ( d->m_title.isEmpty() )
        return d->m_url.path();
    else
        return d->m_title;
}

void QmVocDocument::setTitle( const QString & title )
{
    d->m_title = title;
    d->m_lessonContainer->setName(title);
    setModified(true);
}

QString QmVocDocument::author() const
{
    return d->m_author;
}

void QmVocDocument::setAuthor( const QString & s )
{
    d->m_author = s.simplified();
    setModified(true);
}

QString QmVocDocument::authorContact() const
{
    return d->m_authorContact;
}

void QmVocDocument::setAuthorContact( const QString & s )
{
    d->m_authorContact = s.simplified();
    setModified(true);
}

QString QmVocDocument::license() const
{
    return d->m_license;
}

QString QmVocDocument::documentComment() const
{
    return d->m_comment;
}

void QmVocDocument::setCategory( const QString & category )
{
    d->m_category = category;
    setModified(true);
}

QString QmVocDocument::category() const
{
    return d->m_category;
    ///@todo make writer/reader use this
}

void QmVocDocument::queryIdentifier( QString &org, QString &trans ) const
{
    org = d->m_queryorg;
    trans = d->m_querytrans;
}

void QmVocDocument::setQueryIdentifier( const QString &org, const QString &trans )
{
    d->m_queryorg = org;
    d->m_querytrans = trans;
    setModified(true);
}

void QmVocDocument::setLicense( const QString & s )
{
    d->m_license = s.simplified();
    setModified(true);
}

void QmVocDocument::setDocumentComment( const QString & s )
{
    d->m_comment = s.trimmed();
    setModified(true);
}

void QmVocDocument::setGenerator( const QString & generator )
{
    d->m_generator = generator;
    setModified(true);
}

QString QmVocDocument::generator() const
{
    return d->m_generator;
}

QString QmVocDocument::version() const
{
    return d->m_version;
}

void QmVocDocument::setVersion( const QString & vers )
{
    d->m_version = vers;
    setModified(true);
}

QString QmVocDocument::csvDelimiter() const
{
    return d->m_csvDelimiter;
}

void QmVocDocument::setCsvDelimiter( const QString &delimiter )
{
    d->m_csvDelimiter = delimiter;
    setModified(true);
}


QString QmVocDocument::pattern( FileDialogMode mode )
{
    static const struct SupportedFilter {
        bool reading;
        bool writing;
        const char* extensions;
        const char* description;
    }

    // removed filters
    //                    { true, false, "*.wql", "KWordQuiz Document"  },
    //                    { true, false, "*.xml.gz *.pau.gz", "Pauker Lesson" },
    //                    { true, false, "*.xdxf", "XML Dictionary Exchange Format" },

    filters[] = {
                    { true, true, "*.kvtml", "KDE Vocabulary Document" },
                    { true, false, "*.voc", "Vokabeltrainer" },
                    { true, true, "*.tsv", "Tab Separated Values (TSV)" },
                    // last is marker for the end, do not remove it
                    { false, false, 0, 0 }
                };
    QStringList newfilters;
    QStringList allext;
    for ( int i = 0; filters[i].extensions; ++i ) {
        if (( mode == Reading && filters[i].reading ) ||
                ( mode == Writing && filters[i].writing ) ) {
            newfilters.append( QLatin1String( filters[i].extensions ) + '|' +  filters[i].description );
            allext.append( QLatin1String( filters[i].extensions ) );
        }
    }
    if ( mode == Reading ) {
        newfilters.prepend( allext.join( " " ) + '|' +  "All supported documents" );
    }
    return newfilters.join( "\n" );
}

QString QmVocDocument::errorDescription( int errorCode )
{
    switch (errorCode) {
    case NoError:
        return "No error found.";

    case InvalidXml:
        return "Invalid XML in document.";
    case FileTypeUnknown:
        return "Unknown file type.";
    case FileCannotWrite:
        return "File is not writeable.";
    case FileWriterFailed:
        return "File writer failed.";
    case FileCannotRead:
        return "File is not readable.";
    case FileReaderFailed:
        return "The file reader failed.";
    case FileDoesNotExist:
        return "The file does not exist.";
    case Unknown:
    default:
        return "Unknown error.";
    }
}


