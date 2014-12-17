#include "autodialog.h"
#include "ui_autodialog.h"
#include "qdebug.h"

AutoDialog::AutoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AutoDialog)
{
    ui->setupUi(this);
    this->setGeometry(0 ,390 ,width() ,height());
    this->setWindowFlags(Qt::FramelessWindowHint);

    flag = 0x00;
    deep = 0x00;
    current = 0x00;
    area = 0x00;
    effect = 0x00;

    QPixmap a_pix = QPixmap(":/selected.png");
    a_icon = QIcon(a_pix);
    QPixmap b_pix = QPixmap(":/unselected.png");
    b_icon = QIcon(b_pix);

    QPixmap c_pix = QPixmap(":/auto.png");

    QPixmap ok_pix = QPixmap(":/ok.png");
    ok_icon = QIcon(ok_pix);
    QPixmap cancel_pix = QPixmap(":/cancel.png");
    cancel_icon = QIcon(cancel_pix);

    connect(ui->lineEdit ,SIGNAL(textChanged(QString)) ,this ,SLOT(valueChange(QString)));
    connect(ui->lineEdit_2 ,SIGNAL(textChanged(QString)) ,this ,SLOT(valueChange(QString)));
    connect(ui->lineEdit_3 ,SIGNAL(textChanged(QString)) ,this ,SLOT(valueChange(QString)));
    connect(ui->lineEdit_4 ,SIGNAL(textChanged(QString)) ,this ,SLOT(valueChange(QString)));

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();

    ui->label->setPixmap(c_pix);
    ui->buttonBox->button(ui->buttonBox->Ok)->setText(tr("确定(O)"));
    ui->buttonBox->button(ui->buttonBox->Ok)->setIcon(ok_icon);
    ui->buttonBox->button(ui->buttonBox->Ok)->setIconSize(QSize(32,32));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setText(tr("取消(C)"));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIcon(cancel_icon);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIconSize(QSize(32,32));
}

void AutoDialog::keyPressEvent(QKeyEvent *k)
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
    case Qt::Key_F2:
        finished(1);
        break;
    case Qt::Key_F1:
    case Qt::Key_F3:
    case Qt::Key_F4:
    case Qt::Key_F5:
    case Qt::Key_F6:
    case Qt::Key_F7:
    case Qt::Key_F8:
        finished(0);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        focusNextChild();
        break;
    default:
        break;
    }
}

void AutoDialog::valueChange(QString s)
{
    bool ok = false;
    s = "";
    if(!ui->lineEdit->text().isEmpty()){
        double t = ui->lineEdit->text().toDouble(&ok);
        deep = t * 1000;
        if(t > 9999.999 && ok){
            t = 9999.999;
            ui->lineEdit->setText("9999.999");
        }
        if(t < -9999.999 && ok){
            t = -9999.999;
            ui->lineEdit->setText("-9999.999");
        }
        flag |= 0x01;
    }
    else{
        flag &= 0xfe;
    }
    if(!ui->lineEdit_2->text().isEmpty()){
        current = ui->lineEdit_2->text().toUInt(&ok ,10);
        if(current > 75 && ok){
            current = 75;
            ui->lineEdit_2->setText("75");
        }
        if(current < 1){
            current = 1;
            ui->lineEdit_2->setText("1");
        }
        flag |= 0x02;
    }
    else{
        flag &= 0xfd;
    }
    if(!ui->lineEdit_3->text().isEmpty()){
        area = ui->lineEdit_3->text().toUInt(&ok ,10);
        if(area > 60 && ok){
            area = 60;
            ui->lineEdit_3->setText("60");
        }
        if(area < 1){
            area = 1;
            ui->lineEdit_3->setText("1");
        }
        flag |= 0x04;
    }
    else{
        flag &= 0xfb;
    }
    if(!ui->lineEdit_4->text().isEmpty()){
        effect= ui->lineEdit_4->text().toUInt(&ok ,10);
        if(effect > 10 && ok){
            effect = 10;
            ui->lineEdit_4->setText("10");
        }
        if(effect < 1){
            effect = 1;
            ui->lineEdit_4->setText("1");
        }
        flag |= 0x08;
    }
    else{
        flag &= 0xf7;
    }
}

AutoDialog::~AutoDialog()
{
    delete ui;
}
