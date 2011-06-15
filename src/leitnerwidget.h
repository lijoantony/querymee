#ifndef LEITNERWIDGET_H
#define LEITNERWIDGET_H

#include <QWidget>
#include <QList>
#include <QProgressBar>

class LeitnerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeitnerWidget(QWidget *parent = 0);

signals:

public slots:
    void updateProgressBar(int, int);
    void updateMaxProgressBar(int, int);

private:
    QList<QProgressBar*> *progressBarsList;
};

#endif // LEITNERWIDGET_H
