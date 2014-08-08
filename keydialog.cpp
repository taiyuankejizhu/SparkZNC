#include "keydialog.h"
#include "ui_keydialog.h"
#include "qdebug.h"

KeyDialog::KeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyDialog)
{
    ui->setupUi(this);

    this->setGeometry((parent->width()-width())/2 ,390 ,width() ,height());
    this->setWindowFlags(Qt::FramelessWindowHint);

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
        qDebug()<<"key!";
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
        /*从铁电芯片中读出密码*/
        FM25V02_READ(ID_ADDR , id ,sizeof id);
        QString ids;
        ids.append(id);
        ui->label_2->setText(ids);
        ui->lineEdit_1->setDisabled(false);
    }
    else{
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

    memset(key ,0x00 ,sizeof key);

    for(i = 0;i < k.length();i++){
        tmp = k.at(i).toAscii();
        if(tmp != key[i])
            break;
    }

    if(i == KEY_LENGTH){
        ui->label_3->setText(tr("该序列号的有效时间为：")+"200H");
        ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(false);
    }
    else{
        ui->label_3->setText(tr("序列号无效！"));
        ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(true);
    }
}

void KeyDialog::commitResult(int i)
{
    if(i == 0){
    }
    if(i == 1){
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
