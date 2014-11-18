#include "toplevel.h"
#include "sparkinfo.h"
#include "ui_toplevel.h"
#include "qdebug.h"

toplevel::toplevel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::toplevel)
{
    ui->setupUi(this);

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

    connect(ui->pushButton_F8,SIGNAL(clicked()),this ,SLOT(F8()));
    ui->pushButton_F8->setCheckable(true);

    /*根据命令行参数字符串，切换开始按钮的图片*/
    QString tmp(QApplication::argv()[1]);
    if(!tmp.compare("en")){
/*
        ui->pushButton_F8->setStyleSheet(
            "QPushButton{border-image:url(:/icon_e.png); color: black;}"
            "QPushButton:checked{border-image:url(:/icon_p_e.png); color: black;}"
            "QPushButton:hover{border-image:url(:/icon_h_e.png); color: black;}"
            "QPushButton:pressed{border-image:url(:/icon_p_e.png); color: black;}");
*/
        ui->pushButton_F8->setStyleSheet(
            "QPushButton{border-image:url(:/icon_e.png); color: black;}"
            "QPushButton:checked{border-image:url(:/icon_p_e.png); color: black;}"
            "QPushButton:pressed{border-image:url(:/icon_p_e.png); color: black;}");
    }else{
 /*
        ui->pushButton_F8->setStyleSheet(
            "QPushButton{border-image:url(:/icon.png); color: black;}"
            "QPushButton:checked{border-image:url(:/icon_p.png); color: black;}"
            "QPushButton:hover{border-image:url(:/icon_h.png); color: black;}"
            "QPushButton:pressed{border-image:url(:/icon_p.png); color: black;}");
 */
        ui->pushButton_F8->setStyleSheet(
            "QPushButton{border-image:url(:/icon.png); color: black;}"
            "QPushButton:checked{border-image:url(:/icon_p.png); color: black;}"
            "QPushButton:pressed{border-image:url(:/icon_p.png); color: black;}");
    }

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

void toplevel::F8()
{
    if(ui->pushButton_F8->isChecked()){
        emit menu(false);
    }else{
        emit menu(false);
    }
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
