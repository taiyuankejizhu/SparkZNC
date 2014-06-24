#ifndef SECONLEVEL_F4_H
#define SECONLEVEL_F4_H

#include "groupdialog.h"
#include "autodialog.h"
#include "jumpdialog.h"
#include <QWidget>
#include <QMenu>

namespace Ui {
    class seconlevel_f4;
}

class seconlevel_f4 : public QWidget
{
    Q_OBJECT

public:
    explicit seconlevel_f4(QWidget *parent = 0);
    GroupDialog *group;
    AutoDialog *autotab;
    JumpDialog *jump;
    ~seconlevel_f4();

private:
    Ui::seconlevel_f4 *ui;

signals:
    void enter(int i);
    void stateChange(char);
    void selectRows(bool);
public slots:
    void F1();
    void F2();
    void F3();
    void F4();
    void F5();
    void F6();
    void F7();
    void F8();
    void F0();
    void f1_Done(int);
    void f2_Done(int);
    void f6_Done(int);
};

#endif // SECONLEVEL_F4_H
