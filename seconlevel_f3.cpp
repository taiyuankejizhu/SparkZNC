#include "seconlevel_f3.h"
#include "ui_seconlevel_f3.h"
#include "sparkinfo.h"
#include "qdebug.h"

seconlevel_f3::seconlevel_f3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::seconlevel_f3)
{
    ui->setupUi(this);

    connect(ui->pushButton_F1 ,SIGNAL(clicked()) ,this ,SLOT(F1()));
    ui->pushButton_F1->setCheckable(true);

    coor = new CoorDialog(parent);
    coor->hide();
    connect(coor ,SIGNAL(finished(int)) ,this ,SLOT(f3_Done(int)));

    connect(ui->pushButton_F3,SIGNAL(clicked()),this ,SLOT(F3()));
    ui->pushButton_F3->setCheckable(true);

    home = new HomeDialog(parent);
    home->hide();
    connect(home ,SIGNAL(finished(int)) ,this ,SLOT(f6_Done(int)));

    connect(ui->pushButton_F6 ,SIGNAL(clicked()) ,this ,SLOT(F6()));
    ui->pushButton_F6->setCheckable(true);

    connect(this ,SIGNAL(enter(int)),parent ,SLOT(funcbarUpdate(int)));
    connect(ui->pushButton_F8,SIGNAL(clicked()),this ,SLOT(F8()));

    setFocusPolicy(Qt::NoFocus);
}

void seconlevel_f3::F1()
{
    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();

    if(ui->pushButton_F6->isChecked())
        ui->pushButton_F6->click();

    spark_info->reverseBool(B_ZERO);
}

void seconlevel_f3::F3()
{
    if(ui->pushButton_F1->isChecked())
        ui->pushButton_F1->click();

    if(ui->pushButton_F6->isChecked())
        ui->pushButton_F6->click();

    if(ui->pushButton_F3->isChecked()){
        coor->show();
        coor->updateContent();
        coor->setFocus();
    }
    else
        coor->hide();
}

void seconlevel_f3::F6()
{
    if(ui->pushButton_F1->isChecked())
        ui->pushButton_F1->click();

    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();

    if(ui->pushButton_F6->isChecked()){
        home->show();
        home->setFocus();
    }
    else
        home->hide();
}

void seconlevel_f3::F8()
{
    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();

    if(ui->pushButton_F6->isChecked())
        ui->pushButton_F6->click();

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
        spark_info->setLong(L_X_CURRENT ,coor->x);
        spark_info->setLong(L_Y_CURRENT ,coor->y);
        spark_info->setLong(L_Z_CURRENT ,coor->z);
        qDebug()<<"F3 OK!";
    }
    parentWidget()->setFocus();
    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();
}

void seconlevel_f3::f6_Done(int r)
{
    if(r == 0){
        qDebug()<<"F6 Cancel!";
    }
    else{
        qDebug()<<"F6 OK!";
    }
    parentWidget()->setFocus();
    if(ui->pushButton_F6->isChecked())
        ui->pushButton_F6->click();
}

seconlevel_f3::~seconlevel_f3()
{
    delete ui;
}
