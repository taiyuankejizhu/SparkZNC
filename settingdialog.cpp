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
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    //this->setWindowModality(Qt::ApplicationModal);

    flag = false;

    grp1 = new QButtonGroup(this);
    grp2 = new QButtonGroup(this);
    grp3 = new QButtonGroup(this);
    grp4 = new QButtonGroup(this);

    grp1->addButton(ui->radioButton);
    grp1->addButton(ui->radioButton_2);

    grp2->addButton(ui->radioButton_3);
    grp2->addButton(ui->radioButton_4);

    grp3->addButton(ui->radioButton_5);
    grp3->addButton(ui->radioButton_6);

    grp4->addButton(ui->radioButton_7);
    grp4->addButton(ui->radioButton_8);

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
    flag = false;

    ui->lineEdit_2->setEnabled(b);
    ui->lineEdit_2->clear();
    ui->lineEdit_3->setEnabled(b);
    ui->lineEdit_3->clear();
    if(spark_info->uint_array[UINT_SCALE] == SCALE_5)
        ui->radioButton->setChecked(true);
    else
        ui->radioButton_2->setChecked(true);
    ui->radioButton->setEnabled(b);
    ui->radioButton_2->setEnabled(b);
    if(!spark_info->b_array[B_X_ORIENT])
        ui->radioButton_3->setChecked(true);
    else
        ui->radioButton_4->setChecked(true);
    ui->radioButton_3->setEnabled(b);
    ui->radioButton_4->setEnabled(b);
    if(!spark_info->b_array[B_Y_ORIENT])
        ui->radioButton_5->setChecked(true);
    else
        ui->radioButton_6->setChecked(true);
    ui->radioButton_5->setEnabled(b);
    ui->radioButton_6->setEnabled(b);
    if(!spark_info->b_array[B_Z_ORIENT])
        ui->radioButton_7->setChecked(true);
    else
        ui->radioButton_8->setChecked(true);
    ui->radioButton_7->setEnabled(b);
    ui->radioButton_8->setEnabled(b);

    ui->label_9->clear();
}

void SettingDialog::keyPressEvent(QKeyEvent *k)
{
    k->accept();
    switch(k->key())
    {
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
    /*复位密码*/
    char godwd[PASSWD_LENGTH];
    char tmp = 0;
    int i = 0;

    memset(passwd ,0x00 ,sizeof passwd);
    memset(godwd ,0x00 ,sizeof godwd);
    /*从铁电芯片中读出密码*/
    FM25V02_READ(PASSWD_ADDR , passwd ,sizeof passwd);

    godwd[0] = '8';
    godwd[1] = '8';
    godwd[2] = '8';
    godwd[3] = '8';
    godwd[4] = '8';
    godwd[5] = '8';

    for(i = 0;i < p.length();i++){
        tmp = p.at(i).toAscii();
        if(tmp != passwd[i]){
            break;
        }
    }

    /*检查是否为复位密码*/
    if(i != PASSWD_LENGTH){
        for(i = 0;i < p.length();i++){
            tmp = p.at(i).toAscii();
            if(tmp != godwd[i]){
                break;
            }
        }
    }

    if(i == PASSWD_LENGTH){
        stateUpdate(true);
        ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(false);
    }else{
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

        char tmp = 0;
        if(ui->radioButton->isChecked()){
            tmp = SCALE_5;
            spark_info->uint_array[UINT_SCALE] = tmp;
            FM25V02_WRITE(SCALE_ADDR , &tmp, sizeof tmp);
        }else{
            tmp = SCALE_1;
            spark_info->uint_array[UINT_SCALE] = tmp;
            FM25V02_WRITE(SCALE_ADDR , &tmp, sizeof tmp);
        }

        tmp = 0;
        if(ui->radioButton_3->isChecked()){
            tmp = CFALSE;
            spark_info->b_array[B_X_ORIENT] = false;
            FM25V02_WRITE(X_ORIENT_ADDR , &tmp, sizeof tmp);
        }else{
            tmp = CTRUE;
            spark_info->b_array[B_X_ORIENT] = true;
            FM25V02_WRITE(X_ORIENT_ADDR , &tmp, sizeof tmp);
        }

        tmp = 0;
        if(ui->radioButton_5->isChecked()){
            tmp = CFALSE;
            spark_info->b_array[B_Y_ORIENT] = false;
            FM25V02_WRITE(Y_ORIENT_ADDR , &tmp, sizeof tmp);
        }else{
            tmp = CTRUE;
            spark_info->b_array[B_Y_ORIENT] = true;
            FM25V02_WRITE(Y_ORIENT_ADDR , &tmp, sizeof tmp);
        }

        tmp = 0;
        if(ui->radioButton_7->isChecked()){
            tmp = CFALSE;
            spark_info->b_array[B_Z_ORIENT] = false;
            FM25V02_WRITE(Z_ORIENT_ADDR , &tmp, sizeof tmp);
        }else{
            tmp = CTRUE;
            spark_info->b_array[B_Z_ORIENT] = true;
            FM25V02_WRITE(Z_ORIENT_ADDR , &tmp, sizeof tmp);
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
