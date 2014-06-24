#include "seconlevel_f5.h"
#include "sparkinfo.h"
#include "ui_seconlevel_f5.h"
#include "qdebug.h"

seconlevel_f5::seconlevel_f5(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::seconlevel_f5)
{
    ui->setupUi(this);

    connect(ui->pushButton_F1,SIGNAL(clicked()),this ,SLOT(F1()));
    ui->pushButton_F1->setCheckable(true);

    connect(ui->pushButton_F2,SIGNAL(clicked()),this ,SLOT(F2()));
    ui->pushButton_F2->setCheckable(true);

    connect(ui->pushButton_F3,SIGNAL(clicked()),this ,SLOT(F3()));
    ui->pushButton_F3->setCheckable(true);

    connect(ui->pushButton_F4,SIGNAL(clicked()),this ,SLOT(F4()));
    ui->pushButton_F4->setCheckable(true);

    connect(ui->pushButton_F5,SIGNAL(clicked()),this ,SLOT(F5()));
    ui->pushButton_F5->setCheckable(true);

    connect(ui->pushButton_F6,SIGNAL(clicked()),this ,SLOT(F6()));
    ui->pushButton_F6->setCheckable(true);

    connect(this ,SIGNAL(enter(int)),parent ,SLOT(funcbarUpdate(int)));
    connect(ui->pushButton_F8,SIGNAL(clicked()),this ,SLOT(F8()));

    setFocusPolicy(Qt::NoFocus);
}

void seconlevel_f5::F1()
{

}

void seconlevel_f5::F2()
{
    spark_info->reverseBool(B_FIRE);
}

void seconlevel_f5::F3()
{
    spark_info->reverseBool(B_OIL);
}

void seconlevel_f5::F4()
{
    spark_info->reverseBool(B_DEEPHOLE);
}

void seconlevel_f5::F5()
{
    spark_info->reverseBool(B_XYLOCK);
}

void seconlevel_f5::F6()
{
    spark_info->reverseBool(B_LOCK);
}

void seconlevel_f5::F8()
{
    emit enter(28);
}

void seconlevel_f5::F0()
{

    if(spark_info->b_array[B_FIRE])
        ui->pushButton_F2->setChecked(true);
    else
        ui->pushButton_F2->setChecked(false);

    if(spark_info->b_array[B_OIL])
        ui->pushButton_F3->setChecked(true);
    else
        ui->pushButton_F3->setChecked(false);

    if(spark_info->b_array[B_DEEPHOLE])
        ui->pushButton_F4->setChecked(true);
    else
        ui->pushButton_F4->setChecked(false);

    if(spark_info->b_array[B_XYLOCK])
        ui->pushButton_F5->setChecked(true);
    else
        ui->pushButton_F5->setChecked(false);

    if(spark_info->b_array[B_LOCK])
        ui->pushButton_F6->setChecked(true);
    else
        ui->pushButton_F6->setChecked(false);

}

seconlevel_f5::~seconlevel_f5()
{
    delete ui;
}
