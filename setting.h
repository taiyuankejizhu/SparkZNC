#ifndef SETTING_H
#define SETTING_H

#define VERSION "V1.0"

#define ISA_FILE "/dev/port"
#define GPMI_FILE "/dev/mtd0"
#define BEEP_FILE "/dev/beep"
#define GPMC_FILE "/dev/fpga"
#define SPI_FILE  "/dev/spidev2.0"

#define TABLES_DIR "/tables/"
#define TSLIB_CALIBFILE "/etc/pointercal"

#define SCALE 1     /*光栅尺的刻度，1微米，5微米*/

/*FM25V02 ADDR: 0x000000 ~ 0x007FFF*/
#define X_AXIS_ADDR 0x000000
#define Y_AXIS_ADDR 0x000008
#define Z_AXIS_ADDR 0x000010

#define CURRENT_AXIS_ADDR 0x0001e0   /*当前坐标索引*/
#define CURRENT_TAB_ADDR  0x000200   /*当前数据表的索引*/

#define CURRENT_TIME_ADDR 0x000300 /*当前放电时间的地址*/
#define TARGET_TIME_ADDR  0x000400 /*放电溢出时间的地址*/
#define USEDP_TIME_ADDR   0x000500 /*放电时间的和的地址*/
#define PASSWD_ADDR       0x000600 /*系统密码的地址*/

/*根据Qt编译器的版本判断目标机的类型*/
#if(QT_VERSION == 0x040801)
    #define X86
#else
    #define ARM
#endif

#endif // SETTING_H
