#ifndef CLEARDIALOG_H
#define CLEARDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QPushButton>
#include <QKeyEvent>
#include "setting.h"
#include "fm25v02.h"
#include "sparkinfo.h"

#define PASSWD_LENGTH 6

namespace Ui {
class ClearDialog;
}

class ClearDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ClearDialog(QWidget *parent = 0);
    ~ClearDialog();
    
protected:
    void keyPressEvent(QKeyEvent *);

public slots:
    void valueChange(QString);
    void commitResult(int);

private:
    Ui::ClearDialog *ui;
    QIcon ok_icon;
    QIcon cancel_icon;
};

#endif // CLEARDIALOG_H
