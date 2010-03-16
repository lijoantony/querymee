#ifndef TRAINER_H
#define TRAINER_H

#include <QtGui/QWidget>

#include "keduvocdocument.h"
#include "keduvoclesson.h"
#include "keduvocexpression.h"
#include "keduvocleitnerbox.h"

class Trainer : public QWidget
{
    Q_OBJECT

public:
    Trainer(QWidget *parent = 0);
    ~Trainer();
};

#endif // TRAINER_H
