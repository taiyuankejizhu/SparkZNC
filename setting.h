#ifndef SETTING_H
#define SETTING_H

#define VERSION "V1.0"

#define ISA_FILE "/dev/port"
#define GPMI_FILE "/dev/mtd0"
#define BEEP_FILE "/dev/beep"
#define SPI_FILE  "/dev/spidev1.0"

#define SCALE 1     /*光栅尺的刻度，1微米，5微米*/

/*FM25V02 ADDR: 0x000000 ~ 0x007FFF*/
#define X_AXIS_ADDR 0x000000
#define Y_AXIS_ADDR 0x000100
#define Z_AXIS_ADDR 0x000200

#define CURRENT_TIME 0x000300 /*当前放电时间的地址*/
#define TARGET_TIME  0x000400 /*放电溢出时间的地址*/

/*根据Qt编译器的版本判断目标机的类型*/

#if(QT_VERSION == 0x040806)
    #define X86
#else
    #define ARM
#endif

#endif // SETTING_H
