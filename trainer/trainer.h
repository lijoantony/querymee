#ifndef TRAINER_H
#define TRAINER_H

#include <QtGui/QWidget>

#include "qtvtvocdocument.h"
#include "qtvtvoclesson.h"
#include "qtvtvocexpression.h"
#include "qtvtvocleitnerbox.h"

class Trainer : public QWidget
{
    Q_OBJECT

public:
    Trainer(QWidget *parent = 0);
    ~Trainer();
};

#endif // TRAINER_H
