/***************************************************************************

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

#ifndef TINYVOCTRAINERSETTINGS_H
#define TINYVOCTRAINERSETTINGS_H

#include <QObject>
#include <QMap>
#include <QStringList>

#include "qmvoccontainer.h"

class QmVocLesson;
class QmVocDocument;

class QueryMeeSettings : public QObject
{

Q_OBJECT

public:

    static QueryMeeSettings* instance();
    /*!
     * \brief Returns list of dictionaries ( filenames )
     */
    QStringList dictionaries() const;
    QStringList languages() const;
    QStringList lessons() const;

    QmVocLesson* lesson(int index) const;
    int openedDictionary() const;

public Q_SLOTS:
    /*!
     * \brief Shows file selection dialog
     */
    void openDictionary();

    /*!
     * \brief Opens dictionary by name @see m_Dictionaries
     */
    void openDictionary(const QString& dictionaryName);

Q_SIGNALS:
    /*!
     * \brief emited when dictionary document is changed, e.g. new document
     * is opened
     */
    void dictionaryChanged();

private:
    explicit QueryMeeSettings(QObject *parent = 0);
    static QueryMeeSettings* m_Instance;
    void init();
    void openDictionaryFile(const QString& fileName);
    QList<QmVocContainer*> getChildLessons(QmVocContainer *container);

    QString m_CurrentlyOpenedFile;
    QmVocDocument* m_CurrentDocument;
    QList<QmVocLesson*> m_Lessons;
    QStringList m_Languages;
    // <FileName,Path>
    QMap<QString,QString> m_Dictionaries;
};

#endif // TINYVOCTRAINERSETTINGS_H
