#include "scanthread.h"
#include "sparkinfo.h"
#include "sparkthread.h"
#include "fpga.h"
#include "unistd.h"
#include "sys/time.h"
#include "sys/reboot.h"
#include "qdebug.h"

/* #define TEST */

#ifdef TEST
struct timeval tv_last, tv_current;
struct timeval tv_diff;
float diff_ms = 0;
#endif


ScanThread::ScanThread(QObject *parent) :
    QThread(parent)
{
    a_cycle = 0;
    b_cycle = 0;
    c_cycle = 0;
    d_cycle = 0;
    e_cycle = 0;

    a_last = false;
    a_edge = NONE;
    b_last = false;
    b_edge = NONE;
    c_last = false;
    c_edge = NONE;
    d_last = false;
    d_edge = NONE;

    a_lock = false;

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

#ifdef TEST

        gettimeofday(&tv_current, NULL);
        tv_diff.tv_sec = tv_current.tv_sec - tv_last.tv_sec;
        tv_diff.tv_usec = tv_current.tv_usec - tv_last.tv_usec;
        diff_ms = tv_diff.tv_sec * 1000 + tv_diff.tv_usec / 1000;
        if(diff_ms > 1){
            printf("tv_sec; %ld\n", tv_diff.tv_sec);
            printf("tv_usec; %ld\n", tv_diff.tv_usec);
        }

        tv_last = tv_current;

#endif

        if(a_cycle == A_CYCLE){
            /*to do*/
            a_cycle = 0;
#ifndef Q_WS_X11
            sync();
            reboot(RB_AUTOBOOT);
            //reboot(RB_POWER_OFF);
#endif
        }

        if(b_cycle == B_CYCLE){
            /*to do*/
            spark_info->setUInt(UINT_BRIGHTNESS ,spark_info->uint_array[UINT_BRIGHTNESS] | 0x10);
            emit clear();
            b_cycle = 0;
        }

        if(d_cycle == D_CYCLE){
            /*to do*/
            d_cycle = 0;
            spark_info->setBool(B_FAN ,false);
        }

        if(e_cycle == E_CYCLE){
            /*to do*/
            e_cycle = 0;
#ifndef Q_WS_X11
            sync();
            reboot(RB_AUTOBOOT);
#endif
        }


        if(spark_info->b_array[B_SHUTDOWN])
            a_cycle++;
        if(spark_info->b_array[B_SLEEP])
            b_cycle++;
        if(spark_info->b_array[B_FAN] && !spark_info->b_array[B_START]){
            d_cycle++;
        }
        if(spark_info->b_array[B_REBOOT])
            e_cycle++;

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

        /*a_lock为false时开始扫描遥控板*/
        if(!a_lock){
            Scan_Panel();
        }

        /*移动位置*/
        Move();

        /*读取电压值*/
        spark_info->setUInt(UINT_VOLTAGE ,Voltage_Read());

        /*检查报警信息*/
        Check_Alert();

        msleep(1);
    }

    /*to do something*/
}

/*键盘按下信号处理函数，处理方向键和模式切换键*/
void ScanThread::keyPress(int k)
{
    /*键盘按键按下时获得互斥锁*/
    a_lock = true;

    switch(k)
    {
    case Qt::Key_Up:
        spark_info->c_array[C_Z_IN0] |= 0x10;
        break;
    case Qt::Key_Down:
        spark_info->c_array[C_Z_IN0] |= 0x20;
        break;
    case Qt::Key_Left:
        spark_info->c_array[C_U_IN0] |= 0x04;
        spark_info->setUInt(UINT_SPEED ,spark_info->uint_array[UINT_SPEED]+10);
        break;
    case Qt::Key_Right:
        spark_info->setUInt(UINT_SPEED ,spark_info->uint_array[UINT_SPEED]+10);
        spark_info->c_array[C_U_IN0] |= 0x04;
        break;
    default:
        break;
    }
}

/*键盘松开信号处理函数*/
void ScanThread::keyRelease(int k)
{
    switch(k)
    {
    case Qt::Key_Up:
        spark_info->c_array[C_Z_IN0] &= 0xef;
        break;
    case Qt::Key_Down:
        spark_info->c_array[C_Z_IN0] &= 0xdf;
        break;
    case Qt::Key_Left:
        spark_info->c_array[C_U_IN0] &= 0xfb;
        break;
    case Qt::Key_Right:
        spark_info->c_array[C_U_IN0] &= 0xfb;
        break;
    default:
        break;
    }

    /*键盘按键松开时释放互斥锁*/
    a_lock = false;
}

/*遥控板扫描*/
void ScanThread::Scan_Panel()
{
    _READ_BYTE_(C_Z_IN0);
    _READ_BYTE_(C_U_IN0);
    /*检测模式按键的边缘*/
    if(d_last){
        if(!(spark_info->c_array[C_U_IN0] & 0x04))
            d_edge = RISE;
        else
            d_edge = NONE;
    }
    else{
        if((spark_info->c_array[C_U_IN0] & 0x04))
            d_edge = FALL;
        else
            d_edge = NONE;
    }
    /*模式按键*/
    if((spark_info->c_array[C_U_IN0] & 0x04))
    {
        if(d_edge == FALL){
            spark_info->setUInt(UINT_SPEED ,spark_info->uint_array[UINT_SPEED]+10);
            d_edge = NONE;
        }
        d_last = true;
        return;
    }
    else{
        if(d_edge == RISE){
            d_edge = NONE;
        }
        d_last = false;
    }
}

/*移动位置，包括手动操作和归零操作*/
void ScanThread::Move()
{
    /*检测上升按键的边缘*/
    if(a_last){
        if(!(spark_info->c_array[C_Z_IN0] & 0x10))
            a_edge = RISE;
        else
            a_edge = NONE;
    }else{
        if((spark_info->c_array[C_Z_IN0] & 0x10))
            a_edge = FALL;
        else
            a_edge = NONE;
    }
    /*检测上升按键的边缘*/
    if(b_last){
        if(!(spark_info->c_array[C_Z_IN0] & 0x20))
            b_edge = RISE;
        else
            b_edge = NONE;
    }else{
        if((spark_info->c_array[C_Z_IN0] & 0x20))
            b_edge = FALL;
        else
            b_edge = NONE;
    }
    /*检测自动归零的边缘*/
    if(c_last){
        if(!spark_info->b_array[B_ZERO])
            c_edge = RISE;
        else
            c_edge = NONE;
    }else{
        if(spark_info->b_array[B_ZERO])
            c_edge = FALL;
        else
            c_edge = NONE;
    }
    /*上升按键*/
    if((spark_info->c_array[C_Z_IN0] & 0x10))
    {
        if(a_edge == FALL){
            switch(spark_info->uint_array[UINT_SPEED]){
            case 10:
                SparkThread::Z_Position_Control(Z_Count() + 10);
                break;
            case 20:
                SparkThread::Z_Velocity_Control(0x20);
                break;
            case 30:
                SparkThread::Z_Position_Control(Z_Count() + 30);
                break;
            case 40:
                SparkThread::Z_Velocity_Control(0x400);
                break;
            case 50:
                SparkThread::Z_Position_Control(Z_Count() + 50);
                break;
            default:
                break;
            }
            a_edge = NONE;
        }
        a_last = true;
        return;
    }else{
        if(a_edge == RISE){
            switch(spark_info->uint_array[UINT_SPEED]){
            case 10:
                break;
            case 20:
                SparkThread::Z_Position_Control(Z_Count());
                break;
            case 30:
                break;
            case 40:
                SparkThread::Z_Position_Control(Z_Count());
                break;
            case 50:
                break;
            default:
                break;
            }
            a_edge = NONE;
        }
        a_last = false;
    }
    /*下降按键*/
    if((spark_info->c_array[C_Z_IN0] & 0x20))
    {
        if(b_edge == FALL){
            switch(spark_info->uint_array[UINT_SPEED]){
            case 10:
                SparkThread::Z_Position_Control(Z_Count() - 10);
                break;
            case 20:
                SparkThread::Z_Velocity_Control(0xffe0);
                break;
            case 30:
                SparkThread::Z_Position_Control(Z_Count() - 30);
                break;
            case 40:
                SparkThread::Z_Velocity_Control(0xfc00);
                break;
            case 50:
                SparkThread::Z_Position_Control(Z_Count() - 50);
                break;
            default:
                break;
            }
            b_edge = NONE;
        }
        b_last = true;
        return;
    }else{
        if(b_edge == RISE){
            switch(spark_info->uint_array[UINT_SPEED]){
            case 10:
                break;
            case 20:
                SparkThread::Z_Position_Control(Z_Count());
                break;
            case 30:
                break;
            case 40:
                SparkThread::Z_Position_Control(Z_Count());
                break;
            case 50:
                break;
            default:
                break;
            }
            b_edge = NONE;
        }
        b_last = false;
    }

    /*自动归零*/
    if(spark_info->b_array[B_ZERO]){
        if(c_edge == FALL){
            /*归零操作时控制放电头为速度模式*/
            if(spark_info->b_array[B_REVERSE]){
                if(spark_info->uint_array[UINT_VOLTAGE] < 10){
                    SparkThread::Z_Velocity_Control(0xffeb);
                }else{
                    SparkThread::Z_Velocity_Control(0xffd0);
                }
            }else{
                if(spark_info->uint_array[UINT_VOLTAGE] < 10){
                    SparkThread::Z_Velocity_Control(0x15);
                }else{
                    SparkThread::Z_Velocity_Control(0x30);
                }
            }
            qDebug()<<"fall";
        }else{
            /*电压过低时表示已经接触到工件*/
            if(spark_info->uint_array[UINT_VOLTAGE] < 1){
                 SparkThread::Z_Position_Control(Z_Count());
            }else{
                if(spark_info->b_array[B_REVERSE]){
                    SparkThread::Z_Velocity_Control(0xffeb);
                }else{
                    SparkThread::Z_Velocity_Control(0x15);
                }
            }
            qDebug()<<"none";
        }
        c_last = true;
    }
    else{
        /*手动停止归零操作*/
        if(c_edge == RISE){
            qDebug()<<"rise";
            SparkThread::Z_Position_Control(Z_Count());
        }
        c_last = false;
    }
}

long ScanThread::X_Count()
{
    long ret = 0;
/*   _READ_BYTE_(C_X_CP0);      */
/*   _READ_BYTE_(C_X_CP1);      */
/*   _READ_BYTE_(C_X_CP2);      */

    spark_info->c_array[C_R_0H] = 0x01;
    _WRITE_BYTE_(C_R_0H);
    spark_info->c_array[C_R_0L] = 0x11;
    _WRITE_BYTE_(C_R_0L);

    _READ_BYTE_(C_R_6L);
    spark_info->c_array[C_X_CP0] = spark_info->c_array[C_R_6L];
    _READ_BYTE_(C_R_6H);
    spark_info->c_array[C_X_CP1] = spark_info->c_array[C_R_6H];
    _READ_BYTE_(C_R_7L);
    spark_info->c_array[C_X_CP2] = spark_info->c_array[C_R_7L];

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

/*    _READ_BYTE_(C_Y_CP0);     */
/*    _READ_BYTE_(C_Y_CP1);     */
/*    _READ_BYTE_(C_Y_CP2);     */

    spark_info->c_array[C_R_0H] = 0x02;
    _WRITE_BYTE_(C_R_0H);
    spark_info->c_array[C_R_0L] = 0x11;
    _WRITE_BYTE_(C_R_0L);

    _READ_BYTE_(C_R_6L);
    spark_info->c_array[C_Y_CP0] = spark_info->c_array[C_R_6L];
    _READ_BYTE_(C_R_6H);
    spark_info->c_array[C_Y_CP1] = spark_info->c_array[C_R_6H];
    _READ_BYTE_(C_R_7L);
    spark_info->c_array[C_Y_CP2] = spark_info->c_array[C_R_7L];

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

/*    _READ_BYTE_(C_Z_CP0);     */
/*    _READ_BYTE_(C_Z_CP1);     */
/*    _READ_BYTE_(C_Z_CP2);     */

    spark_info->c_array[C_R_0H] = 0x04;
    _WRITE_BYTE_(C_R_0H);
    spark_info->c_array[C_R_0L] = 0x11;
    _WRITE_BYTE_(C_R_0L);

    _READ_BYTE_(C_R_6L);
    spark_info->c_array[C_Z_CP0] = spark_info->c_array[C_R_6L];
    _READ_BYTE_(C_R_6H);
    spark_info->c_array[C_Z_CP1] = spark_info->c_array[C_R_6H];
    _READ_BYTE_(C_R_7L);
    spark_info->c_array[C_Z_CP2] = spark_info->c_array[C_R_7L];

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

/*    _READ_BYTE_(C_X_CS0);     */
/*    _READ_BYTE_(C_X_CS1);     */

    spark_info->c_array[C_R_0H] = 0x01;
    _WRITE_BYTE_(C_R_0H);
    spark_info->c_array[C_R_0L] = 0x12;
    _WRITE_BYTE_(C_R_0L);

    _READ_BYTE_(C_R_6L);
    spark_info->c_array[C_X_CS0] = spark_info->c_array[C_R_6L];
    _READ_BYTE_(C_R_6H);
    spark_info->c_array[C_X_CS1] = spark_info->c_array[C_R_6H];

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

 /*   _READ_BYTE_(C_Y_CS0);     */
 /*   _READ_BYTE_(C_Y_CS1);     */

    spark_info->c_array[C_R_0H] = 0x02;
    _WRITE_BYTE_(C_R_0H);
    spark_info->c_array[C_R_0L] = 0x12;
    _WRITE_BYTE_(C_R_0L);

    _READ_BYTE_(C_R_6L);
    spark_info->c_array[C_Y_CS0] = spark_info->c_array[C_R_6L];
    _READ_BYTE_(C_R_6H);
    spark_info->c_array[C_Y_CS1] = spark_info->c_array[C_R_6H];

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

/*    _READ_BYTE_(C_Z_CS0);     */
/*    _READ_BYTE_(C_Z_CS1);     */

    spark_info->c_array[C_R_0H] = 0x04;
    _WRITE_BYTE_(C_R_0H);
    spark_info->c_array[C_R_0L] = 0x12;
    _WRITE_BYTE_(C_R_0L);

    _READ_BYTE_(C_R_6L);
    spark_info->c_array[C_Z_CS0] = spark_info->c_array[C_R_6L];
    _READ_BYTE_(C_R_6H);
    spark_info->c_array[C_Z_CS1] = spark_info->c_array[C_R_6H];

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

/*    _READ_BYTE_(C_U_DVT);     */
/*    ret = spark_info->c_array[C_U_DVT];   */

    spark_info->c_array[C_R_0H] = 0x40;
    _WRITE_BYTE_(C_R_0H);
    spark_info->c_array[C_R_0L] = 0x1b;
    _WRITE_BYTE_(C_R_0L);

    _READ_BYTE_(C_R_6L);
    spark_info->c_array[C_U_DVT] = spark_info->c_array[C_R_6L];

    ret = spark_info->c_array[C_U_DVT];
    return ret;
}

void ScanThread::Check_Alert()
{
    /*更新IO端口*/
    _READ_BYTE_(C_U_IN0);
    _READ_BYTE_(C_Z_IN0);

    /*火警*/
    if(spark_info->b_array[B_FIRE]){
        if(spark_info->c_array[C_U_IN0] & 0x80){
            spark_info->setBool(B_FIRE_ALERT ,true);
        }
        else{
            spark_info->setBool(B_FIRE_ALERT ,false);
        }
    }
    else{
        spark_info->setBool(B_FIRE_ALERT ,false);
    }

    /*油位*/
    if(spark_info->b_array[B_OIL]){
        if(spark_info->c_array[C_U_IN0] & 0x02){
            spark_info->setBool(B_OIL_ALERT ,true);
        }
        else{
            spark_info->setBool(B_OIL_ALERT ,false);
        }
    }
    else{
        spark_info->setBool(B_OIL_ALERT ,false);
    }

    /*Z上限*/
    if(spark_info->b_array[B_Z_UP]){
        if(spark_info->c_array[C_Z_IN0] & 0x80){
            spark_info->setBool(B_Z_UP_ALERT ,true);
        }
        else{
            spark_info->setBool(B_Z_UP_ALERT ,false);
        }
    }
    else{
        spark_info->setBool(B_Z_UP_ALERT ,false);
    }

    /*Z下限*/
    if(spark_info->b_array[B_Z_DOWN]){
        if(spark_info->c_array[C_Z_IN0] & 0x04){
            spark_info->setBool(B_Z_DOWN_ALERT ,true);
        }
        else{
            spark_info->setBool(B_Z_DOWN_ALERT ,false);
        }
    }
    else{
        spark_info->setBool(B_Z_DOWN_ALERT ,false);
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

    if(spark_info->b_array[B_REVERSE]){
        spark_info->c_array[C_Z_OT0] |= 0x06;
        spark_info->c_array[C_Z_OT0] &= 0xfe;
    }
    else{
        spark_info->c_array[C_Z_OT0] |= 0x04;
        spark_info->c_array[C_Z_OT0] &= 0xfc;
    }

    /*更新IO端口*/
    _WRITE_BYTE_(C_P_IO0);
    _WRITE_BYTE_(C_P_IO1);
    _WRITE_BYTE_(C_U_OT1);
    _WRITE_BYTE_(C_Z_OT0);
}

ScanThread::~ScanThread()
{

}
