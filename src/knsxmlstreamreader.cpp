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

#include <QtGui>
#include <QtXml>
#include <iostream>

#include "knsxmlstreamreader.h"

KNSXmlStreamReader::KNSXmlStreamReader(QListWidget *list)
{
    listWidget = list;
}

bool KNSXmlStreamReader::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }

    reader.setDevice(&file);

    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "knewstuff") {
                readKnewStuffElement();
            } else {
                reader.raiseError(QObject::tr("Not a kvtml file"));
            }
        } else {
            reader.readNext();
        }
    }

    file.close();

    if (reader.hasError()) {
        std::cerr << "Error: Failed to parse file "
                  << qPrintable(fileName) << ": "
                  << qPrintable(reader.errorString()) << std::endl;
        return false;
    } else if (file.error() != QFile::NoError) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }

    return true;
}

void KNSXmlStreamReader::readKnewStuffElement()
{
    reader.readNext();

    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            if (reader.name() == "stuff") {
                readStuffElement();
            }
        } else {
            reader.readNext();
        }


    }
}


void KNSXmlStreamReader::readStuffElement()
{

    QListWidgetItem *item = new QListWidgetItem();
    listWidget->addItem(item);

    reader.readNext();

    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }



        if (reader.isStartElement()) {
            if (reader.name() == "name") {
                readNameElement(item);
            }
            else if (reader.name() == "summary") {
                readSummaryElement(item);
            }
            else if (reader.name() == "payload") {
                readPayloadElement(item);
            }
            else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}


void KNSXmlStreamReader::readNameElement(QListWidgetItem *item)
{
    QString name = reader.readElementText();
    if (reader.isEndElement())
        reader.readNext();

    item->setText(name);
}

void KNSXmlStreamReader::readSummaryElement(QListWidgetItem *item)
{
    QString summary = reader.readElementText();
    if (reader.isEndElement())
        reader.readNext();

    item->setData(32, summary);
}

void KNSXmlStreamReader::readPayloadElement(QListWidgetItem *item)
{
    QString payload = reader.readElementText();
    if (reader.isEndElement())
        reader.readNext();

    item->setData(33, payload);
}

void KNSXmlStreamReader::skipUnknownElement()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            skipUnknownElement();
        } else {
            reader.readNext();
        }
    }
}
