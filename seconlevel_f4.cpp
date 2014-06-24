#include "maininterface.h"
#include "seconlevel_f4.h"
#include "ui_seconlevel_f4.h"
#include "sparkinfo.h"
#include "qdebug.h"

seconlevel_f4::seconlevel_f4(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::seconlevel_f4)
{
    ui->setupUi(this);

    group = new GroupDialog(parent);
    group->hide();
    connect(group ,SIGNAL(finished(int)) ,this ,SLOT(f1_Done(int)));

    autotab = new AutoDialog(parent);
    autotab->hide();
    connect(autotab ,SIGNAL(finished(int)) ,this ,SLOT(f2_Done(int)));

    jump = new JumpDialog(parent);
    jump->hide();
    connect(jump ,SIGNAL(finished(int)) ,this ,SLOT(f6_Done(int)));

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

    connect(ui->pushButton_F7,SIGNAL(clicked()),this ,SLOT(F7()));
    ui->pushButton_F7->setCheckable(true);

    connect(this ,SIGNAL(enter(int)),parent ,SLOT(funcbarUpdate(int)));
    connect(ui->pushButton_F8,SIGNAL(clicked()),this ,SLOT(F8()));

    /*数据表模式改变信号*/
    connect(this ,SIGNAL(stateChange(char)) ,parent ,SLOT(tableStateUpdate(char)));
    /*段选信号*/
    connect(this ,SIGNAL(selectRows(bool)) ,parent ,SLOT(tableSelect(bool)));

    setFocusPolicy(Qt::NoFocus);
}

void seconlevel_f4::F1()
{
    if(ui->pushButton_F1->isChecked()){
        group->show();
        group->setFocus();
    }
    else{
        group->hide();
    }

    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();

    if(ui->pushButton_F2->isChecked())
        ui->pushButton_F2->click();

    if(ui->pushButton_F5->isChecked())
        ui->pushButton_F5->click();

    if(ui->pushButton_F4->isChecked())
        ui->pushButton_F4->click();

    if(ui->pushButton_F6->isChecked())
        ui->pushButton_F6->click();
}

void seconlevel_f4::F2()
{

    if(ui->pushButton_F2->isChecked()){
        autotab->show();
        autotab->setFocus();
    }
    else{
        autotab->hide();
    }

    if(ui->pushButton_F1->isChecked())
        ui->pushButton_F1->click();

    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();

    if(ui->pushButton_F5->isChecked())
        ui->pushButton_F5->click();

    if(ui->pushButton_F4->isChecked())
        ui->pushButton_F4->click();

    if(ui->pushButton_F6->isChecked())
        ui->pushButton_F6->click();
}

void seconlevel_f4::F3()
{
    if(ui->pushButton_F3->isChecked()){
        emit stateChange(TABLE_SELECT);
        emit selectRows(true);
    }
    else{
        emit stateChange(TABLE_EDIT);
    }

    if(ui->pushButton_F1->isChecked())
        ui->pushButton_F1->click();

    if(ui->pushButton_F2->isChecked())
        ui->pushButton_F2->click();

    if(ui->pushButton_F5->isChecked())
        ui->pushButton_F5->click();

    if(ui->pushButton_F4->isChecked())
        ui->pushButton_F4->click();

    if(ui->pushButton_F6->isChecked())
        ui->pushButton_F6->click();
}

void seconlevel_f4::F4()
{
    if(ui->pushButton_F4->isChecked())
        emit stateChange(TABLE_DELETE);
    else
        emit stateChange(TABLE_EDIT);

    if(ui->pushButton_F1->isChecked())
        ui->pushButton_F1->click();

    if(ui->pushButton_F2->isChecked())
        ui->pushButton_F2->click();

    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();

    if(ui->pushButton_F5->isChecked())
        ui->pushButton_F5->click();

    if(ui->pushButton_F6->isChecked())
        ui->pushButton_F6->click();
}

void seconlevel_f4::F5()
{
    if(ui->pushButton_F5->isChecked())
        emit stateChange(TABLE_ADD);
    else
        emit stateChange(TABLE_EDIT);

    if(ui->pushButton_F1->isChecked())
        ui->pushButton_F1->click();

    if(ui->pushButton_F2->isChecked())
        ui->pushButton_F2->click();

    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();

    if(ui->pushButton_F4->isChecked())
        ui->pushButton_F4->click();

    if(ui->pushButton_F6->isChecked())
        ui->pushButton_F6->click();
}

void seconlevel_f4::F6()
{
    if(ui->pushButton_F6->isChecked()){
        jump->show();
        jump->setFocus();
    }
    else{
        jump->hide();
    }

    if(ui->pushButton_F1->isChecked())
        ui->pushButton_F1->click();

    if(ui->pushButton_F2->isChecked())
        ui->pushButton_F2->click();

    if(ui->pushButton_F5->isChecked())
        ui->pushButton_F5->click();

    if(ui->pushButton_F4->isChecked())
        ui->pushButton_F4->click();

    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();
}

void seconlevel_f4::F7()
{
    spark_info->reverseBool(B_BOTTOM);
}

void seconlevel_f4::F8()
{
    if(ui->pushButton_F1->isChecked())
        ui->pushButton_F1->click();

    if(ui->pushButton_F2->isChecked())
        ui->pushButton_F2->click();

    if(ui->pushButton_F3->isChecked())
        ui->pushButton_F3->click();

    if(ui->pushButton_F4->isChecked())
        ui->pushButton_F4->click();

    emit stateChange(TABLE_SHOW);
    emit enter(28);
}

void seconlevel_f4::F0()
{
    ui->pushButton_F1->setChecked(false);
    ui->pushButton_F2->setChecked(false);
    ui->pushButton_F3->setChecked(false);
    ui->pushButton_F4->setChecked(false);
    ui->pushButton_F5->setChecked(false);
    ui->pushButton_F6->setChecked(false);

    if(spark_info->b_array[B_BOTTOM])
        ui->pushButton_F7->setChecked(true);
    else
        ui->pushButton_F7->setChecked(false);

    emit stateChange(TABLE_EDIT);
}

void seconlevel_f4::f1_Done(int r)
{
    if(r == 0)
        qDebug()<<"F1 Cancel!";
    else
        qDebug()<<"F1 OK!";
    parentWidget()->setFocus();
    emit stateChange(TABLE_EDIT);
    if(ui->pushButton_F1->isChecked())
        ui->pushButton_F1->click();
}

void seconlevel_f4::f2_Done(int r)
{
    if(r == 0){
        qDebug()<<"F2 Cancel!";
    }
    else{
        qDebug()<<"F2 OK!";
        spark_info->tableAuto(autotab->deep ,autotab->current,autotab->area,autotab->effect);
    }
    parentWidget()->setFocus();
    emit stateChange(TABLE_EDIT);
    if(ui->pushButton_F2->isChecked())
        ui->pushButton_F2->click();
}

void seconlevel_f4::f6_Done(int r)
{
    if(r == 0){
        qDebug()<<"F6 Cancel!";
        spark_info->setUInt(UINT_JUMP_H ,jump->height);
        spark_info->setUInt(UINT_JUMP_T ,jump->count);
    }
    else{
        qDebug()<<"F6 OK!";
        if(jump->check){
            spark_info->setUInt(UINT_JUMP_H ,jump->height);
            spark_info->setUInt(UINT_JUMP_T ,jump->count);
        }
        else{
            spark_info->setUInt(UINT_JUMP_H ,0);
            spark_info->setUInt(UINT_JUMP_T ,5);
        }
    }
    parentWidget()->setFocus();
    emit stateChange(TABLE_EDIT);
    if(ui->pushButton_F6->isChecked())
        ui->pushButton_F6->click();
}

seconlevel_f4::~seconlevel_f4()
{
    delete ui;
}
