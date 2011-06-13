#ifndef LESSONCHOOSEWIDGET_H
#define LESSONCHOOSEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include "querymeesettings.h"

class LessonChooseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LessonChooseWidget(QWidget *parent = 0, QueryMeeSettings *settings = 0);

signals:

public slots:

private:
    QListWidget *listWidget;
    QueryMeeSettings *m_settings;
    void updateListWidget();

};

#endif // LESSONCHOOSEWIDGET_H
