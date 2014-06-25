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
#define B_DEEPHOLE 10        /*深孔报警的开关索引*/
#define B_XYLOCK 11         /*XY轴锁定的开关索引*/
#define B_SLEEP 12          /*屏幕睡眠的开关索引*/
#define B_SCREEN 13         /*屏幕状态的索引*/
#define B_SHUTDOWN 14       /*关机的开关索引*/
#define B_SOUND 15          /*按键声音的开关索引*/
#define B_Z_UP 16           /*Z上限报警的开关索引*/
#define B_Z_DOWN 17         /*Z下限报警的开关索引*/

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
#define C_Z_OT1 0x14+Z_OFFSET       /*Z_OT1的值的索引*/
#define C_Z_OT2 0x15+Z_OFFSET       /*Z_OT2的值的索引*/
#define C_Z_CP0 0x20+Z_OFFSET       /*Z_CP0的值的索引*/
#define C_Z_CP1 0x21+Z_OFFSET       /*Z_CP1的值的索引*/
#define C_Z_CP2 0x22+Z_OFFSET       /*Z_CP2的值的索引*/
#define C_Z_CS0 0x2c+Z_OFFSET       /*Z_CS0的值的索引*/
#define C_Z_CS1 0x2d+Z_OFFSET       /*Z_CS1的值的索引*/

#define C_U_DVT 0x00+U_OFFSET          /*U_DVT的值的索引*/
#define C_U_IN0 0x10+U_OFFSET          /*U_IN0的值的索引*/
#define C_U_IN1 0x11+U_OFFSET          /*U_IN1的值的索引*/
#define C_U_OT0 0x13+U_OFFSET          /*U_OT0的值的索引*/
#define C_U_OT1 0x14+U_OFFSET          /*U_OT1的值的索引*/
#define C_U_OT2 0x15+U_OFFSET          /*U_OT2的值的索引*/

#define C_P_IO0 0x00+P_OFFSET            /*P_IO0的值的索引,xxx*****音量大小,***x****音量开关,****x***24V电压开关*/
#define C_P_IO1 0x01+P_OFFSET            /*P_IO1的值的索引,*x******总电源开关标志,**x*****放电极性标志,****x***油泵开关标志,*****x**变压器开关标志,******x*风扇开关标志,*******x副电源开关标志*/
#define C_P_IO2 0x02+P_OFFSET            /*P_IO2的值的索引,********放电电流值*/
#define C_P_IO3 0x03+P_OFFSET            /*P_IO3的值的索引,****xxxx高电压数值*/
#define C_P_IO4 0x04+P_OFFSET            /*P_IO4的值的索引*/
#define C_P_IO5 0x05+P_OFFSET            /*P_IO5的值的索引*/
#define C_P_IO6 0x06+P_OFFSET            /*P_IO6的值的索引*/

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
