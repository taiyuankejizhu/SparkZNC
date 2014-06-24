#ifndef SECONLEVEL_F6_H
#define SECONLEVEL_F6_H

#include <QWidget>

namespace Ui {
    class seconlevel_f6;
}

class seconlevel_f6 : public QWidget
{
    Q_OBJECT

public:
    explicit seconlevel_f6(QWidget *parent = 0);
    ~seconlevel_f6();

private:
    Ui::seconlevel_f6 *ui;
signals:
    void enter(int i);
public slots:
    void F1();
    void F2();
    void F3();
    void F4();
    void F8();
    void F0();
};

#endif // SECONLEVEL_F6_H
