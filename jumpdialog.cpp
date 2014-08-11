#include "jumpdialog.h"
#include "ui_jumpdialog.h"
#include "qdebug.h"

JumpDialog::JumpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JumpDialog)
{
    bool ok = false;

    ui->setupUi(this);
    this->setGeometry(0 ,390 ,874 ,300);
    this->setWindowFlags(Qt::FramelessWindowHint);

    QPixmap a_pix = QPixmap(":/selected.png");
    a_icon = QIcon(a_pix);
    QPixmap b_pix = QPixmap(":/unselected.png");
    b_icon = QIcon(b_pix);

    QPixmap c_pix = QPixmap(":/jump.png");

    QPixmap ok_pix = QPixmap(":/ok.png");
    ok_icon = QIcon(ok_pix);
    QPixmap cancel_pix = QPixmap(":/cancel.png");
    cancel_icon = QIcon(cancel_pix);

    connect(ui->checkBox ,SIGNAL(stateChanged(int)) ,this ,SLOT(checkChange(int)));
    connect(ui->lineEdit ,SIGNAL(editingFinished()) ,this ,SLOT(valueChange()));
    connect(ui->lineEdit_2 ,SIGNAL(editingFinished()) ,this ,SLOT(valueChange()));

    ui->lineEdit->setText("0");
    ui->lineEdit_2->setText("5");

    height = ui->lineEdit->text().toUInt(&ok ,10);
    count = ui->lineEdit_2->text().toUInt(&ok ,10);

    if(ui->checkBox->isChecked()){
        check = true;
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
    }
    else{
        check = false;
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
    }

    ui->label->setPixmap(c_pix);
    ui->buttonBox->button(ui->buttonBox->Ok)->setText(tr("确定(O)"));
    ui->buttonBox->button(ui->buttonBox->Ok)->setIcon(ok_icon);
    ui->buttonBox->button(ui->buttonBox->Ok)->setIconSize(QSize(32,32));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setText(tr("取消(C)"));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIcon(cancel_icon);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIconSize(QSize(32,32));
}

void JumpDialog::keyPressEvent(QKeyEvent *k)
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
    case Qt::Key_F6:
        finished(1);
        break;
    case Qt::Key_F1:
    case Qt::Key_F2:
    case Qt::Key_F3:
    case Qt::Key_F4:
    case Qt::Key_F5:
    case Qt::Key_F7:
    case Qt::Key_F8:
        finished(0);
        break;
    default:
        break;
    }
}

void JumpDialog::checkChange(int s)
{
    qDebug()<<s;

    if(ui->checkBox->isChecked()){
        check = true;
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
    }
    else{
        check = false;
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
    }
}

void JumpDialog::valueChange()
{
    bool ok = false;
    height = ui->lineEdit->text().toUInt(&ok ,10);
    if(height > 99 && ok){
        height = 99;
        ui->lineEdit->setText("99");
    }
    count = ui->lineEdit_2->text().toUInt(&ok ,10);
    if(count > 9 && ok){
        count = 9;
        ui->lineEdit_2->setText("9");
    }
}

JumpDialog::~JumpDialog()
{
    delete ui;
}
