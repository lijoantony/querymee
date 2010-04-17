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

class QTvtVocLesson;
class QTvtVocDocument;

class TinyVocTrainerSettings : public QObject
{

Q_OBJECT

public:

    static TinyVocTrainerSettings* instance();
    /*!
     * \brief Shows file selection dialog
     */
    void openDictionary();

    /*!
     * \brief Opens dictionary by name @see m_Dictionaries
     */
    void openDictionary(const QString& dictionaryName);

    /*!
     * \brief Returns list of dictionaries ( filenames )
     */
    QStringList dictionaries();
    QStringList languages();
    QStringList lessons();

    QTvtVocLesson* lesson(int index);

Q_SIGNALS:
    /*!
     * \brief emited when dictionary document is changed, e.g. new document
     * is opened
     */
    void dictionaryChanged();

private:
    explicit TinyVocTrainerSettings(QObject *parent = 0);
    static TinyVocTrainerSettings* m_Instance;
    void init();

private:
    void openDictionaryFile(const QString& fileName);

private:
    QString m_CurrentlyOpenedFile;
    QTvtVocDocument* m_CurrentDocument;
    QList<QTvtVocLesson*> m_Lessons;
    QStringList m_Languages;
    // <FileName,Path>
    QMap<QString,QString> m_Dictionaries;
};

#endif // TINYVOCTRAINERSETTINGS_H
