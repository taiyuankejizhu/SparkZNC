#ifndef SECONLEVEL_F3_H
#define SECONLEVEL_F3_H

#include "coordialog.h"
#include <QWidget>

namespace Ui {
    class seconlevel_f3;
}

class seconlevel_f3 : public QWidget
{
    Q_OBJECT

public:
    explicit seconlevel_f3(QWidget *parent = 0);
    CoorDialog *coor;
    ~seconlevel_f3();

private:
    Ui::seconlevel_f3 *ui;

signals:
    void enter(int i);
public slots:
    void F3();
    void F8();
    void F0();
    void f3_Done(int);
};

#endif // SECONLEVEL_F3_H
