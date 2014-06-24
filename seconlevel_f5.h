#ifndef SECONLEVEL_F5_H
#define SECONLEVEL_F5_H

#include <QWidget>

namespace Ui {
    class seconlevel_f5;
}

class seconlevel_f5 : public QWidget
{
    Q_OBJECT

public:
    explicit seconlevel_f5(QWidget *parent = 0);
    ~seconlevel_f5();

private:
    Ui::seconlevel_f5 *ui;
signals:
    void enter(int i);
public slots:
    void F1();
    void F2();
    void F3();
    void F4();
    void F5();
    void F6();
    void F8();
    void F0();
};

#endif // SECONLEVEL_F5_H
