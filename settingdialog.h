#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QPushButton>
#include <QKeyEvent>
#include <QButtonGroup>
#include "setting.h"
#include "fm25v02.h"
#include "sparkinfo.h"

#define PASSWD_LENGTH 6

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingDialog(QWidget *parent = 0);
    void stateUpdate(bool);
    bool flag;
    ~SettingDialog();

protected:
    void keyPressEvent(QKeyEvent *);

public slots:
    void valueChange(QString);
    void newPasswd(QString);
    void commitResult(int);

private:
    Ui::SettingDialog *ui;
    QIcon ok_icon;
    QIcon cancel_icon;
    QButtonGroup *grp1;
    QButtonGroup *grp2;
    QButtonGroup *grp3;
    QButtonGroup *grp4;
};

#endif // SETTINGDIALOG_H
