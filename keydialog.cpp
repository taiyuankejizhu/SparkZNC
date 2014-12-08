#include "keydialog.h"
#include "ui_keydialog.h"
#include "sparkinfo.h"
#include "qdebug.h"

KeyDialog::KeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyDialog)
{
    ui->setupUi(this);

    this->setGeometry((parent->width()-width())/2 ,390 ,width() ,height());
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    //this->setWindowModality(Qt::ApplicationModal);

    QPixmap ok_pix = QPixmap(":/ok.png");
    ok_icon = QIcon(ok_pix);
    QPixmap cancel_pix = QPixmap(":/cancel.png");
    cancel_icon = QIcon(cancel_pix);

    QPixmap c_pix = QPixmap(":/password.png");

    connect(ui->lineEdit ,SIGNAL(textChanged(QString)) ,this ,SLOT(valueChange(QString)));
    connect(ui->lineEdit_1 ,SIGNAL(textChanged(QString)) ,this ,SLOT(keyChange(QString)));
    connect(this ,SIGNAL(finished(int)) ,this ,SLOT(commitResult(int)));

    ui->label_2->setText("XXXXXXXXXXX");
    ui->lineEdit_1->setDisabled(true);

    ui->label->setPixmap(c_pix);
    ui->buttonBox->button(ui->buttonBox->Ok)->setText(tr("确定(O)"));
    ui->buttonBox->button(ui->buttonBox->Ok)->setIcon(ok_icon);
    ui->buttonBox->button(ui->buttonBox->Ok)->setIconSize(QSize(32,32));
    ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(true);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setText(tr("取消(C)"));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIcon(cancel_icon);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIconSize(QSize(32,32));
}

void KeyDialog::keyPressEvent(QKeyEvent *k)
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

void KeyDialog::valueChange(QString p)
{
    if(p.length() != PASSWD_LENGTH)
        return;

    char passwd[PASSWD_LENGTH];
    char id[ID_LENGTH];
    char tmp = 0;
    int i = 0;

    memset(passwd ,0x00 ,sizeof passwd);
    memset(id ,0x00 ,sizeof id);

    /*从铁电芯片中读出密码*/
    FM25V02_READ(PASSWD_ADDR , passwd ,sizeof passwd);

    for(i = 0;i < p.length();i++){
        tmp = p.at(i).toAscii();
        if(tmp != passwd[i])
            break;
    }

    if(i == PASSWD_LENGTH){
        /*从铁电芯片中读出密码*/
        FM25V02_READ(ID_ADDR , id ,sizeof id);
        QString str;
        QString tmp;
        str.append(tr("ID："));
        str.append(id);
        str.append(tr("当前剩余加工时间："));
        tmp = QString::number(spark_info->target.ushorts[0] - spark_info->useup.ushorts[0] ,10);
        str.append(tmp);
        str.append(tr("小时 "));
        tmp = QString::number(spark_info->target.ushorts[1] - spark_info->useup.ushorts[1] ,10);
        str.append(tmp);
        str.append(tr("分"));
        ui->label_2->setText(str);
        ui->lineEdit_1->setDisabled(false);
    }else{
        ui->label_2->setText(tr("密码错误！"));
    }
}

void KeyDialog::keyChange(QString k)
{

    if(k.length() != KEY_LENGTH)
        return;
    char key[KEY_LENGTH];
    char tmp;
    int i = 0;

    memset(key ,'1' ,sizeof key);

    for(i = 0;i < k.length();i++){
        tmp = k.at(i).toAscii();
        if(tmp != key[i])
            break;
    }

    if(i == KEY_LENGTH){
        ui->label_3->setText(tr("该序列号的有效时间为：200小时"));
        ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(false);
    }else{
        ui->label_3->setText(tr("序列号无效！"));
        ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(true);
    }
}

void KeyDialog::commitResult(int i)
{
    if(i == 0){
    }
    if(i == 1){
        spark_info->target.ushorts[0] = 0xff;
        spark_info->target.ushorts[1] = 0xff;
        spark_info->target.ushorts[2] = 0x00;

        spark_info->useup.ushorts[0] = 0x00;
        spark_info->useup.ushorts[1] = 0x00;
        spark_info->useup.ushorts[2] = 0x00;

        /*向铁电芯片中写放电溢出时间*/
        FM25V02_WRITE(TARGET_TIME_ADDR , spark_info->target.bytes ,sizeof spark_info->target);
        /*清空铁电芯片中的已经放电时间*/
        FM25V02_WRITE(USEDP_TIME_ADDR , spark_info->useup.bytes ,sizeof spark_info->useup);
    }
    ui->label_3->clear();
    ui->lineEdit_1->clear();
    ui->lineEdit_1->setDisabled(true);
    ui->label_2->setText("XXXXXXXXXXX");
    ui->lineEdit->clear();
    ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(true);
    return;
}

KeyDialog::~KeyDialog()
{
    delete ui;
}
