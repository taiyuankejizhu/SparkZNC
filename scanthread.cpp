#include "scanthread.h"
#include "sparkinfo.h"
#include "fpga.h"
#include "qdebug.h"

ScanThread::ScanThread(QObject *parent) :
    QThread(parent)
{
    a_cycle = 0;
    b_cycle = 0;

    /*初始化IO*/
    IOZ0_Write(spark_info->c_array[C_IOZ_0]);

    OSC0_Write(spark_info->c_array[C_OTPS_0]);
    OSC1_Write(spark_info->c_array[C_OTPS_1]);
    OSC2_Write(0xff);

    IO0_Write(spark_info->c_array[C_IO_0]);
    IO1_Write(spark_info->c_array[C_IO_1]);
    IO2_Write(spark_info->c_array[C_IO_2]);
    IO3_Write(spark_info->c_array[C_IO_3]);
    IO4_Write(spark_info->c_array[C_IO_4]);

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

        if(spark_info->b_array[B_SHUTDOWN])
            a_cycle++;
        if(spark_info->b_array[B_SLEEP])
            b_cycle++;

        /*布尔数组发生更新，重新输出IO*/
        if(spark_info->b_array[B_UPDATE]){

            if(spark_info->b_array[B_SOUND])
                spark_info->c_array[C_IO_0] |= 0x08;
            else
                spark_info->c_array[C_IO_0] &= 0xf7;

            if(spark_info->b_array[B_PUMP])
                spark_info->c_array[C_IO_1] |= 0x10;
            else
                spark_info->c_array[C_IO_1] &= 0xef;

            if(spark_info->b_array[B_POWER])
                spark_info->c_array[C_IO_1] |= 0x80;
            else
                spark_info->c_array[C_IO_1] &= 0x7f;

            /*更新IO端口*/
            IO0_Write(spark_info->c_array[C_IO_0]);
            IO1_Write(spark_info->c_array[C_IO_1]);

            spark_info->b_array[B_UPDATE] = false;
        }
        else{
            spark_info->c_array[C_IO_0] = spark_info->c_array[C_IO_0];
            spark_info->c_array[C_IO_1] = spark_info->c_array[C_IO_1];
        }

        /*读取光栅计数值*/
        spark_info->setLong(L_X_COUNTER ,X_Count());
        spark_info->setLong(L_Y_COUNTER ,Y_Count());
        spark_info->setLong(L_Z_COUNTER ,Z_Count());

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

        msleep(200);
    }

    /*to do something*/
}
