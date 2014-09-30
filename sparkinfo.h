#ifndef SPARKINFO_H
#define SPARKINFO_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include "fpga.h"
#include "fm25v02.h"
#include "setting.h"

#define _WRITE_BYTE_(a) FPGA_Write(a ,spark_info->c_array + a ,1)
#define _READ_BYTE_(a) FPGA_Read(a,spark_info->c_array + a ,1)

#define B_SLOT 0            /*布尔数组的槽标记*/
#define UINT_SLOT 1         /*无符号整型数组的槽标记*/
#define L_SLOT 2            /*长整型数组的槽标记*/
#define C_SLOT 3            /*字符型数组的槽标记*/

#define B_LENGTH 40         /*布尔数组的长度*/
#define UINT_LENGTH 15      /*无符号整型数组的长度*/
#define L_LENGTH 20         /*长整型数组的长度*/
#define C_LENGTH 255        /*字符型数组的长度*/

#define B_UPDATE 0          /*布尔数组有更新的开关索引*/
#define B_PUMP 1            /*冷却油泵的开关索引*/
#define B_POWER 2           /*副电源的开关索引*/
#define B_LOCK 3            /*机床锁定的开关索引*/
#define B_REVERSE 4         /*反加工的开关索引*/
#define B_START 5           /*放电加工的开关索引*/
#define B_TIME 6            /*当前时间的开关索引*/
#define B_BOTTOM 7          /*修底的开关索引*/
#define B_FIRE 8            /*火警的开关索引*/
#define B_OIL 9             /*油位报警的开关索引*/
#define B_DEEPHOLE 10       /*深孔报警的开关索引*/
#define B_XYLOCK 11         /*XY轴锁定的开关索引*/
#define B_SLEEP 12          /*屏幕睡眠的开关索引*/
#define B_SCREEN 13         /*屏幕状态的索引*/
#define B_SHUTDOWN 14       /*关机的开关索引*/
#define B_SOUND 15          /*按键声音的开关索引*/
#define B_Z_UP 16           /*Z上限报警的开关索引*/
#define B_Z_DOWN 17         /*Z下限报警的开关索引*/
#define B_FAN 18            /*风扇的开关索引*/
#define B_TRANS_A 19        /*大变压器的开关索引*/
#define B_OSCF 20           /*OSCF的开关索引*/
#define B_POWER_ALL 21      /*总电源的开关索引*/
#define B_SELECT 22         /*待加工行已选择开关的索引*/
#define B_REBOOT 23         /*系统重启的开关的索引*/
#define B_X_ORIENT 24       /*X轴的方向的索引*/
#define B_Y_ORIENT 25       /*Y轴的方向的索引*/
#define B_Z_ORIENT 26       /*Z轴的方向的索引*/
#define B_FIRE_ALERT 27     /*火警发生的索引*/
#define B_OIL_ALERT 28      /*油位低发生的索引*/
#define B_Z_UP_ALERT 29     /*Z轴上限位发生的索引*/
#define B_Z_DOWN_ALERT 30   /*Z轴下限位发生的索引*/
#define B_SHORT_ALERT 31    /*短路发生的索引*/
#define B_CABERN_ALERT 32   /*积碳发生的索引*/
#define B_Y_UP_ALERT 33     /*Y轴上限位发生的索引*/
#define B_Y_DOWN_ALERT 34   /*Y轴下限位发生的索引*/
#define B_ZERO 35           /*自动归零开关的索引*/

#define UINT_VOLTAGE 0      /*放电电压值的索引*/
#define UINT_CURRENT 1      /*放电电流值的索引*/
#define UINT_SPEED 2        /*加工速度值的索引*/
#define UINT_JUMP_H 3       /*跳高值的索引*/
#define UINT_JUMP_T 4       /*跳高次数的索引*/
#define UINT_TAB_INDEX 5    /*当前表的索引*/
#define UINT_COOR_INDEX 6   /*当前坐标的索引*/
#define UINT_CURRENT_ROM 7  /*当前行的索引*/
#define UINT_START_ROW 8    /*开始行的索引*/
#define UINT_END_ROW 9      /*结束行的索引*/
#define UINT_TAB_MAX 10     /*表格的最大索引*/
#define UINT_COOR_MAX 11    /*坐标的最大索引*/
#define UINT_SOUND 12       /*音量的索引*/
#define UINT_BRIGHTNESS 13  /*屏幕亮度的索引*/
#define UINT_SCALE 14       /*光栅刻度值的索引*/

#define L_DEEP_CURRENT 0    /*当前深度值的索引*/
#define L_DEEP_TARGET 1     /*目标深度值的索引*/
#define L_X_CURRENT 2       /*当前X轴位置的索引*/
#define L_Y_CURRENT 3       /*当前Y轴位置的索引*/
#define L_Z_CURRENT 4       /*当前Z轴位置的索引*/
#define L_X_OFFSET 5        /*当前X轴辅助位置的索引*/
#define L_Y_OFFSET 6        /*当前Y轴辅助位置的索引*/
#define L_Z_OFFSET 7        /*当前Z轴辅助位置的索引*/
#define L_X_ABSOLUTE 8      /*X轴绝对位置的索引*/
#define L_Y_ABSOLUTE 9      /*Y轴绝对位置的索引*/
#define L_Z_ABSOLUTE 10     /*Z轴绝对位置的索引*/
#define L_X_REMAIN 11       /*X轴剩余位置的索引*/
#define L_Y_REMAIN 12       /*Y轴剩余位置的索引*/
#define L_Z_REMAIN 13       /*Z轴剩余位置的索引*/
#define L_X_COUNTER 14      /*X轴光栅计数值的索引*/
#define L_Y_COUNTER 15      /*Y轴光栅计数值的索引*/
#define L_Z_COUNTER 16      /*Z轴光栅计数值的索引*/
#define L_X_VELOCITY 17     /*X轴速度值的索引*/
#define L_Y_VELOCITY 18     /*Y轴速度值的索引*/
#define L_Z_VELOCITY 19     /*Z轴速度值的索引*/

#define X_OFFSET 0x00       /*X轴地址偏移*/
#define Y_OFFSET 0x40       /*Y轴地址偏移*/
#define Z_OFFSET 0x80       /*Z轴地址偏移*/
#define U_OFFSET 0xC0       /*放电地址偏移*/
#define P_OFFSET 0xE0       /*IO地址偏移*/

#define C_X_IN0 0x10+X_OFFSET       /*X_IN0的值的索引*/
#define C_X_IN1 0x11+X_OFFSET       /*X_IN1的值的索引*/
#define C_X_OT0 0x13+X_OFFSET       /*X_OT0的值的索引*/
#define C_X_OT1 0x14+X_OFFSET       /*X_OT1的值的索引*/
#define C_X_OT2 0x15+X_OFFSET       /*X_OT2的值的索引*/
#define C_X_CP0 0x20+X_OFFSET       /*X_CP0的值的索引*/
#define C_X_CP1 0x21+X_OFFSET       /*X_CP1的值的索引*/
#define C_X_CP2 0x22+X_OFFSET       /*X_CP2的值的索引*/
#define C_X_CS0 0x2c+X_OFFSET       /*X_CS0的值的索引*/
#define C_X_CS1 0x2d+X_OFFSET       /*X_CS1的值的索引*/

#define C_Y_IN0 0x10+Y_OFFSET       /*Y_IN0的值的索引*/
#define C_Y_IN1 0x11+Y_OFFSET       /*Y_IN1的值的索引*/
#define C_Y_OT0 0x13+Y_OFFSET       /*Y_OT00的值的索引*/
#define C_Y_OT1 0x14+Y_OFFSET       /*Y_OT1的值的索引*/
#define C_Y_OT2 0x15+Y_OFFSET       /*Y_OT2的值的索引*/
#define C_Y_CP0 0x20+Y_OFFSET       /*Y_CP0的值的索引*/
#define C_Y_CP1 0x21+Y_OFFSET       /*Y_CP1的值的索引*/
#define C_Y_CP2 0x22+Y_OFFSET       /*Y_CP2的值的索引*/
#define C_Y_CS0 0x2c+Y_OFFSET       /*Y_CS0的值的索引*/
#define C_Y_CS1 0x2d+Y_OFFSET       /*Y_CS1的值的索引*/

#define C_Z_IN0 0x10+Z_OFFSET       /*Z_IN0的值的索引*/
#define C_Z_IN1 0x11+Z_OFFSET       /*Z_IN1的值的索引*/
#define C_Z_OT0 0x13+Z_OFFSET       /*Z_OT0的值的索引,**x*****Z轴ENZ,***x****sl3,*****xxxZ轴间隙*/
#define C_Z_OT1 0x14+Z_OFFSET       /*Z_OT1的值的索引，Z轴的控制模式*/
#define C_Z_OT2 0x15+Z_OFFSET       /*Z_OT2的值的索引*/
#define C_Z_STT 0x16+Z_OFFSET       /*Z_STT的值的索引*/
#define C_Z_STP 0x19+Z_OFFSET       /*Z_STP的值的索引*/
#define C_Z_STI 0x1a+Z_OFFSET       /*Z_STI的值的索引*/
#define C_Z_STD 0x1b+Z_OFFSET       /*Z_STD的值的索引*/
#define C_Z_CP0 0x20+Z_OFFSET       /*Z_CP0的值的索引*/
#define C_Z_CP1 0x21+Z_OFFSET       /*Z_CP1的值的索引*/
#define C_Z_CP2 0x22+Z_OFFSET       /*Z_CP2的值的索引*/
#define C_Z_OP0 0x26+Z_OFFSET       /*Z_OP0的值的索引*/
#define C_Z_OP1 0x27+Z_OFFSET       /*Z_OP1的值的索引*/
#define C_Z_OP2 0x28+Z_OFFSET       /*Z_OP2的值的索引*/
#define C_Z_CS0 0x2c+Z_OFFSET       /*Z_CS0的值的索引*/
#define C_Z_CS1 0x2d+Z_OFFSET       /*Z_CS1的值的索引*/
#define C_Z_TP0 0x30+Z_OFFSET       /*Z_TP0的值的索引*/
#define C_Z_TP1 0x31+Z_OFFSET       /*Z_TP1的值的索引*/
#define C_Z_TP2 0x32+Z_OFFSET       /*Z_TP2的值的索引*/
#define C_Z_TS0 0x3c+Z_OFFSET       /*Z_TS0的值的索引*/
#define C_Z_TS1 0x3d+Z_OFFSET       /*Z_TS1的值的索引*/

#define C_U_DVT 0x00+U_OFFSET       /*U_DVT的值的索引*/
#define C_U_GVT 0x02+U_OFFSET       /*U_GVT的值的索引*/
#define C_U_PW0 0x04+U_OFFSET       /*U_PW0的值的索引*/
#define C_U_PW1 0x05+U_OFFSET       /*U_PW1的值的索引*/
#define C_U_PG0 0x08+U_OFFSET       /*U_PG0的值的索引*/
#define C_U_PG1 0x09+U_OFFSET       /*U_PG1的值的索引*/
#define C_U_IN0 0x10+U_OFFSET       /*U_IN0的值的索引*/
#define C_U_IN1 0x11+U_OFFSET       /*U_IN1的值的索引*/
#define C_U_OT0 0x13+U_OFFSET       /*U_OT0的值的索引*/
#define C_U_OT1 0x14+U_OFFSET       /*U_OT1的值的索引*/
#define C_U_OT2 0x15+U_OFFSET       /*U_OT2的值的索引*/

#define C_P_IO0 0x00+P_OFFSET       /*P_IO0的值的索引,xxx*****音量大小,***x****音量开关,****x***24V电压开关*/
#define C_P_IO1 0x01+P_OFFSET       /*P_IO1的值的索引,*x******总电源开关标志,**x*****放电极性标志,****x***油泵开关标志,*****x**变压器开关标志,******x*风扇开关标志,*******x副电源开关标志*/
#define C_P_IO2 0x02+P_OFFSET       /*P_IO2的值的索引,********放电电流值*/
#define C_P_IO3 0x03+P_OFFSET       /*P_IO3的值的索引,****xxxx高电压数值*/
#define C_P_IO4 0x04+P_OFFSET       /*P_IO4的值的索引*/
#define C_P_IO5 0x05+P_OFFSET       /*P_IO5的值的索引*/
#define C_P_IO6 0x06+P_OFFSET       /*P_IO6的值的索引*/

struct Table
{
    LONG64  Shendu[10];   /*深度*/
    UNINT32 Dianliu[10];  /*电流*/
    UNINT32 Maikuan[10];  /*脉宽*/
    UNINT32 Xiuzhi[10];   /*休止*/
    UNCHAR8 Jianxi[10];   /*间隙*/
    UNCHAR8 Sudu[10];     /*速度*/
    UNCHAR8 Shenggao[10]; /*升高*/
    UNCHAR8 Gongshi[10];  /*工时*/
    UNCHAR8 Mianji[10];   /*面积*/
    UNCHAR8 Jixing[10];   /*极性*/
    UNCHAR8 Gaoya[10];    /*高压*/
    UNCHAR8 Index[10];    /*索引*/
};

struct SearchTable
{
    UNCHAR8 Index[4];   /*索引*/
    LONG64  Shendu[10];   /*深度*/
    FLOAT32 Dianliu[10];  /*电流*/
    UNINT32 Maikuan[10];  /*脉宽*/
    UNINT32 Xiuzhi[10];   /*休止*/
    UNCHAR8 Jianxi[10];   /*间隙*/
    UNCHAR8 Gongshi[10];  /*工时*/
};

union TwoBytes
{
    CHAR8 bytes[sizeof(unsigned short)];
    UNSHORT16 ushort;
};

union FourBytes
{
    CHAR8 bytes[sizeof(unsigned int)];
    UNINT32 uint;
};

union SixBytes
{
    CHAR8 bytes[3*sizeof(unsigned short)];
    UNSHORT16 ushorts[3];
};

union EightBytes
{
    CHAR8 bytes[sizeof(long)];
    LONG64 longs;
};

const Table table_init = {
    {-5,5,10,15,20,30,40,50,55,60},
    {0,35*2,6,0,4,0,0,0,0,0},
    {1,999,200,0,20,0,0,0,0,},
    {3,999,100,0,30,0,0,0,0,},
    {30,120,55,0,50,0,0,0,0,},
    {1,8,3,0,5,0,0,0,0,},
    {0,99,6,0,0,0,0,0,0,0},
    {1,99,5,0,99,0,0,0,0,0},
    {1,99,1,0,1,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0,0},
    {0,3,1,0,2,0,0,0,0,0},
    {1,2,3,4,5,6,7,8,9,10},
};

const SearchTable search[12]=
{
    {
        {4,1,2,3},
        {60,35,15,0},
        {1,1,0.5,0},
        {50,20,10,5},
        {40,30,20,15},
        {55,60,65,70},
        {4,3,3,2}
    },/*1*/
    {
        {5,2,3,4},
        {90,60,35,15,0},
        {1,0.9,0.5,0.5,0.3},
        {100,50,20,10,5},
        {60, 40,30,20,15},
        {50,55,60,65,70},
        {4,4,3,3,2}
    },/*2*/
    {
        {5,2,3,4},
        {90,60,35,15,0},
        {1,0.8,0.6,0.4,0.3},
        {100,50,20,10,5},
        {60, 40,30,20,15},
        {50,55,60,65,70},
        {5,4,4,3,3}
    },/*3*/
    {
        {6,2,3,5},
        {130,90,60,35,15,0},
        {1,0.9,0.7,0.4,0.2,0.1},
        {150,100,50,20,10,5},
        { 80, 60,40,30,20,15},
        {50,50,55,60,65,70},
        {5,4,4,3,3,2}
    },/*5*/
    {
        {7,3,5,7},
        {200,150,110,75,45,20,0},
        {1,0.9,0.8,0.6,0.4,0.3,0.2},
        {200,150,100, 50, 25, 10, 5},
        { 80, 60, 60, 50, 30, 20,15},
        {50,50,55,55,60,65,70},
        {10,10,8,8,6,6,5}
    },/*7*/
    {
        {7,3,5,7},
        {270,200,140,90,50,20,0},
        {1,0.8,0.6,0.5,0.4,0.3,0.2},
        {250,200,150,100, 50, 25, 10},
        { 80, 80, 60, 60, 50, 30, 20},
        {50,50,55,55,60,65,70},
        {12,10,10,8,8,6,6}
    },/*9*/
    {
        {8,3,5,7},
        {350,270,200,140,90,50,20,0},
        {1,0.9,0.8,0.7,0.6,0.5,0.4,0.3},
        {350,300,250,200,150,100, 50,25},
        {100, 80, 80, 80, 60, 60, 50,30},
        {50,50,50,55,55,60,65,70},
        {15,15,12,12,10,10,8,8}
    },/*12*/
    {
        {8,3,5,7},
        {420,330,250,180,120,70,30,0},
        {1,0.9,0.8,0.7,0.6,0.5,0.4,0.3},
        {400,350,300,250,200,150,100, 50},
        {100,100, 80, 80, 80, 60, 60, 50},
        {50,50,50,55,55,60,65,70},
        {15,15,12,12,10,10,8,8}
    },/*17*/
     {
        {8,3,5,7},
        {490,390,300,220,150,90,40,0},
        {1,0.9,0.8,0.7,0.6,0.5,0.4,0.3},
        {450,400,350,300,250,200,150,100},
        {100,100, 80, 80, 80, 60, 60, 50},
        {55,50,50,55,55,60,65,70},
        {15,15,12,12,10,10,8,8}
    },/*25*/
    {
        {9,3,5,7},
        {680,560,450,350,260,180,110,50,0},
        {1,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2},
        {500,450,400,350,300,250,200,150,100},
        {100,100,100, 80, 80, 80, 60, 60, 50},
        {50,50,50,55,55,55,60,65,70},
        {20,15,15,12,12,10,10,8,8}
    },/*35*/
    {
        {10,3,5,7},
        {900,760,630,510,400,300,210,130,60,0},
        {1,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1},
        {600,500,450,400,350,300,250,200,150, 100},
        {100,100,100,100, 80, 80, 80, 60, 60, 50},
        {50,50,50,50,55,55,55,60,65,70},
        {20,20,15,15,12,12,10,10,8,8}
    },/*50*/
    {
        {10,3,5,7},
        {990,840,700,570,450,340,240,150,70,0},
        {1,0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1},
        {700,600,500,450,400,350,300,250,200, 150},
        {100,100,100,100, 80, 80, 80, 60, 60, 50},
        {50,50,50,50,55,55,55,60,65,70},
        {20,20,15,15,12,12,10,10,8,8}
    }/*75*/

};

const bool bool_init[] = {
        false ,false ,false ,false ,false ,
        false ,true ,false ,false ,false ,
        false ,false ,false ,false ,false ,
        false ,false ,false ,false ,false ,
        false ,false ,false ,false ,false ,
        false ,false ,false ,false ,false ,
        false ,false ,false ,false ,false ,
        false ,false ,false ,false ,false ,
};

const UNINT32 uint_init[] = {
        0, 0, 10, 0, 5,
        0, 0, 10, 10, 10,
        99, 9, 0, 0, 5,
};

const LONG64 long_init[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
};

class SparkInfo : public QObject
{
    Q_OBJECT
public:
    explicit SparkInfo(QObject *parent = 0);
    void tableClear();
    void tableSave();
    void tableAuto(LONG64 ,UNINT32 ,UNINT32 ,UNINT32);
    bool b_array[B_LENGTH];
    unsigned int uint_array[UINT_LENGTH];
    long l_array[L_LENGTH];
    char c_array[C_LENGTH];
    /*放电时间溢出值*/
    SixBytes target;
    /*已经放电时间值*/
    SixBytes useup;
    Table table;

private:
    void tableInit();
    void carryInit();
    void fm25v02Init();

signals:
    /*放电开关信号*/
    void startChange();
    void boolChange();
    void longChange();
    void uintChange();
    /*刷新XYZ显示数值*/
    void xyzChange(int);
    void tableChange();
    void tableIndexChange();
    void tableRowChange();
    void coorIndexChange();

public slots:
    void setBool(UNINT32 ,bool);
    void reverseBool(UNINT32);
    void setLong(UNINT32 ,LONG64);
    void setUInt(UNINT32 , UNINT32);
    void tableLoad();
    /*检查放电时间是否溢出的槽*/
    bool checkTime();
};

extern SparkInfo *spark_info;

#endif // SPARKINFO_H
