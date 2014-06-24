#include "mesgbox.h"
#include "ui_mesgbox.h"
#include "sparkinfo.h"

MesgBox::MesgBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MesgBox)
{
    ui->setupUi(this);

    label_ctime = new QcomboxTime(this ,tr("当前时间") ,true);
    label_ctime->updateTimer();
    connect(spark_info ,SIGNAL(startChange()) ,label_ctime ,SLOT(updateTimer()));
    ui->verticalLayout->addWidget(label_ctime);

    label_mtime = new QcomboxTime(this ,tr("加工时间") ,false);
    label_mtime->updateTimer();
    connect(spark_info ,SIGNAL(startChange()) ,label_mtime ,SLOT(updateTimer()));
    ui->verticalLayout->addWidget(label_mtime);

    label_cdeepth = new Qcombox(this ,tr("当前深度") ,L_DEEP_CURRENT);
    label_cdeepth->updateValue();
    connect(spark_info ,SIGNAL(longChange()) ,label_cdeepth ,SLOT(updateValue()));
    ui->verticalLayout->addWidget(label_cdeepth);

    label_mdeepth = new Qcombox(this ,tr("目标深度") ,L_DEEP_TARGET);
    label_mdeepth->updateValue();
    connect(spark_info ,SIGNAL(longChange()) ,label_mdeepth ,SLOT(updateValue()));
    ui->verticalLayout->addWidget(label_mdeepth);

    check_pump = new Qcheck(this ,tr("油泵"),B_PUMP);
    check_pump->updateCheck();
    connect(spark_info , SIGNAL(boolChange()) ,check_pump , SLOT(updateCheck()));
    ui->verticalLayout->addWidget(check_pump);

    check_alterpower = new Qcheck(this ,tr("副电源"),B_POWER);
    check_alterpower->updateCheck();
    connect(spark_info , SIGNAL(boolChange()) ,check_alterpower , SLOT(updateCheck()));
    ui->verticalLayout->addWidget(check_alterpower);

    check_lock = new Qcheck(this ,tr("锁定"),B_LOCK);
    check_lock->updateCheck();
    connect(spark_info , SIGNAL(boolChange()) ,check_lock , SLOT(updateCheck()));
    ui->verticalLayout->addWidget(check_lock);

    check_reverse = new Qcheck(this ,tr("反加工"),B_REVERSE);
    check_reverse->updateCheck();
    connect(spark_info , SIGNAL(boolChange()) ,check_reverse , SLOT(updateCheck()));
    ui->verticalLayout->addWidget(check_reverse);

    indicate_voltage = new Qindicate(this ,tr("电压"),255,0,UINT_VOLTAGE);
    indicate_voltage->updateValue();
    connect(spark_info ,SIGNAL(uintChange()) ,indicate_voltage ,SLOT(updateValue()));
    ui->verticalLayout->addWidget(indicate_voltage);

    indicate_speed = new Qindicate(this ,tr("速度"),50,10,UINT_SPEED);
    indicate_speed->updateValue();
    connect(spark_info ,SIGNAL(uintChange()) ,indicate_speed ,SLOT(updateValue()));
    ui->verticalLayout->addWidget(indicate_speed);

    parameter_jump = new Qparameter(this ,tr("跳高"),100,0,UINT_JUMP_H,10,0,UINT_JUMP_T);
    parameter_jump->updateValue();
    connect(spark_info ,SIGNAL(uintChange()) ,parameter_jump ,SLOT(updateValue()));
    ui->verticalLayout->addWidget(parameter_jump);
}

MesgBox::~MesgBox()
{
    delete ui;
}
