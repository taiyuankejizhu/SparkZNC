#ifndef JUMPDIALOG_H
#define JUMPDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>

namespace Ui {
    class JumpDialog;
}

class JumpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JumpDialog(QWidget *parent = 0);
    unsigned int height;
    unsigned int count;
    bool check;
    ~JumpDialog();

protected:
    void keyPressEvent(QKeyEvent *);

public slots:
    void checkChange(int);
    void valueChange();

private:
    Ui::JumpDialog *ui;
    QIcon a_icon;
    QIcon b_icon;
    QIcon ok_icon;
    QIcon cancel_icon;
};

#endif // JUMPDIALOG_H
