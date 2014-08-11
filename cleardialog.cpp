#include "cleardialog.h"
#include "ui_cleardialog.h"
#include "qdebug.h"

ClearDialog::ClearDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClearDialog)
{
    ui->setupUi(this);
    this->setGeometry((parent->width()-width())/2 ,390 ,width() ,height());
    this->setWindowFlags(Qt::FramelessWindowHint);

    QPixmap ok_pix = QPixmap(":/ok.png");
    ok_icon = QIcon(ok_pix);
    QPixmap cancel_pix = QPixmap(":/cancel.png");
    cancel_icon = QIcon(cancel_pix);

    QPixmap c_pix = QPixmap(":/clear.png");

    connect(ui->lineEdit ,SIGNAL(textChanged(QString)) ,this ,SLOT(valueChange(QString)));
    connect(this ,SIGNAL(finished(int)) ,this ,SLOT(commitResult(int)));

    ui->label->setPixmap(c_pix);
    ui->buttonBox->button(ui->buttonBox->Ok)->setText(tr("确定(O)"));
    ui->buttonBox->button(ui->buttonBox->Ok)->setIcon(ok_icon);
    ui->buttonBox->button(ui->buttonBox->Ok)->setIconSize(QSize(32,32));
    ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(true);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setText(tr("取消(C)"));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIcon(cancel_icon);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIconSize(QSize(32,32));
}

void ClearDialog::keyPressEvent(QKeyEvent *k)
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

void ClearDialog::valueChange(QString p)
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
        ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(false);
    }
    else{
        ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(true);
    }
}

void ClearDialog::commitResult(int i)
{
    SixBytes current;
    memset(current.bytes , 0 ,sizeof current);

    if(i == 0){
    }
    if(i == 1){
        /*向铁电芯片中写当前加工时间为零*/
        FM25V02_WRITE(CURRENT_TIME_ADDR , current.bytes, sizeof current);
    }
    ui->lineEdit->clear();
    ui->buttonBox->button(ui->buttonBox->Ok)->setDisabled(true);
    return;
}

ClearDialog::~ClearDialog()
{
    delete ui;
}
