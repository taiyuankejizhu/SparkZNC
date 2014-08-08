#ifndef KEYDIALOG_H
#define KEYDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QPushButton>
#include <QKeyEvent>
#include "setting.h"
#include "fm25v02.h"

#define PASSWD_LENGTH 6
#define ID_LENGTH 11
#define KEY_LENGTH 11

namespace Ui {
class KeyDialog;
}

class KeyDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit KeyDialog(QWidget *parent = 0);
    ~KeyDialog();

protected:
    void keyPressEvent(QKeyEvent *);

public slots:
    void valueChange(QString);
    void keyChange(QString);
    void commitResult(int);

private:
    Ui::KeyDialog *ui;
    QIcon ok_icon;
    QIcon cancel_icon;
};

#endif // KEYDIALOG_H
