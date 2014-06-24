#include "seconlevel_f6.h"
#include "ui_seconlevel_f6.h"
#include "sparkinfo.h"
#include "qdebug.h"

seconlevel_f6::seconlevel_f6(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::seconlevel_f6)
{
    ui->setupUi(this);

    /*关机和休眠开关打开后，在扫描进程中计时一段时间后执行相应命令*/

    connect(ui->pushButton_F1,SIGNAL(clicked()),this ,SLOT(F1()));
    ui->pushButton_F1->setCheckable(true);

    connect(ui->pushButton_F2,SIGNAL(clicked()),this ,SLOT(F2()));
    ui->pushButton_F2->setCheckable(true);

    connect(ui->pushButton_F4,SIGNAL(clicked()),this ,SLOT(F4()));
    ui->pushButton_F4->setCheckable(true);

    connect(this ,SIGNAL(enter(int)),parent ,SLOT(funcbarUpdate(int)));
    connect(ui->pushButton_F8,SIGNAL(clicked()),this ,SLOT(F8()));

    setFocusPolicy(Qt::NoFocus);
}

void seconlevel_f6::F1()
{
    spark_info->reverseBool(B_SOUND);
}

void seconlevel_f6::F2()
{
    spark_info->reverseBool(B_SLEEP);
}

void seconlevel_f6::F3()
{

}

void seconlevel_f6::F4()
{
    spark_info->reverseBool(B_SHUTDOWN);
}

void seconlevel_f6::F8()
{
    emit enter(28);
}

void seconlevel_f6::F0()
{

    if(spark_info->b_array[B_SOUND])
        ui->pushButton_F1->setChecked(true);
    else
        ui->pushButton_F1->setChecked(false);

    if(spark_info->b_array[B_SLEEP])
        ui->pushButton_F2->setChecked(true);
    else
        ui->pushButton_F2->setChecked(false);

    if(spark_info->b_array[B_SHUTDOWN])
        ui->pushButton_F4->setChecked(true);
    else
        ui->pushButton_F4->setChecked(false);

}

seconlevel_f6::~seconlevel_f6()
{
    delete ui;
}
