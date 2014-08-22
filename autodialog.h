#ifndef AUTODIALOG_H
#define AUTODIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>

namespace Ui {
    class AutoDialog;
}

class AutoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoDialog(QWidget *parent = 0);
    long long deep;
    unsigned int current;
    unsigned int area;
    unsigned int effect;
    char flag;
    ~AutoDialog();

protected:
    void keyPressEvent(QKeyEvent *);

public slots:
    void valueChange(QString);

private:
    Ui::AutoDialog *ui;
    QIcon a_icon;
    QIcon b_icon;
    QIcon ok_icon;
    QIcon cancel_icon;
};

#endif // AUTODIALOG_H
