#ifndef SETTING_H
#define SETTING_H

#define VERSION "V1.0"

#define ISA_FILE        "/dev/port"
#define GPMI_FILE       "/dev/mtd0"
#define BEEP_FILE       "/dev/beep"
#define GPMC_FILE       "/dev/fpga"
#define SPI_FILE        "/dev/spidev2.0"
#define BACKLIGHT_FILE  "/dev/backlight"

#define DEV_DIR         "/dev/"
#define TABLES_DIR      "/tables/"
#define DATABASE        "database.db"
#define TSLIB_CALIBFILE "/etc/pointercal"
#define USB_MOUSE       "/dev/mouse1"

/*FM25V02 ADDR: 0x000000 ~ 0x007FFF*/

#define CURRENT_AXIS_ADDR 0x0001e0   /*当前坐标索引的地址*/
#define CURRENT_TAB_ADDR  0x000200   /*当前数据表的索引的地址*/

#define CURRENT_TIME_ADDR 0x000300   /*当前放电时间的地址*/
#define TARGET_TIME_ADDR  0x000400   /*放电溢出时间的地址*/
#define USEDP_TIME_ADDR   0x000500   /*放电时间的和的地址*/
#define PASSWD_ADDR       0x000600   /*系统密码的地址*/
#define ID_ADDR           0x000700   /*机器ID的地址*/
#define X_ORIENT_ADDR     0x000800   /*X轴方向的地址*/
#define Y_ORIENT_ADDR     0x000802   /*Y轴方向的地址*/
#define Z_ORIENT_ADDR     0x000804   /*Z轴方向的地址*/
#define SCALE_ADDR        0x000806   /*光栅刻度值的地址*/

#define X_AXIS_ADDR       0x001000   /*坐标系0的X轴的偏移值的地址*/
#define Y_AXIS_ADDR       0x001008   /*坐标系0的Y轴的偏移值的地址*/
#define Z_AXIS_ADDR       0x001010   /*坐标系0的Z轴的偏移值的地址*/
#define X_ABSOLUTE_ADDR   0x001800   /*X轴光栅刻度值的地址*/
#define Y_ABSOLUTE_ADDR   0x001808   /*Y轴光栅刻度值的地址*/
#define Z_ABSOLUTE_ADDR   0x001810   /*Z轴光栅刻度值的地址*/

/*布尔型数据在铁电芯片中存储*/
#define CFALSE 0x00
#define CTRUE  0xff
#define SCALE_1 0x01
#define SCALE_5 0x05

typedef unsigned char      UNCHAR8;
typedef unsigned short     UNSHORT16;
typedef unsigned int       UNINT32;
typedef unsigned long      UNLONG32;
typedef unsigned long long UNLONG64;

typedef char      CHAR8;
typedef short     SHORT16;
typedef int       INT32;
typedef float     FLOAT32;
typedef long      LONG32;
typedef long long LONG64;

/*根据Qt编译器的版本判断目标机的类型*/
#ifdef Q_WS_X11
    #define X86
#else
    #define ARM
#endif

#endif // SETTING_H
