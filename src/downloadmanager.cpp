/***************************************************************************

    Copyright 2010 Reto Zingg <g.d0b3rm4n@gmail.com>

    the following methods are based on the examples of the Qt Toolkit see
    the follwing licence...

 ***************************************************************************/

/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "downloadmanager.h"

#include "querymee_defaults.h"


DownloadManager::DownloadManager()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}

void DownloadManager::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

    currentDownloads.append(reply);
}

QString DownloadManager::saveFileName(const QUrl &url)
{
    QString savePathName;
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    QString settingsPath = QDir::homePath() + QString(DICTIONARY_PATH);

    if (basename.isEmpty())
        basename = "download";

    savePathName.append(settingsPath);
    savePathName.append(basename);

    return savePathName;
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open" << qPrintable(filename) << "for writing:" << qPrintable(file.errorString());
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

void DownloadManager::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();

    if (reply->error()) {
        qDebug() << "Download of" << url.toEncoded().constData() << "failed:" << qPrintable(reply->errorString());
        emit downloadFailed( url.toEncoded() );
    } else if(url.host() == "opendesktop.org" && url.hasQuery()){
        QByteArray refresh = reply->rawHeader("Refresh");
        int equalIndex = refresh.indexOf("=");
        if ( equalIndex > 0 ){
            refresh.remove(0, equalIndex + 1);
            QUrl realurl = QUrl("http://opendesktop.org/");
            realurl.setPath(refresh);
            this->doDownload(realurl);
        }
    }
    else {
        QString filename = saveFileName(url);
        if (saveToDisk(filename, reply)){
            // qDebug() << "Download of" << url.toEncoded().constData() << "succeded saved to:"  << qPrintable(filename);
        }
    }

    currentDownloads.removeAll(reply);
    reply->deleteLater();

    if (currentDownloads.isEmpty()){
        // all downloads finished
        emit done();
    }

}
