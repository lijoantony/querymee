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
#include <QListWidget>

#include "downloadwidget.h"

#include "knsxmlstreamreader.h"
#include "ocsxmlstreamreader.h"

#include "downloadmanager.h"

#include "querymee_defaults.h"

DownloadWidget::DownloadWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *hbox = new QHBoxLayout(this);

    QPushButton *downloadButton = new QPushButton(this);
    downloadButton->setText(tr("Download"));
    hbox->addWidget(downloadButton);

    connect(downloadButton,
            SIGNAL(clicked()),
            this,
            SLOT(slotDownloadButton()));

    QPushButton *upDateButton = new QPushButton(this);
    upDateButton->setText(tr("update list"));
    hbox->addWidget(upDateButton);

    connect(upDateButton,
            SIGNAL(clicked()),
            this,
            SLOT(slotUpDateButton()));

    vbox->addLayout(hbox);

    listWidget = new QListWidget(this);
    listWidget->setSortingEnabled(true);
    listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    vbox->addWidget(listWidget);

    connect(listWidget,
            SIGNAL(itemClicked(QListWidgetItem*)),
            this,
            SLOT(slotItemClicked(QListWidgetItem*)));

    updateListWidget();

    setLayout(vbox);

}

void DownloadWidget::slotDownloadButton(){

    QList<QListWidgetItem *> selected = listWidget->selectedItems();

    DownloadManager *dm = new DownloadManager();

    for (int i=0; i < selected.size(); ++i){

        dm->doDownload( QUrl(selected.at(i)->data(33).toString() ) );
    }

    connect(dm,
            SIGNAL(done()),
            this,
            SLOT(slotDownloadDone()));

}

void DownloadWidget::slotItemClicked(QListWidgetItem *item){

    if ( item->isSelected() ){
        QMessageBox::information(this,
                                 tr("Summary"),
                                item->data(32).value<QString>(),
                                QMessageBox::Ok);
    }

}

void DownloadWidget::slotUpDateButton(){

    QList<QUrl> urls;
    urls.append(QUrl("http://edu.kde.org/contrib/kvtml/kvtml.xml"));
    urls.append(QUrl("http://edu.kde.org/contrib/kvtml2/provider41.xml"));
    urls.append(QUrl("http://api.opendesktop.org/v1/content/data?categories=687x694x693&pagesize=100"));

    DownloadManager *dm = new DownloadManager();

    for (int i = 0; i < urls.count(); ++i)
        dm->doDownload(urls.at(i));

    connect(dm,
            SIGNAL(done()),
            this,
            SLOT(slotDownloadDone()));


}

void DownloadWidget::slotDownloadDone(){
    qDebug() << "slotDownloadDone";

    QMessageBox::information(this,
                             tr("Download Manager"),
                             tr("all downloads finished..."),
                             QMessageBox::Ok);

    updateListWidget();

    emit finished();

}

void DownloadWidget::updateListWidget(){
    qDebug() << "updateListWidget";

    QStringList files;

    files.append("./kvtml.xml");
    files.append("./provider41.xml");

    listWidget->clear();

    KNSXmlStreamReader reader(listWidget);

    QString Path = QDir::homePath() + QString(DICTIONARY_PATH);

    for (int i = 0; i < files.count(); ++i)
        reader.readFile( Path + files[i]);

    OCSXmlStreamReader ocsreader(listWidget);

    ocsreader.readFile( Path + "data");

}

