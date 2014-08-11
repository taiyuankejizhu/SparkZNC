#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "qdebug.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    this->setGeometry((parent->width()-width())/2 ,390 ,width() ,height());
    this->setWindowFlags(Qt::FramelessWindowHint);

    flag = false;

    QPixmap ok_pix = QPixmap(":/ok.png");
    ok_icon = QIcon(ok_pix);
    QPixmap cancel_pix = QPixmap(":/cancel.png");
    cancel_icon = QIcon(cancel_pix);

    QPixmap c_pix = QPixmap(":/setting.png");

    connect(ui->lineEdit ,SIGNAL(textChanged(QString)) ,this ,SLOT(valueChange(QString)));
    connect(ui->lineEdit_3 ,SIGNAL(textChanged(QString)) ,this ,SLOT(newPasswd(QString)));
    connect(ui->lineEdit_2 ,SIGNAL(textChanged(QString)) ,this ,SLOT(newPasswd(QString)));
    connect(this ,SIGNAL(finished(int)) ,this ,SLOT(commitResult(int)));

    stateUpdate(false);

    ui->label->setPixmap(c_pix);
    ui->buttonBox->button(ui->buttonBox->Ok)->setText(tr("确定(O)"));
    ui->buttonBox->button(ui->buttonBox->Ok)->setIcon(ok_icon);
    ui->buttonBox->button(ui->buttonBox->Ok)->setIconSize(QSize(32,32));
    ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(true);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setText(tr("取消(C)"));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIcon(cancel_icon);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIconSize(QSize(32,32));

}

void SettingDialog::stateUpdate(bool b)
{
    ui->lineEdit_2->setEnabled(b);
    ui->lineEdit_2->clear();
    ui->lineEdit_3->setEnabled(b);
    ui->lineEdit_3->clear();
    ui->checkBox->setEnabled(b);
    ui->checkBox_2->setEnabled(b);
    ui->checkBox_3->setEnabled(b);
    ui->checkBox_4->setEnabled(b);

    ui->label_9->clear();
}

void SettingDialog::keyPressEvent(QKeyEvent *k)
{
    k->accept();
    switch(k->key())
    {
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;
    case Qt::Key_Down:
        focusNextChild();
        break;
    case Qt::Key_Up:
        focusPreviousChild();
        break;
    case Qt::Key_F1:
    case Qt::Key_F2:
    case Qt::Key_F3:
    case Qt::Key_F4:
    case Qt::Key_F5:
    case Qt::Key_F6:
    case Qt::Key_F7:
    case Qt::Key_F8:
        finished(0);
        break;
    default:
        break;
    }
}

void SettingDialog::valueChange(QString p)
{
    if(p.length() != PASSWD_LENGTH)
        return;

    char passwd[PASSWD_LENGTH];
    char tmp = 0;
    int i = 0;

    memset(passwd ,0x00 ,sizeof passwd);
    /*从铁电芯片中读出密码*/
    FM25V02_READ(PASSWD_ADDR , passwd ,sizeof passwd);

    passwd[0] = '1';
    passwd[1] = '2';
    passwd[2] = '3';
    passwd[3] = '4';
    passwd[4] = '5';
    passwd[5] = '6';

    for(i = 0;i < p.length();i++){
        tmp = p.at(i).toAscii();
        if(tmp != passwd[i])
            break;
    }

    if(i == PASSWD_LENGTH){
        stateUpdate(true);
        ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(false);
    }
    else{
        stateUpdate(false);
        ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(true);
    }
}

void SettingDialog::newPasswd(QString n)
{
    QString str1 = ui->lineEdit_2->text();
    QString str2 = ui->lineEdit_3->text();
    n = "";

    if(str1.length() != PASSWD_LENGTH||str2.length() != PASSWD_LENGTH)
        return;
    if(str1 == str2){
        ui->label_9->setText(tr("两次输入一致！"));
        flag = true;
    }
    else{
        ui->label_9->setText(tr("两次输入不一致！"));
        flag = false;
    }
}

void SettingDialog::commitResult(int i)
{
    char passwd[PASSWD_LENGTH];
    char tmp = 0;
    memset(passwd ,0x00 ,sizeof passwd);
    int k = 0;

    for(k = 0;k < ui->lineEdit_2->text().length();k++){
        tmp = ui->lineEdit_2->text().at(k).toAscii();
        passwd[k] = tmp;
    }

    if(i == 0){
    }
    if(i == 1){
        if(flag){
            /*更改系统密码*/
            FM25V02_WRITE(PASSWD_ADDR , passwd, sizeof passwd);
        }
    }
    stateUpdate(false);
    flag = false;
    ui->lineEdit->clear();
    ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(true);
    return;
}

SettingDialog::~SettingDialog()
{
    delete ui;
}
