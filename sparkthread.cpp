#include "sparkthread.h"
#include "sys/time.h"
#include "sparkinfo.h"
#include "qdebug.h"

SparkThread::SparkThread(QObject *parent) :
    QThread(parent)
{
    /*状态机初始状态为SEARCH*/
    state = SEARCH;

    x_start = 0;
    y_start = 0;
    z_start = 0;

    z_diff = 0;
}

void SparkThread::run()
{
    /*
    在没有开始放电加工或放电加工结束之后，当前深度值（L_DEEP_CURRENT）与当前Z轴位置（L_Z_CURRENT）保持相同。
    放电加工开始后当前深度值（L_DEEP_CURRENT）应为放电加工的最深深度。而目标深度（L_DEEP_TARGET）在没有放电
    开始时或放电结束时也应与当前Z轴位置（L_Z_CURRENT）保持相同，当放电开始后应为当前放电行的深度参数。

    当停止放电时，当前深度值（L_DEEP_CURRENT）与当前Z轴位置（L_Z_CURRENT）保持相同。当前的目标深度（L_DEEP_TARGET）
    与当前Z轴位置（L_Z_CURRENT）保持相同。且状态机恢复到初始状态（SEARCH）。
    */

    x_start = spark_info->l_array[L_X_CURRENT];
    y_start = spark_info->l_array[L_Y_CURRENT];
    z_start = spark_info->l_array[L_Z_CURRENT];

    state = SEARCH;

    while(spark_info->b_array[B_START]){
        if(spark_info->l_array[L_Z_CURRENT] > spark_info->l_array[L_DEEP_CURRENT])
            spark_info->setLong(L_DEEP_CURRENT ,spark_info->l_array[L_Z_CURRENT]);

        z_diff = spark_info->l_array[L_Z_CURRENT] - spark_info->l_array[L_DEEP_CURRENT];

        if(spark_info->l_array[L_Z_CURRENT] >= spark_info->table.Shendu[spark_info->uint_array[UINT_CURRENT_ROM]]){

            if(spark_info->uint_array[UINT_CURRENT_ROM] < spark_info->uint_array[UINT_END_ROW]){
                spark_info->setUInt(UINT_CURRENT_ROM ,++spark_info->uint_array[UINT_CURRENT_ROM]);
                spark_info->setLong(L_DEEP_TARGET ,spark_info->table.Shendu[spark_info->uint_array[UINT_CURRENT_ROM]]);
                Set_Row(spark_info->uint_array[UINT_CURRENT_ROM]);
            }
            else{
                spark_info->setUInt(UINT_CURRENT_ROM ,0x00);
                spark_info->setUInt(UINT_END_ROW ,0x00);
                spark_info->setUInt(UINT_START_ROW ,0x00);
                spark_info->setBool(B_SELECT ,false);

                //spark_info->setLong(L_DEEP_TARGET ,spark_info->l_array[L_Z_CURRENT]);
                //spark_info->setLong(L_DEEP_CURRENT ,spark_info->l_array[L_Z_CURRENT]);
                spark_info->setLong(L_DEEP_TARGET ,0x00);
                spark_info->setLong(L_DEEP_CURRENT ,0x00);


                spark_info->reverseBool(B_START);
                break;
            }

        }

        switch(state){
        case SEARCH:
            if(spark_info->uint_array[UINT_VOLTAGE] < 10){
                Z_Position_Control(spark_info->l_array[L_Z_COUNTER]);
                state = WORK;
                timer.restart();
                break;
            }
            Z_Velocity_Control(0x40);
            break;
        case WORK:
            if(timer.elapsed() > spark_info->table.Gongshi[spark_info->uint_array[UINT_CURRENT_ROM]]
                    && spark_info->table.Shenggao[spark_info->uint_array[UINT_CURRENT_ROM]] != 0){
                Z_Velocity_Control(0x80);
                state = UP;
                break;
            }
            break;
        case UP:
            if(z_diff < spark_info->table.Shenggao[spark_info->uint_array[UINT_CURRENT_ROM]]){
                Z_Position_Control(spark_info->table.Shenggao[spark_info->uint_array[UINT_CURRENT_ROM]]);
                state = DOWN;
                break;
            }
            break;
        case DOWN:
            if(spark_info->uint_array[UINT_VOLTAGE] < 10){
                Z_Position_Control(spark_info->l_array[L_Z_COUNTER]);
                state = WORK;
                timer.restart();
                break;
            }
            break;
        case END:
            break;
        default:
            break;
        }

        spark_info->setLong(L_Z_CURRENT ,++spark_info->l_array[L_Z_CURRENT]);
        msleep(200);
    }
    /*todo some thing*/
}

/*Z轴位置控制模式*/
void SparkThread::Z_Position_Control(long p)
{
    EightBytes tmp;
    tmp.longs = p;

    spark_info->c_array[C_Z_TP0] = tmp.bytes[0];
    spark_info->c_array[C_Z_TP1] = tmp.bytes[1];
    spark_info->c_array[C_Z_TP2] = tmp.bytes[2];

    _WRITE_BYTE_(C_Z_TP0);
    _WRITE_BYTE_(C_Z_TP1);
    _WRITE_BYTE_(C_Z_TP2);

    spark_info->c_array[C_Z_OT1] &= 0x18;
    _WRITE_BYTE_(C_Z_OT1);
}

/*Z轴速度控制模式*/
void SparkThread::Z_Velocity_Control(long v)
{
    EightBytes tmp;
    tmp.longs = v;

    spark_info->c_array[C_Z_TS0] = tmp.bytes[0];
    spark_info->c_array[C_Z_TS1] = tmp.bytes[1];

    _WRITE_BYTE_(C_Z_TS0);
    _WRITE_BYTE_(C_Z_TS1);

    spark_info->c_array[C_Z_OT1] &= 0x18;
    spark_info->c_array[C_Z_OT1] |= 0x01;
    _WRITE_BYTE_(C_Z_OT1);
}

/*Z轴PID参数设置*/
void SparkThread::Z_PID_Tune(char p, char i, char d, char t)
{
    spark_info->c_array[C_Z_STP] = p;
    spark_info->c_array[C_Z_STI] = i;
    spark_info->c_array[C_Z_STD] = d;
    spark_info->c_array[C_Z_STT] = t;

    _WRITE_BYTE_(C_Z_STP);
    _WRITE_BYTE_(C_Z_STI);
    _WRITE_BYTE_(C_Z_STD);
    _WRITE_BYTE_(C_Z_STT);
}

void SparkThread::Set_Row(unsigned int r)
{
    unsigned int row;
    row = r;

    if(spark_info->table.Index[row] > 0){

        /*设置放电电流*/
        if(spark_info->table.Dianliu[row] > 0x7f)
            spark_info->c_array[C_P_IO2] = 0x7f;
        else
            spark_info->c_array[C_P_IO2] = spark_info->table.Dianliu[row];
        _WRITE_BYTE_(C_P_IO2);

        /*电流大于设定值时开风扇散热*/
        if(spark_info->table.Dianliu[row] > 0x07){
            spark_info->setBool(B_FAN ,true);
        }

        /*设定放电高压*/
        spark_info->c_array[C_P_IO3] &= 0xf0;
        _WRITE_BYTE_(C_P_IO3);
        switch(spark_info->table.Gaoya[row]){
        case 1:
            spark_info->c_array[C_P_IO3] |= 0x00;
            break;
        case 2:
            spark_info->c_array[C_P_IO3] |= 0x01;
            break;
        case 3:
            spark_info->c_array[C_P_IO3] |= 0x02;
            break;
        case 4:
            spark_info->c_array[C_P_IO3] |= 0x08;
            break;
        case 5:
            spark_info->c_array[C_P_IO3] |= 0x09;
            break;
        case 6:
            spark_info->c_array[C_P_IO3] |= 0x0a;
            break;
        case 7:
            spark_info->c_array[C_P_IO3] |= 0x0c;
            break;
        case 8:
            spark_info->c_array[C_P_IO3] |= 0x0d;
            break;
        case 9:
            spark_info->c_array[C_P_IO3] |= 0x0e;
            break;
        default:
            break;
        }
        _WRITE_BYTE_(C_P_IO3);

        /*设定PWM波的占空比*/
        spark_info->c_array[C_U_PW0] = spark_info->table.Maikuan[row] & 0xff;
        spark_info->c_array[C_U_PW1] = spark_info->table.Maikuan[row] >> 8;

        spark_info->c_array[C_U_PG0] = spark_info->table.Maikuan[row] & 0xff;
        spark_info->c_array[C_U_PG1] = spark_info->table.Maikuan[row] >> 8;

        /*设定速度*/
        spark_info->c_array[C_U_OT0] &= 0xf8;
        spark_info->c_array[C_U_OT0] |= spark_info->table.Sudu[row] & 0x07;
        _WRITE_BYTE_(C_U_OT0);

        /*设定间隙电压*/
        spark_info->c_array[C_U_GVT] = spark_info->table.Jianxi[row];
        _WRITE_BYTE_(C_U_GVT);

        /*设定放电极性*/
        if(spark_info->table.Jixing[row] == 1){
            spark_info->c_array[C_P_IO1] |= 0x04;
        }
        else{
            spark_info->c_array[C_P_IO1] &= 0xfb;
        }
        _WRITE_BYTE_(C_P_IO1);

    }else{
        return;
    }
}

/*放电进程监听spark_info全局变量的开始结束信号*/
void SparkThread::sparkChange()
{
    if(spark_info->b_array[B_START]&&!isRunning()){
        start();
    }
    else if(!spark_info->b_array[B_START]&&isRunning()){
    }
}

SparkThread::~SparkThread()
{

}
