#include "qcomboxtime.h"
#include "setting.h"

QcomboxTime::QcomboxTime(QWidget *parent,QString l,bool b) :
    Qcombox(parent , l )
{
    flag = b;

    memset(current.bytes , 0 ,sizeof current);
    memset(target.bytes , 0 ,sizeof target);

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer ,SIGNAL(timeout()) ,this ,SLOT(valueUpdate()));

    initTime();
    timeToString();
}

void QcomboxTime::initTime()
{
    if(flag){
        time = QTime::currentTime();
        current.ushorts[HOURS] = (unsigned short)time.hour();
        current.ushorts[MINUTES] = (unsigned short)time.minute();
        current.ushorts[SECONDS] = (unsigned short)time.second();
    }
    else{
        current.ushorts[HOURS] = 0;
        current.ushorts[MINUTES] = 0;
        current.ushorts[SECONDS] = 0;
#ifdef ARM
        FM25V02_READ(CURRENT_TIME , current.bytes ,sizeof current);
        FM25V02_READ(TARGET_TIME , target.bytes ,sizeof target);
#endif
    }
}

void QcomboxTime::valueUpdate()
{
    if(flag){
        time = QTime::currentTime();
        current.ushorts[HOURS] = (unsigned short)time.hour();
        current.ushorts[MINUTES] = (unsigned short)time.minute();
        current.ushorts[SECONDS] = (unsigned short)time.second();
    }
    else{
        if(++current.ushorts[SECONDS] > 60){
            current.ushorts[SECONDS] = 0;
            if(++current.ushorts[MINUTES] > 60){
                current.ushorts[MINUTES] = 0;
                current.ushorts[HOURS]++;
                /*放电时间溢出*/
                if(current.ushorts[HOURS] > target.ushorts[HOURS]){
                    spark_info->reverseBool(B_START);
                }
            }
#ifdef ARM
        FM25V02_WRITE(CURRENT_TIME , current.bytes, sizeof current);
#endif
        }
    }
    timeToString();
    update();
}

void QcomboxTime::timeToString()
{
    QString str,tmp;
    tmp.setNum(current.ushorts[HOURS]);
    if(current.ushorts[HOURS] < 10){
        if(flag)
            str = '0';
    }
    else if(current.ushorts[HOURS] < 100&&current.ushorts[HOURS] >= 10){
    }
    str += tmp;
    str += ':';

    tmp.setNum(current.ushorts[MINUTES]);
    if(current.ushorts[MINUTES] < 10){
        str += '0';
    }
    str += tmp;
    str += ':';

    tmp.setNum(current.ushorts[SECONDS]);
    if(current.ushorts[SECONDS] < 10){
        str += '0';
    }
    str += tmp;
    value = str;
}

void QcomboxTime::updateTimer()
{
    if(flag){
        if(spark_info->b_array[B_TIME])
            timer->start();
        else
            timer->stop();
    }
    else{
        if(spark_info->b_array[B_START])
            timer->start();
        else
            timer->stop();
    }
}
