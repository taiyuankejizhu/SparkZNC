#include "scanthread.h"
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
    e_last = false;
    e_edge = NONE;
    f_last = false;
    f_edge = NONE;

    a_lock = false;

    mouse_fd = inotify_init();
    mouse_wd = inotify_add_watch(mouse_fd, DEV_DIR, IN_MODIFY | IN_DELETE | IN_CREATE);
    mouse_event = NULL;

    if (mouse_fd < 0 || mouse_wd < 0){
        printf("Fail to initialize inotify.\n");
    }

    program.Axis = Hand_NULL_AXIS;
    program.Position_Control = NULL;
    program.Velocity_Control = NULL;
    program.Count = NULL;

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

    AxisSwitch(&spark_info->hand ,Hand_NULL_AXIS ,Hand_A_DRIECT);
    AxisSwitch(&program ,Hand_NULL_AXIS ,Hand_A_DRIECT);

}

void ScanThread::run()
{

//    char mouse_buf[BUF_LENGTH];
//    int length = 0;
//    int index = 0;

    while(1){

/*        length = read(mouse_fd, mouse_buf, BUF_LENGTH);
        if(index < length){
            mouse_event = (struct inotify_event *) &mouse_buf[index];
            if (mouse_event -> mask & IN_DELETE){
                qDebug()<<"del:"<<mouse_event->name;
                if (!strcmp ("mouse1", mouse_event->name)){
                    emit cursor(false);
                }
            }else if (mouse_event -> mask & IN_CREATE){
                qDebug()<<"create:"<<mouse_event->name;
                if (!strcmp ("mouse1", mouse_event->name)){
                    emit cursor(true);
                }
            }
            index = 0;
        }*/

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

/*切换当前移动轴*/
void ScanThread::AxisSwitch(HandMove *hm ,HandAxis a ,HandDirect d)
{
    switch(a){
    case Hand_X_AXIS:
        hm->Axis = Hand_X_AXIS;
        hm->Driect = d;
        hm->Position_Control = SparkThread::Null_Position_Control;
        hm->Velocity_Control = SparkThread::Null_Velocity_Control;
        hm->Voltage_Control = SparkThread::Null_Voltage_Control;
        hm->PID_Tune = SparkThread::Null_PID_Tune;
        hm->Count = ScanThread::X_Count;
        break;
    case Hand_Y_AXIS:
        hm->Axis = Hand_Y_AXIS;
        hm->Driect = d;
        hm->Position_Control = SparkThread::Null_Position_Control;
        hm->Velocity_Control = SparkThread::Null_Velocity_Control;
        hm->Voltage_Control = SparkThread::Null_Voltage_Control;
        hm->PID_Tune = SparkThread::Null_PID_Tune;
        hm->Count = ScanThread::Y_Count;
        break;
    case Hand_Z_AXIS:
        hm->Axis = Hand_Z_AXIS;
        hm->Driect = d;
        hm->Position_Control = SparkThread::Z_Position_Control;
        hm->Velocity_Control = SparkThread::Z_Velocity_Control;
        hm->Voltage_Control = SparkThread::Z_Voltage_Control;
        hm->PID_Tune = SparkThread::Z_PID_Tune;
        hm->Count = ScanThread::Z_Count;
        break;
    case Hand_NULL_AXIS:
        hm->Axis = Hand_NULL_AXIS;
        hm->Driect = d;
        hm->Position_Control = SparkThread::Null_Position_Control;
        hm->Velocity_Control = SparkThread::Null_Velocity_Control;
        hm->Voltage_Control = SparkThread::Null_Voltage_Control;
        hm->PID_Tune = SparkThread::Null_PID_Tune;
        hm->Count = ScanThread::Null_Count;
        break;
    default:
        break;
    }
}

/*实际操作运动轴*/
void ScanThread::DoMove(HandMove *hm)
{

    if(hm->Axis == Hand_NULL_AXIS){
        return;
    }

    if(hm->Mode == Hand_Position_MODE){
        if(hm->Driect == Hand_A_DRIECT)
            hm->Position_Control(hm->Count() + hm->Step);
        else
            hm->Position_Control(hm->Count() - hm->Step);
    }else if(hm->Mode == Hand_Velocity_MODE){
        if(hm->Driect == Hand_A_DRIECT)
            hm->Velocity_Control(hm->Speed);
        else
            hm->Velocity_Control(- hm->Speed);
    }

}

/*实际停止运动轴*/
void ScanThread::DoStop(HandMove *hm)
{
    if(hm->Axis == Hand_NULL_AXIS){
        return;
    }

    if(hm->Mode == Hand_Velocity_MODE){
        if(hm->Driect == Hand_A_DRIECT)
            hm->Position_Control(hm->Count());
        else
            hm->Position_Control(hm->Count());
    }
}

/*移动位置，包括手动操作和归零操作*/
void ScanThread::Move()
{
    /*检测上升按键的边缘*/
    if(a_last){
        if(!(spark_info->c_array[C_Z_IN0] & 0x10)){
            a_edge = RISE;
        }
        else
            a_edge = NONE;
    }else{
        if((spark_info->c_array[C_Z_IN0] & 0x10)){
            if(spark_info->hand.Mutex == Hand_NULL_MUTEX){
                a_edge = FALL;
                AxisSwitch(&spark_info->hand ,Hand_Z_AXIS ,Hand_A_DRIECT);
                spark_info->hand.Mutex = Hand_MOVE_MUTEX;
            }else{
                qDebug()<<"Can't handle this axis!";
            }
        }
        else
            a_edge = NONE;
    }

    /*检测上升按键的边缘*/
    if(b_last){
        if(!(spark_info->c_array[C_Z_IN0] & 0x20)){
            b_edge = RISE;
        }
        else
            b_edge = NONE;
    }else{
        if((spark_info->c_array[C_Z_IN0] & 0x20)){
            if(spark_info->hand.Mutex == Hand_NULL_MUTEX){
                b_edge = FALL;
                AxisSwitch(&spark_info->hand ,Hand_Z_AXIS ,Hand_B_DRIECT);
                spark_info->hand.Mutex = Hand_MOVE_MUTEX;
            }else{
                qDebug()<<"Can't handle this axis!";
            }
        }
        else
            b_edge = NONE;
    }

    /*检测自动归零的边缘*/
    if(c_last){
        if(!spark_info->b_array[B_ZERO]){
            c_edge = RISE;
        }
        else
            c_edge = NONE;
    }else{
        if(spark_info->b_array[B_ZERO]){
            if(spark_info->hand.Mutex == Hand_NULL_MUTEX){
                c_edge = FALL;
                AxisSwitch(&program ,Hand_Z_AXIS ,Hand_A_DRIECT);
                program.Mutex = Hand_ZERO_MUTEX;
                spark_info->hand.Mutex = Hand_ZERO_MUTEX;
            }
        }
        else
            c_edge = NONE;
    }

    /*检测Z轴向上归原点的边缘*/
    if(e_last){
        if(!spark_info->b_array[B_HOME_Z_UP]){
            e_edge = RISE;
        }
        else
            e_edge = NONE;
    }else{
        if(spark_info->b_array[B_HOME_Z_UP]){
            if(spark_info->hand.Mutex == Hand_NULL_MUTEX){
                e_edge = FALL;
                AxisSwitch(&program ,Hand_Z_AXIS ,Hand_A_DRIECT);
                program.Mutex = Hand_HOME_MUTEX;
                spark_info->hand.Mutex = Hand_HOME_MUTEX;
            }
        }
        else
            e_edge = NONE;
    }

    /*检测Z轴向下归原点的边缘*/
    if(f_last){
        if(!spark_info->b_array[B_HOME_Z_DOWN]){
            f_edge = RISE;
        }
        else
            f_edge = NONE;
    }else{
        if(spark_info->b_array[B_HOME_Z_DOWN]){
            if(spark_info->hand.Mutex == Hand_NULL_MUTEX){
                f_edge = FALL;
                AxisSwitch(&program ,Hand_Z_AXIS ,Hand_B_DRIECT);
                program.Mutex = Hand_HOME_MUTEX;
                spark_info->hand.Mutex = Hand_HOME_MUTEX;
            }
        }
        else
            f_edge = NONE;
    }

    switch(spark_info->hand.Mutex){
    case Hand_NULL_MUTEX:
        break;
    case Hand_MOVE_MUTEX:
        HandleMove();
        break;
    case Hand_ZERO_MUTEX:
        ZeroMove();
        break;
    case Hand_HOME_MUTEX:
        HomeMove();
        break;
    case Hand_POINT_MUTEX:
    case Hand_CALIB_MUTEX:
    case Hand_SPARK_MUTEX:
        break;
    }

}

/*手动按键操作*/
void ScanThread::HandleMove()
{

    /*上升按键*/
    if((spark_info->c_array[C_Z_IN0] & 0x10))
    {
        if(a_edge == FALL){
            DoMove(&spark_info->hand);
            a_edge = NONE;
        }
        a_last = true;
        return;
    }else{
        if(a_edge == RISE){
            DoStop(&spark_info->hand);
            AxisSwitch(&spark_info->hand ,Hand_NULL_AXIS ,Hand_A_DRIECT);
            spark_info->hand.Mutex = Hand_NULL_MUTEX;
            a_edge = NONE;
        }
        a_last = false;
    }
    /*下降按键*/
    if((spark_info->c_array[C_Z_IN0] & 0x20))
    {
        if(b_edge == FALL){
            DoMove(&spark_info->hand);
            b_edge = NONE;
        }
        b_last = true;
        return;
    }else{
        if(b_edge == RISE){
            DoStop(&spark_info->hand);
            AxisSwitch(&spark_info->hand ,Hand_NULL_AXIS ,Hand_A_DRIECT);
            spark_info->hand.Mutex = Hand_NULL_MUTEX;
            b_edge = NONE;
        }
        b_last = false;
    }

}

/*自动归零*/
void ScanThread::ZeroMove()
{

    if(spark_info->b_array[B_ZERO]){
        if(c_edge == FALL){
            /*归零操作时控制放电头为速度模式*/
            if(spark_info->b_array[B_REVERSE]){
                if(spark_info->uint_array[UINT_VOLTAGE] < 10){
                    program.Speed = 200;
                    program.Mode = Hand_Velocity_MODE;
                    program.Driect = Hand_A_DRIECT;
                    DoMove(&program);
                }else{
                    program.Speed = 400;
                    program.Mode = Hand_Velocity_MODE;
                    program.Driect = Hand_A_DRIECT;
                    DoMove(&program);
                }
            }else{
                if(spark_info->uint_array[UINT_VOLTAGE] < 10){
                    program.Speed = 200;
                    program.Mode = Hand_Velocity_MODE;
                    program.Driect = Hand_B_DRIECT;
                    DoMove(&program);
                }else{
                    program.Speed = 400;
                    program.Mode = Hand_Velocity_MODE;
                    program.Driect = Hand_B_DRIECT;
                    DoMove(&program);
                }
            }
        }else{
            /*电压过低时表示已经接触到工件*/
            if(spark_info->uint_array[UINT_VOLTAGE] < 1){
                DoStop(&program);
            }else{
                if(spark_info->b_array[B_REVERSE]){
                    program.Speed = 50;
                    program.Mode = Hand_Velocity_MODE;
                    program.Driect = Hand_A_DRIECT;
                    DoMove(&program);
                }else{
                    program.Speed = 50;
                    program.Mode = Hand_Velocity_MODE;
                    program.Driect = Hand_B_DRIECT;
                    DoMove(&program);
                }
            }
        }
        c_last = true;
    }
    else{
        /*手动停止归零操作*/
        if(c_edge == RISE){
            DoStop(&program);
            AxisSwitch(&program ,Hand_NULL_AXIS ,Hand_A_DRIECT);
            program.Mutex = Hand_NULL_MUTEX;
            spark_info->hand.Mutex = Hand_NULL_MUTEX;
        }
        c_last = false;
    }

}

/*归原点操作*/
void ScanThread::HomeMove()
{
    if(spark_info->b_array[B_HOME_Z_UP]){
        if(e_edge == FALL){
            qDebug()<<"home up";
            program.Speed = 400;
            program.Mode = Hand_Velocity_MODE;
            program.Driect = Hand_A_DRIECT;
            DoMove(&program);
        }else{
            if(spark_info->b_array[B_Z_UP_ALERT]){
                DoStop(&program);
            }
        }
        e_last = true;
    }
    else{
        /*手动停止归原点操作*/
        if(e_edge == RISE){
            DoStop(&program);
            AxisSwitch(&program ,Hand_NULL_AXIS ,Hand_A_DRIECT);
            program.Mutex = Hand_NULL_MUTEX;
            spark_info->hand.Mutex = Hand_NULL_MUTEX;
        }
        e_last = false;
    }

    if(spark_info->b_array[B_HOME_Z_DOWN]){
        if(f_edge == FALL){
            program.Speed = 400;
            program.Mode = Hand_Velocity_MODE;
            program.Driect = Hand_B_DRIECT;
            DoMove(&program);
        }else{
            if(spark_info->b_array[B_Z_DOWN_ALERT]){
                DoStop(&program);
            }
        }
        f_last = true;
    }
    else{
        /*手动停止归原点操作*/
        if(f_edge == RISE){
            DoStop(&program);
            AxisSwitch(&program ,Hand_NULL_AXIS ,Hand_B_DRIECT);
            spark_info->hand.Mutex = Hand_NULL_MUTEX;
            program.Mutex = Hand_NULL_MUTEX;
        }
        f_last = false;
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

long ScanThread::Null_Count()
{
    qDebug()<<"Count :Null axis selected!";
    return 0;
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
