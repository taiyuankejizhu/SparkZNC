#ifndef SPARKINFO_H
#define SPARKINFO_H

#include <QObject>

#define B_SLOT 0            /*布尔数组的槽标记*/
#define UINT_SLOT 1         /*无符号整型数组的槽标记*/
#define L_SLOT 2            /*长整型数组的槽标记*/
#define C_SLOT 3            /*字符型数组的槽标记*/

#define B_LENGTH 20         /*布尔数组的长度*/
#define UINT_LENGTH 10      /*无符号整型数组的长度*/
#define L_LENGTH 20         /*长整型数组的长度*/
#define C_LENGTH 20         /*字符型数组的长度*/

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
#define B_DEEPHOLE 10        /*深孔报警的开关索引*/
#define B_XYLOCK 11         /*XY轴锁定的开关索引*/
#define B_SLEEP 12          /*屏幕睡眠的开关索引*/
#define B_SCREEN 13         /*屏幕状态的索引*/
#define B_SHUTDOWN 14       /*关机的开关索引*/
#define B_SOUND 15          /*按键声音的开关索引*/

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

#define C_IO_0 0            /*IO_0的值的索引*/
#define C_IO_1 1            /*IO_1的值的索引*/
#define C_IO_2 2            /*IO_2的值的索引*/
#define C_IO_3 3            /*IO_3的值的索引*/
#define C_IO_4 4            /*IO_4的值的索引*/

#define C_OTPS_0 5          /*OTPS_0的值的索引*/
#define C_OTPS_1 6          /*OTPS_1的值的索引*/
#define C_IOPS_0 7          /*IOPS_0的值的索引*/

#define C_OTPX_0 8          /*OTPX_0的值的索引*/
#define C_OTPX_1 9          /*OTPX_1的值的索引*/
#define C_IOPX_0 10         /*IOPX_0的值的索引*/

#define C_OTPY_0 11          /*OTPY_0的值的索引*/
#define C_OTPY_1 12          /*OTPY_1的值的索引*/
#define C_IOPY_0 13         /*IOPY_0的值的索引*/

#define C_OTPZ_0 14          /*OTPZ_0的值的索引*/
#define C_OTPZ_1 15          /*OTPZ_1的值的索引*/
#define C_IOPZ_0 16         /*IOPZ_0的值的索引*/

#define C_IOX_0 17          /*IOX_0的值的索引*/
#define C_IOY_0 18          /*IOY_0的值的索引*/
#define C_IOZ_0 19          /*IOZ_0的值的索引*/

struct Table
{
    int           Shendu[10];   /*深度*/
    unsigned int  Dianliu[10];  /*电流*/
    unsigned int  Maikuan[10];  /*脉宽*/
    unsigned int  Xiuzhi[10];   /*休止*/
    unsigned char Jianxi[10];   /*间隙*/
    unsigned char Sudu[10];     /*速度*/
    unsigned char Shenggao[10]; /*升高*/
    unsigned char Gongshi[10];  /*工时*/
    unsigned char Mianji[10];   /*面积*/
    unsigned char Jixin[10];    /*极性*/
    unsigned char Gaoya[10];    /*高压*/
    unsigned char Index[10];    /*索引*/
};

union TwoBytes
{
    char bytes[sizeof(unsigned short)];
    unsigned short ushort;
};

union FourBytes
{
    char bytes[sizeof(unsigned int)];
    unsigned int uint;
};

union SixBytes
{
    char bytes[3*sizeof(unsigned short)];
    unsigned short ushorts[3];
};

union EightBytes
{
    char bytes[sizeof(long)];
    long longs;
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

const bool bool_init[] = {
        false ,false ,false ,false ,false ,
        false ,true ,false ,false ,false ,
        false ,false ,false ,false ,false ,
        false ,false ,false ,false ,false ,
};

const unsigned int uint_init[] = {
        0, 0, 10, 0, 5,
        0, 0, 10, 10, 10,
};

const long long_init[] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
};

const char char_init[] = {
        0x10 ,0x80 ,0x00 ,0x00 ,0x00 ,
        0x00 ,0x0b ,0x00 ,0x00 ,0x00 ,
        0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,
        0x00 ,0x00 ,0xe0 ,0x00 ,0x00 ,
};

class SparkInfo : public QObject
{
    Q_OBJECT
public:
    explicit SparkInfo(QObject *parent = 0);
    void tableClear();
    void tableAuto(long ,unsigned int ,unsigned int ,unsigned int);
    bool b_array[B_LENGTH];
    unsigned int uint_array[UINT_LENGTH];
    long l_array[L_LENGTH];
    char c_array[C_LENGTH];
    Table table;

private:
    void tableInit();

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

public slots:
    void setBool(unsigned int ,bool);
    void reverseBool(unsigned int);
    void setLong(unsigned int ,long);
    void setUInt(unsigned int , unsigned int);
    void updateTable();
};

extern SparkInfo *spark_info;

#endif // SPARKINFO_H
