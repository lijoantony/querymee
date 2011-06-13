#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "qmvoclesson.h"
#include "querymeesettings.h"
#include "lessonchoosewidget.h"

LessonChooseWidget::LessonChooseWidget(QWidget *parent, QueryMeeSettings *settings) :
    QWidget(parent)
{
    m_settings = settings;
    QVBoxLayout *vbox = new QVBoxLayout(this);
    QHBoxLayout *hbox = new QHBoxLayout(this);

    QPushButton *doneButton = new QPushButton(this);
    doneButton->setText(tr("Done"));
    hbox->addWidget(doneButton);

    QPushButton *selectAllButton = new QPushButton(this);
    selectAllButton->setText(tr("Select All"));
    hbox->addWidget(selectAllButton);



    vbox->addLayout(hbox);

    listWidget = new QListWidget(this);
    listWidget->setSortingEnabled(true);
    listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(selectAllButton,
            SIGNAL(clicked()),
            listWidget,
            SLOT(selectAll()));


    vbox->addWidget(listWidget);

    updateListWidget();

    setLayout(vbox);

}

void LessonChooseWidget::updateListWidget(){
    foreach (QString lesson, m_settings->lessons()){
        QListWidgetItem *item = new QListWidgetItem();
        listWidget->addItem(item);
        item->setText(lesson);
    }
}
