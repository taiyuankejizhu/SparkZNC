#ifndef MESGBOX_H
#define MESGBOX_H

#include "qcheck.h"
#include "qcombox.h"
#include "qcomboxtime.h"
#include "qindicate.h"
#include "qparameter.h"
#include <QWidget>

namespace Ui {
    class MesgBox;
}

class MesgBox : public QWidget
{
    Q_OBJECT

public:
    explicit MesgBox(QWidget *parent = 0);

    QcomboxTime *label_ctime ;
    QcomboxTime *label_mtime ;
    Qcombox *label_cdeepth ;
    Qcombox *label_mdeepth ;
    Qcheck *check_pump ;
    Qcheck *check_lock ;
    Qcheck *check_reverse ;
    Qcheck *check_alterpower ;
    Qindicate *indicate_voltage ;
    Qindicate *indicate_speed ;
    Qparameter *parameter_jump ;

    ~MesgBox();

private:
    Ui::MesgBox *ui;
};

#endif // MESGBOX_H
