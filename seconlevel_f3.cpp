#include "seconlevel_f3.h"
#include "ui_seconlevel_f3.h"
#include "sparkinfo.h"
#include "qdebug.h"

seconlevel_f3::seconlevel_f3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::seconlevel_f3)
{
    ui->setupUi(this);

    coor = new CoorDialog(parent);
    coor->hide();
    connect(coor ,SIGNAL(finished(int)) ,this ,SLOT(f3_Done(int)));

    connect(ui->pushButton_F3,SIGNAL(clicked()),this ,SLOT(F3()));
    ui->pushButton_F3->setCheckable(true);

    connect(this ,SIGNAL(enter(int)),parent ,SLOT(funcbarUpdate(int)));
    connect(ui->pushButton_F8,SIGNAL(clicked()),this ,SLOT(F8()));

    setFocusPolicy(Qt::NoFocus);
}

void seconlevel_f3::F3()
{
    if(ui->pushButton_F3->isChecked()){
        coor->show();
        coor->updateContent();
        coor->setFocus();
    }
    else
        coor->hide();
}

void seconlevel_f3::F8()
{
    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();

    emit enter(28);
}

void seconlevel_f3::F0()
{
}

void seconlevel_f3::f3_Done(int r)
{
    if(r == 0){
        coor->selected = spark_info->uint_array[UINT_COOR_INDEX];
        qDebug()<<"F3 Cancel!";
    }
    else{
        spark_info->setUInt(UINT_COOR_INDEX ,coor->selected);
        qDebug()<<"F3 OK!";
    }
    parentWidget()->setFocus();
    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();
}

seconlevel_f3::~seconlevel_f3()
{
    delete ui;
}
