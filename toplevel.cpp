#include "toplevel.h"
#include "sparkinfo.h"
#include "ui_toplevel.h"
#include "qdebug.h"

toplevel::toplevel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::toplevel)
{
    ui->setupUi(this);

    QPixmap pix = QPixmap(":/icon.png");
    ui->label->setPixmap(pix);

    connect(ui->pushButton_F1,SIGNAL(clicked()),this ,SLOT(F1()));
    ui->pushButton_F1->setCheckable(true);

    connect(ui->pushButton_F2,SIGNAL(clicked()),this ,SLOT(F2()));
    ui->pushButton_F2->setCheckable(true);

    connect(this ,SIGNAL(enter(int)),parent ,SLOT(funcbarUpdate(int)));

    connect(ui->pushButton_F3,SIGNAL(clicked()),this ,SLOT(F3()));
    connect(ui->pushButton_F4,SIGNAL(clicked()),this ,SLOT(F4()));
    connect(ui->pushButton_F5,SIGNAL(clicked()),this ,SLOT(F5()));
    connect(ui->pushButton_F6,SIGNAL(clicked()),this ,SLOT(F6()));

    connect(ui->pushButton_F7,SIGNAL(clicked()),this ,SLOT(F7()));
    ui->pushButton_F7->setCheckable(true);

    setFocusPolicy(Qt::NoFocus);
}

void toplevel::F1()
{
    spark_info->reverseBool(B_PUMP);
}

void toplevel::F2()
{
    spark_info->reverseBool(B_POWER);
}

void toplevel::F3()
{
    emit enter(23);
}

void toplevel::F4()
{
    emit enter(24);
}

void toplevel::F5()
{
    emit enter(25);
}

void toplevel::F6()
{
    emit enter(26);
}

void toplevel::F7()
{
    spark_info->reverseBool(B_REVERSE);
}

void toplevel::F0()
{
    if(spark_info->b_array[B_PUMP])
        ui->pushButton_F1->setChecked(true);
    else
        ui->pushButton_F1->setChecked(false);

    if(spark_info->b_array[B_POWER])
        ui->pushButton_F2->setChecked(true);
    else
        ui->pushButton_F2->setChecked(false);

    if(spark_info->b_array[B_REVERSE])
        ui->pushButton_F7->setChecked(true);
    else
        ui->pushButton_F7->setChecked(false);
}

toplevel::~toplevel()
{
    delete ui;
}
