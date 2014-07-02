#include "scanthread.h"
#include "sparkinfo.h"
#include "fpga.h"
#include "qdebug.h"

ScanThread::ScanThread(QObject *parent) :
    QThread(parent)
{
    a_cycle = 0;
    b_cycle = 0;
    c_cycle = 0;
    d_cycle = 0;

    /*初始化IO*/
    _WRITE_BYTE_(C_Z_OT0);
    _WRITE_BYTE_(C_Z_OT1);

    _WRITE_BYTE_(C_U_OT0);
    _WRITE_BYTE_(C_U_OT1);
    _WRITE_BYTE_(C_U_OT2);

    _WRITE_BYTE_(C_P_IO0);
    _WRITE_BYTE_(C_P_IO1);
    _WRITE_BYTE_(C_P_IO2);
    _WRITE_BYTE_(C_P_IO3);
    _WRITE_BYTE_(C_P_IO4);

}

void ScanThread::run()
{

    while(1){

        if(a_cycle == A_CYCLE){
            /*to do*/
            a_cycle = 0;
        }

        if(b_cycle == B_CYCLE){
            /*to do*/
            b_cycle = 0;
        }

        if(d_cycle == D_CYCLE){
            /*to do*/
            d_cycle = 0;
            spark_info->setBool(B_FAN ,false);
        }

        if(spark_info->b_array[B_SHUTDOWN])
            a_cycle++;
        if(spark_info->b_array[B_SLEEP])
            b_cycle++;
        if(spark_info->b_array[B_FAN] && !spark_info->b_array[B_START]){
            d_cycle++;
        }

        /*布尔数组发生更新，重新输出IO*/
        if(spark_info->b_array[B_UPDATE]){
            Update_Relay();
            spark_info->b_array[B_UPDATE] = false;
        }

        /*读取光栅计数值*/
        //spark_info->setLong(L_X_COUNTER ,X_Count());
        //spark_info->setLong(L_Y_COUNTER ,Y_Count());
        //spark_info->setLong(L_Z_COUNTER ,Z_Count());

        /*读取速度值*/
        if(c_cycle == C_CYCLE){
            spark_info->setLong(L_X_VELOCITY ,X_Velocity());
            spark_info->setLong(L_Y_VELOCITY ,Y_Velocity());
            spark_info->setLong(L_Z_VELOCITY ,Z_Velocity());
            c_cycle = 0;
        }
        c_cycle++;

        /*读取电压值*/
        spark_info->setUInt(UINT_VOLTAGE ,Voltage_Read());

        /*检查报警信息*/
        Check_Alert();

        msleep(200);
    }

    /*to do something*/
}

long ScanThread::X_Count()
{
    long ret = 0;
    _READ_BYTE_(C_X_CP0);
    _READ_BYTE_(C_X_CP1);
    _READ_BYTE_(C_X_CP2);

    if(spark_info->c_array[C_X_CP2] & 0x80)
        ret = 0xff;
    else
        ret = 0x00;
    ret <<= 8;
    ret += spark_info->c_array[C_X_CP2];
    ret <<= 8;
    ret += spark_info->c_array[C_X_CP1];
    ret <<= 8;
    ret += spark_info->c_array[C_X_CP0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

long ScanThread::Y_Count()
{
    long ret = 0;

    _READ_BYTE_(C_Y_CP0);
    _READ_BYTE_(C_Y_CP1);
    _READ_BYTE_(C_Y_CP2);

    if(spark_info->c_array[C_Y_CP0] & 0x80)
        ret = 0xff;
    else
        ret = 0x00;
    ret <<= 8;
    ret += spark_info->c_array[C_Y_CP2];
    ret <<= 8;
    ret += spark_info->c_array[C_Y_CP1];
    ret <<= 8;
    ret += spark_info->c_array[C_Y_CP0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

long ScanThread::Z_Count()
{
    long ret = 0;

    _READ_BYTE_(C_Z_CP0);
    _READ_BYTE_(C_Z_CP1);
    _READ_BYTE_(C_Z_CP2);

    if(spark_info->c_array[C_Z_CP2] & 0x80)
        ret = 0xff;
    else
        ret = 0x00;
    ret <<= 8;
    ret += spark_info->c_array[C_Z_CP2];
    ret <<= 8;
    ret += spark_info->c_array[C_Z_CP1];
    ret <<= 8;
    ret += spark_info->c_array[C_Z_CP0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

long ScanThread::X_Velocity()
{
    long ret = 0;

    _READ_BYTE_(C_X_CS0);
    _READ_BYTE_(C_X_CS1);

    if(_READ_BYTE_(C_X_CS1) & 0x80)
        ret = 0xffff;
    else
        ret = 0x0000;
    ret <<= 8;
    ret += spark_info->c_array[C_X_CS1];
    ret <<= 8;
    ret += spark_info->c_array[C_X_CS0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

long ScanThread::Y_Velocity()
{
    long ret = 0;

    _READ_BYTE_(C_Y_CS0);
    _READ_BYTE_(C_Y_CS1);

    if(_READ_BYTE_(C_Y_CS1) & 0x80)
        ret = 0xffff;
    else
        ret = 0x0000;
    ret <<= 8;
    ret += spark_info->c_array[C_Y_CS1];
    ret <<= 8;
    ret += spark_info->c_array[C_Y_CS0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

long ScanThread::Z_Velocity()
{
    long ret = 0;

    _READ_BYTE_(C_Z_CS0);
    _READ_BYTE_(C_Z_CS1);

    if(_READ_BYTE_(C_Z_CS1) & 0x80)
        ret = 0xffff;
    else
        ret = 0x0000;
    ret <<= 8;
    ret += spark_info->c_array[C_Z_CS1];
    ret <<= 8;
    ret += spark_info->c_array[C_Z_CS0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

long ScanThread::Z_Origin()
{
    long ret = 0;

    _READ_BYTE_(C_Z_OP0);
    _READ_BYTE_(C_Z_OP1);
    _READ_BYTE_(C_Z_OP2);

    if(_READ_BYTE_(C_Z_OP2) & 0x80)
        ret = 0xff;
    else
        ret = 0x00;

    ret <<= 8;
    ret += spark_info->c_array[C_Z_OP2];
    ret <<= 8;
    ret += spark_info->c_array[C_Z_OP1];
    ret <<= 8;
    ret += spark_info->c_array[C_Z_OP0];

    return ret;
}

char ScanThread::Voltage_Read()
{
    char ret = 0;
    _READ_BYTE_(C_U_DVT);
    ret = spark_info->c_array[C_U_DVT];
    return ret;
}

void ScanThread::Check_Alert()
{
    /*更新IO端口*/
    _READ_BYTE_(C_U_IN0);
    _READ_BYTE_(C_Z_IN0);

    /*火警*/
    if(spark_info->c_array[C_U_IN0] & 0x80){
        spark_info->setBool(B_FIRE ,true);
    }
    else{
        spark_info->setBool(B_FIRE ,false);
    }

    /*Z上限*/
    if(spark_info->c_array[C_Z_IN0] & 0x08){
        spark_info->setBool(B_Z_UP ,true);
    }
    else{
        spark_info->setBool(B_Z_UP ,false);
    }

    /*Z下限*/
    if(spark_info->c_array[C_Z_IN0] & 0x04){
        spark_info->setBool(B_Z_DOWN ,true);
    }
    else{
        spark_info->setBool(B_Z_DOWN ,false);
    }
}

void ScanThread::Update_Relay()
{
    if(spark_info->b_array[B_SOUND])
        spark_info->c_array[C_P_IO0] |= 0x08;
    else
        spark_info->c_array[C_P_IO0] &= 0xf7;

    if(spark_info->b_array[B_POWER_ALL])
        spark_info->c_array[C_P_IO1] |= 0x02;
    else
        spark_info->c_array[C_P_IO1] &= 0xfd;

    if(spark_info->b_array[B_PUMP])
        spark_info->c_array[C_P_IO1] |= 0x10;
    else
        spark_info->c_array[C_P_IO1] &= 0xef;

    if(spark_info->b_array[B_TRANS_A])
        spark_info->c_array[C_P_IO1] |= 0x20;
    else
        spark_info->c_array[C_P_IO1] &= 0xdf;

    if(spark_info->b_array[B_FAN])
        spark_info->c_array[C_P_IO1] |= 0x40;
    else
        spark_info->c_array[C_P_IO1] &= 0xbf;

    if(spark_info->b_array[B_POWER])
        spark_info->c_array[C_P_IO1] |= 0x80;
    else
        spark_info->c_array[C_P_IO1] &= 0x7f;

    if(spark_info->b_array[B_START])
        spark_info->c_array[C_U_OT1] |= 0x01;
    else
        spark_info->c_array[C_U_OT1] &= 0xfe;

    if(spark_info->b_array[B_OSCF])
        spark_info->c_array[C_U_OT1] |= 0x80;
    else
        spark_info->c_array[C_U_OT1] &= 0x7f;

    /*更新IO端口*/
    _WRITE_BYTE_(C_P_IO0);
    _WRITE_BYTE_(C_P_IO1);
    _WRITE_BYTE_(C_U_OT1);
}

ScanThread::~ScanThread()
{

}
