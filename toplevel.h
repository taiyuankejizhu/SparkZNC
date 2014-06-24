#ifndef TOPLEVEL_H
#define TOPLEVEL_H

#include <QWidget>

namespace Ui {
    class toplevel;
}

class toplevel : public QWidget
{
    Q_OBJECT

public:
    explicit toplevel(QWidget *parent = 0);
    ~toplevel();

private:
    Ui::toplevel *ui;

signals:

    void enter(int i);

public slots:
    void F1();
    void F2();
    void F3();
    void F4();
    void F5();
    void F6();
    void F7();
    void F0();
};

#endif // TOPLEVEL_H
