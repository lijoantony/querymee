#include "trainer.h"

#include <QApplication>
#include <QtDebug>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QList>

Trainer::Trainer(QWidget *parent)
    : QWidget(parent)
{
    QTableWidget *tableWidget = new QTableWidget(this);


    QStringList args = QApplication::arguments();
    args.removeFirst();

    QTvtVocDocument docRead;
    docRead.open(args.at(0));

    QList<QTvtVocContainer *>  lessons = docRead.lesson()->childContainers();

    QTvtVocLesson *m_lesson;

    foreach(QTvtVocContainer * c, lessons) {
        if (c->containerType() == QTvtVocLesson::Lesson) {
                m_lesson = static_cast<QTvtVocLesson *>(c);
                qDebug () << "Lesson: " << m_lesson->name();
                for(int i = 0; i < m_lesson->entries().size(); ++i)
                {
                    tableWidget->insertRow(i);
                    for( int j = 0; j < m_lesson->entry(i)->translationIndices().size(); ++j )
                    {
                        qDebug() << "columnCount: " << tableWidget->columnCount() << "j: " << j;
                        if (tableWidget->columnCount() <= j)
                        {
                            tableWidget->insertColumn(j);
                        }
                        qDebug() << "Entry: " << m_lesson->entry(i)->translation(j)->text() << "Identifier: " << docRead.identifier(j).name();
                        QTableWidgetItem *newItem = new QTableWidgetItem(m_lesson->entry(i)->translation(j)->text());
                        tableWidget->setItem(i, j, newItem);
                    }
                }
        }
    }

    QList<QString> labels;
    for( int i = 0; i < docRead.identifierCount(); ++i )
    {
        qDebug() << "Identifier: " << docRead.identifier(i).name() << " i: " << i;
        labels.insert(i, docRead.identifier(i).name());
    }

    tableWidget->setHorizontalHeaderLabels(labels);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(tableWidget);
    setLayout(layout);
}

Trainer::~Trainer()
{

}
