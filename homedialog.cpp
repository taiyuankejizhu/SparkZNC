#include "homedialog.h"
#include "ui_homedialog.h"
#include "qdebug.h"

HomeDialog::HomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeDialog)
{
    ui->setupUi(this);

    this->setGeometry(0 ,390 ,width() ,height());
    this->setWindowFlags(Qt::FramelessWindowHint);

    QPixmap c_pix = QPixmap(":/home.png");

    QPixmap ok_pix = QPixmap(":/ok.png");
    ok_icon = QIcon(ok_pix);
    QPixmap cancel_pix = QPixmap(":/cancel.png");
    cancel_icon = QIcon(cancel_pix);

    ui->pushButton->setStyleSheet(
        "QPushButton{border-image:url(:/down.png); color: black;}"
        "QPushButton:checked{border-image:url(:/down_p.png); color: black;}"
        "QPushButton:pressed{border-image:url(:/down_p.png); color: black;}");
    ui->pushButton->setCheckable(true);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    connect(ui->pushButton ,SIGNAL(clicked()) ,this ,SLOT(Down()));

    ui->pushButton_2->setStyleSheet(
        "QPushButton{border-image:url(:/up.png); color: black;}"
        "QPushButton:checked{border-image:url(:/up_p.png); color: black;}"
        "QPushButton:pressed{border-image:url(:/up_p.png); color: black;}");
    ui->pushButton_2->setCheckable(true);
    ui->pushButton_2->setFocusPolicy(Qt::NoFocus);
    connect(ui->pushButton_2 ,SIGNAL(clicked()) ,this ,SLOT(Up()));

    ui->label->setPixmap(c_pix);
    ui->buttonBox->button(ui->buttonBox->Ok)->setText(tr("确定(O)"));
    ui->buttonBox->button(ui->buttonBox->Ok)->setIcon(ok_icon);
    ui->buttonBox->button(ui->buttonBox->Ok)->setIconSize(QSize(32,32));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setText(tr("取消(C)"));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIcon(cancel_icon);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIconSize(QSize(32,32));
}

void HomeDialog::keyPressEvent(QKeyEvent *k)
{
    QWidget *w = focusWidget();
    QString type(w->metaObject()->className());

    k->accept();
    switch(k->key())
    {
    case Qt::Key_Down:
        ui->pushButton->click();
        break;
    case Qt::Key_Up:
        ui->pushButton_2->click();
        break;
    case Qt::Key_F6:
        finished(1);
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
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
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if(type.contains("View")){
        }else if(type.contains("Edit")){
            focusNextChild();
        }
        break;
    default:
        break;
    }
}

void HomeDialog::Up()
{
    if(ui->pushButton->isChecked())
        ui->pushButton->click();

    if(ui->pushButton_2->isChecked())
        spark_info->setBool(B_HOME_Z_UP ,true);
    else
        spark_info->setBool(B_HOME_Z_UP ,false);
}

void HomeDialog::Down()
{
    if(ui->pushButton_2->isChecked())
        ui->pushButton_2->click();

    if(ui->pushButton->isChecked())
        spark_info->setBool(B_HOME_Z_DOWN ,true);
    else
        spark_info->setBool(B_HOME_Z_DOWN ,false);
}

HomeDialog::~HomeDialog()
{
    delete ui;
}
