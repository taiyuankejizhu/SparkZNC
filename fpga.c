#include "fpga.h"
#include "setting.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "sys/ioctl.h"
#include "mtd/mtd-user.h"

/*根据Qt编译器的版本判断目标机的类型*/
#define ISA
//#define GMPI

/*存储数据接口的文件描述符*/
static int fpga_fd;

/*
********************************************************************************************************
** Function name:		GPMI_Init
** Descriptions:		初始化GPMI接口
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
uint8 GPMI_Init(void)
{

    fpga_fd = open(GPMI_FILE, O_RDWR);

    if(fpga_fd < 0){
        printf("Can't open GPMI device!\n");
        return 0;
    }
    else{
        printf("GPMI device is openned!\n");
        return 1;
    }
}

/*
********************************************************************************************************
** Function name:		ISA_Init
** Descriptions:		初始化ISA接口
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
uint8 ISA_Init(void)
{

    fpga_fd = open(ISA_FILE, O_RDWR);

    if(fpga_fd < 0){
        printf("Can't open ISA device!\n");
        return 0;
    }
    else{
        printf("ISA device is openned!\n");
        return 1;
    }
}


/*
********************************************************************************************************
** Function name:		GPMI_Info
** Descriptions:		GPMI读取Flash信息
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void GPMI_Info(void)
{
    struct mtd_info_user info;

    if(fpga_fd < 0)
        return;

    if(ioctl(fpga_fd, MEMGETINFO, &info)){
        printf("Can't get the info data!\n");
        return;
    }
    else{

        switch(info.type){
        case MTD_ABSENT:
            printf("type:MTD_ABSENT\n");
            break;
        case MTD_RAM:
            printf("type:MTD_RAM\n");
            break;
        case MTD_ROM:
            printf("type:MTD_ROM\n");
            break;
        case MTD_NORFLASH:
            printf("type:MTD_NORFLASH\n");
            break;
        case MTD_NANDFLASH:
            printf("type:MTD_NANDFLASH\n");
            break;
        case MTD_DATAFLASH:
            printf("type:MTD_DATAFLASH\n");
            break;
        case MTD_UBIVOLUME:
            printf("type:MTD_UBIVOLUME\n");
            break;
        default:
            printf("type:none\n");
            break;
        }

        switch(info.flags){
        case MTD_WRITEABLE:
            printf("flags:MTD_WRITEABLE\n");
            break;
        case MTD_BIT_WRITEABLE:
            printf("flags:MTD_BIT_WRITEABLE\n");
            break;
        case MTD_NO_ERASE:
            printf("flags:MTD_NO_ERASE\n");
            break;
        case MTD_POWERUP_LOCK:
            printf("flags:MTD_POWERUP_LOCK\n");
            break;
        case MTD_CAP_ROM:
            printf("flags:MTD_CAP_ROM\n");
            break;
        case MTD_CAP_RAM:
            printf("flags:MTD_CAP_RAM\n");
            break;
        case MTD_CAP_NORFLASH:
            printf("flags:MTD_CAP_NORFLASH\n");
            break;
        default:
            printf("flags:none\n");
            break;
        }

        printf("size:%d\n" ,info.size);
        printf("erasesize:%d\n" ,info.erasesize);
        printf("writesize:%d\n" ,info.writesize);
        printf("oobsize:%d\n" ,info.oobsize);
        printf("eccsize:%d\n" ,info.eccsize);
        printf("ecctype:%d\n" ,info.ecctype);
    }
}

/*
********************************************************************************************************
** Function name:		ISA_Info
** Descriptions:		ISA读取信息
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void ISA_Info(void)
{
    printf("ISR Info!");
}

/*
********************************************************************************************************
** Function name:		GPMI_Read
** Descriptions:		GPMI读取数据
** input parameters:    pBuf:   读出数据的指针
**                      num:    要读数据的长度
**                      addr:   要读数据的地址相对于文件开始位置
** output parameters:   无
** Returned value:      uint32  读出的数据长度
********************************************************************************************************
*/
uint32 GPMI_Read(uint32 addr ,uint8 *pBuf ,uint32 num)
{
    struct mtd_info_user info;
    uint32 n;
    addr += GPMI_BASE;

    if(fpga_fd < 0)
        return 0;

    if(ioctl(fpga_fd, MEMGETINFO, &info)){
        printf("Can't get the info data!\n");
        return 0;
    }
    else{
        /*check the write length*/
        if(num + addr > info.writesize)
            num = info.writesize - addr;

        lseek(fpga_fd, addr, SEEK_SET);

        n = read(fpga_fd ,pBuf ,num);
        return n;
    }
}

/*
********************************************************************************************************
** Function name:		ISA_Read
** Descriptions:		ISA读取数据
** input parameters:    pBuf:   读出数据的指针
**                      num:    要读数据的长度
**                      addr:   要读数据的地址相对于文件开始位置
** output parameters:   无
** Returned value:      uint32  读出的数据长度
********************************************************************************************************
*/
uint32 ISA_Read(uint32 addr ,uint8 *pBuf ,uint32 num)
{
    uint32 n = 0;
    addr += ISA_BASE;

    if(fpga_fd < 0)
        return 0;

    lseek(fpga_fd, addr, SEEK_SET);
    n = read(fpga_fd ,pBuf ,num);

    return n;
}

/*
********************************************************************************************************
** Function name:		GPMI_Write
** Descriptions:		GPMI读取数据
** input parameters:    pBuf:   写入数据的指针
**                      num:    要写数据的长度
**                      addr:   写入数据的地址相对于文件开始位置
** output parameters:   无
** Returned value:      uint32  写入的数据长度
********************************************************************************************************
*/
uint32 GPMI_Write(uint32 addr ,uint8 *pBuf ,uint32 num)
{
    struct mtd_info_user info;
//    struct erase_info_user e_info;
    uint32 n;
    addr += GPMI_BASE;

    if(fpga_fd < 0)
        return 0;

    if(ioctl(fpga_fd, MEMGETINFO, &info)){
        printf("Can't get the info data!\n");
        return 0;
    }
    else{
//        e_info.start = PAGE_1;
//        e_info.length = info.erasesize;

        /*erase the page_1,if the device is not a nand flash device ,
        this step should be ignore*/
/*
        if (ioctl(fpga_fd,MEMERASE,&e_info) < 0) {
            printf("Erasing page_1 failed!\n");
            return 0;
        }
*/

        /*check the write length*/
        if(num + addr > info.writesize)
            num = info.writesize - addr;

        lseek(fpga_fd, addr, SEEK_SET);

        /*write the data to page_1*/
        n = write(fpga_fd, pBuf, num);
        if (n < num) {
            printf("Error writing image.\n");
            return 0;
        }

        return n;
    }
}

/*
********************************************************************************************************
** Function name:		ISA_Write
** Descriptions:		ISA读取数据
** input parameters:    pBuf:   写入数据的指针
**                      num:    要写数据的长度
**                      addr:   写入数据的地址相对于文件开始位置
** output parameters:   无
** Returned value:      uint32  写入的数据长度
********************************************************************************************************
*/
uint32 ISA_Write(uint32 addr ,uint8 *pBuf ,uint32 num)
{
    uint32 n = 0;
    addr += ISA_BASE;

    if(fpga_fd < 0)
        return 0;

    lseek(fpga_fd, addr, SEEK_SET);
    n = write(fpga_fd ,pBuf ,num);

    return n;
}

/*
********************************************************************************************************
** Function name:		FPGA_Init
** Descriptions:		初始化FPGA接口
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
uint8 FPGA_Init(void)
{
    uint8 ret = 0;
#ifdef GPMI
    ret = GPMI_Init();
#endif

#ifdef ISA
    ret = ISA_Init();
#endif
    return ret;
}

/*
********************************************************************************************************
** Function name:		FPGA_Info
** Descriptions:		读取FPGA信息
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void FPGA_Info(void)
{
#ifdef GPMI
    GPMI_Info();
#endif
#ifdef ISA
    ISA_Info();
#endif
}

/*
********************************************************************************************************
** Function name:		FPGA_Read
** Descriptions:		FPGA读取数据
** input parameters:    pBuf:   读出数据的指针
**                      num:    要读数据的长度
**                      addr:   要读数据的地址相对于文件开始位置
** output parameters:   无
** Returned value:      uint32  读出的数据长度
********************************************************************************************************
*/
uint32 FPGA_Read(uint32 addr ,uint8 *pBuf ,uint32 num)
{
    uint32 ret = 0;
#ifdef GPMI
    ret = GPMI_Read(addr ,pBuf ,num);
#endif
#ifdef ISA
    ret = ISA_Read(addr ,pBuf ,num);
#endif
    return ret;
}

/*
********************************************************************************************************
** Function name:		FPGA_Write
** Descriptions:		FPGA读取数据
** input parameters:    pBuf:   写入数据的指针
**                      num:    要写数据的长度
**                      addr:   写入数据的地址相对于文件开始位置
** output parameters:   无
** Returned value:      uint32  写入的数据长度
********************************************************************************************************
*/
uint32 FPGA_Write(uint32 addr ,uint8 *pBuf ,uint32 num)
{
    uint32 ret = 0;
#ifdef GPMI
    ret = GPMI_Write(addr ,pBuf ,num);
#endif
#ifdef ISA
    ret = ISA_Write(addr ,pBuf ,num);
#endif
    return ret;
}

/*
********************************************************************************************************
** Function name:		Voltage_Read
** Descriptions:		FPGA读取电压
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  电压值
********************************************************************************************************
*/
uint8 Voltage_Read(void)
{
    uint8 ret = 0;
    uint8 out[2];
    uint8 in[2];
    memset(out ,0x02 ,sizeof out);
    memset(in ,0x00 ,sizeof in);

    FPGA_Write(OSC_OFFSET ,out ,1);
    FPGA_Read(OSC_OFFSET+1 ,in ,1);

    ret = in[0];
    return ret;
}

/*
********************************************************************************************************
** Function name:		X_Count
** Descriptions:		FPGA读取X轴计数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  X轴计数值
********************************************************************************************************
*/
uint64 X_Count(void)
{
    uint64 ret = 0;
    uint8 out[2];
    uint8 in[4];
    memset(out ,0x08 ,sizeof out);
    memset(in ,0x00 ,sizeof in);

    FPGA_Write(X_OFFSET ,out ,1);
    FPGA_Read(X_OFFSET+1 ,in ,3);

    if(in[2] & 0x80)
        in[3] = 0xff;
    else
        in[3] = 0x00;
    ret = in[3];
    ret <<= 8;
    ret += in[2];
    ret <<= 8;
    ret += in[1];
    ret <<= 8;
    ret += in[0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

/*
********************************************************************************************************
** Function name:		Y_Count
** Descriptions:		FPGA读取Y轴计数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  Y轴计数值
********************************************************************************************************
*/
uint64 Y_Count(void)
{
    uint64 ret = 0;
    uint8 out[2];
    uint8 in[4];
    memset(out ,0x08 ,sizeof out);
    memset(in ,0x00 ,sizeof in);

    FPGA_Write(Y_OFFSET ,out ,1);
    FPGA_Read(Y_OFFSET+1 ,in ,3);

    if(in[2] & 0x80)
        in[3] = 0xff;
    else
        in[3] = 0x00;
    ret = in[3];
    ret <<= 8;
    ret += in[2];
    ret <<= 8;
    ret += in[1];
    ret <<= 8;
    ret += in[0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

/*
********************************************************************************************************
** Function name:		Z_Count
** Descriptions:		FPGA读取Z轴计数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  Z轴计数值
********************************************************************************************************
*/
uint64 Z_Count(void)
{
    uint64 ret = 0;
    uint8 out[2];
    uint8 in[4];
    memset(out ,0x08 ,sizeof out);
    memset(in ,0x00 ,sizeof in);

    FPGA_Write(Z_OFFSET ,out ,1);
    FPGA_Read(Z_OFFSET+1 ,in ,3);

    if(in[2] & 0x80)
        in[3] = 0xff;
    else
        in[3] = 0x00;
    ret = in[3];
    ret <<= 8;
    ret += in[2];
    ret <<= 8;
    ret += in[1];
    ret <<= 8;
    ret += in[0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

/*
********************************************************************************************************
** Function name:		X_Velocity
** Descriptions:		FPGA读取X速度数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  X轴速度值
********************************************************************************************************
*/
uint64 X_Velocity(void)
{
    uint64 ret = 0;
    uint8 out[2];
    uint8 in[4];
    memset(out ,0x40 ,sizeof out);
    memset(in ,0x00 ,sizeof in);

    FPGA_Write(X_OFFSET ,out ,1);
    FPGA_Read(X_OFFSET+1 ,in ,2);

    if(in[1] & 0x80){
        in[2] = 0xff;
        in[3] = 0xff;
    }
    else{
        in[2] = 0x00;
        in[3] = 0x00;
    }
    ret = in[3];
    ret <<= 8;
    ret += in[2];
    ret <<= 8;
    ret += in[1];
    ret <<= 8;
    ret += in[0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

/*
********************************************************************************************************
** Function name:		Y_Velocity
** Descriptions:		FPGA读取Y速度数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  Y轴速度值
********************************************************************************************************
*/
uint64 Y_Velocity(void)
{
    uint64 ret = 0;
    uint8 out[2];
    uint8 in[4];
    memset(out ,0x40 ,sizeof out);
    memset(in ,0x00 ,sizeof in);

    FPGA_Write(Y_OFFSET ,out ,1);
    FPGA_Read(Y_OFFSET+1 ,in ,2);

    if(in[1] & 0x80){
        in[2] = 0xff;
        in[3] = 0xff;
    }
    else{
        in[2] = 0x00;
        in[3] = 0x00;
    }
    ret = in[3];
    ret <<= 8;
    ret += in[2];
    ret <<= 8;
    ret += in[1];
    ret <<= 8;
    ret += in[0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

/*
********************************************************************************************************
** Function name:		Z_Velocity
** Descriptions:		FPGA读取Z速度数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  Z轴速度值
********************************************************************************************************
*/
uint64 Z_Velocity(void)
{
    uint64 ret = 0;
    uint8 out[2];
    uint8 in[4];
    memset(out ,0x40 ,sizeof out);
    memset(in ,0x00 ,sizeof in);

    FPGA_Write(Z_OFFSET ,out ,1);
    FPGA_Read(Z_OFFSET+1 ,in ,2);

    if(in[1] & 0x80){
        in[2] = 0xff;
        in[3] = 0xff;
    }
    else{
        in[2] = 0x00;
        in[3] = 0x00;
    }
    ret = in[3];
    ret <<= 8;
    ret += in[2];
    ret <<= 8;
    ret += in[1];
    ret <<= 8;
    ret += in[0];

    if(ret > 9999999)
        ret = 9999999;
    if(ret < -9999999)
        ret = -9999999;

    return ret;
}

/*
********************************************************************************************************
** Function name:		PID_Tune
** Descriptions:		调整PID参数
** input parameters:
**                      p:比例系数
**                      i:积分系数
**                      d:微分系数
**                      s:未知
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void PID_Tune(uint8 p,uint8 i,uint8 d,uint8 s)
{
    uint8 out[2];

    memset(out ,0x02 ,sizeof out);

    FPGA_Write(Z_OFFSET ,out ,1);

    memset(out ,p ,sizeof out);
    FPGA_Write(Z_OFFSET+1 ,out ,1);

    memset(out ,0x02 ,sizeof out);
    FPGA_Write(Z_OFFSET ,out ,1);

    memset(out ,i ,sizeof out);
    FPGA_Write(Z_OFFSET+2 ,out ,1);

    memset(out ,0x02 ,sizeof out);
    FPGA_Write(Z_OFFSET ,out ,1);

    memset(out ,d ,sizeof out);
    FPGA_Write(Z_OFFSET+3 ,out ,1);

    memset(out ,0x03 ,sizeof out);
    FPGA_Write(Z_OFFSET ,out ,1);

    memset(out ,s ,sizeof out);
    FPGA_Write(Z_OFFSET+1 ,out ,1);

}

/*
********************************************************************************************************
** Function name:		Z_Velocity_Control
** Descriptions:		Z轴速度控制
** input parameters:            Z轴速度值
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void Z_Velocity_Control(long v)
{
    uint8 out[2];

    memset(out ,0x04 ,sizeof out);
    FPGA_Write(Z_OFFSET ,out ,1);

    memset(out ,(char)v & 0xff ,sizeof out);
    FPGA_Write(Z_OFFSET+1 ,out ,1);

    memset(out ,0x04 ,sizeof out);
    FPGA_Write(Z_OFFSET ,out ,1);

    memset(out ,(char)v >> 8 ,sizeof out);
    FPGA_Write(Z_OFFSET+2 ,out ,1);
    /*something todo ,
    IOZ0 &= 0xf8;
    outb((Address_Z),0x01);
    outb((Address_Z+1),IOZ0|=0x01);*/
}

/*
********************************************************************************************************
** Function name:		Z_Position_Control
** Descriptions:		Z轴位置控制
** input parameters:            Z轴位置
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void Z_Position_Control(long p)
{
    uint8 out[2];

    memset(out ,0x10 ,sizeof out);
    FPGA_Write(Z_OFFSET ,out ,1);

    memset(out ,(char)p & 0xff ,sizeof out);
    FPGA_Write(Z_OFFSET+1 ,out ,1);

    memset(out ,0x10 ,sizeof out);
    FPGA_Write(Z_OFFSET ,out ,1);

    memset(out ,(char)(p >> 8) & 0xff ,sizeof out);
    FPGA_Write(Z_OFFSET+2 ,out ,1);

    memset(out ,0x10 ,sizeof out);
    FPGA_Write(Z_OFFSET ,out ,1);

    memset(out ,(char)(p >> 16) & 0xff ,sizeof out);
    FPGA_Write(Z_OFFSET+3 ,out ,1);
    /*something todo ,
    outb((Address_Z),0x01);
    outb((Address_Z+1),IOZ0&=0xf8);*/
}

/*
********************************************************************************************************
** Function name:		IO0_Write
** Descriptions:		FPGA写入IO0
** input parameters:    IO0的值
   IO1 xxx*****         音量大小
   IO1 ***x****         音量开关
   IO1 ****x***         24V电压开关
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void IO0_Write(uint8 c)
{

    uint8 out[2];
    memset(out ,c ,sizeof out);
    FPGA_Write(IO_OFFSET+0 ,out ,1);

}

/*
********************************************************************************************************
** Function name:		IO1_Write
** Descriptions:		FPGA写入IO1
** input parameters:       IO1的值
   IO1 *x******         总电源开关标志
   IO1 **x*****         放电极性标志
   IO1 ****x***         油泵开关标志
   IO1 *****x**         变压器开关标志
   IO1 ******x*         风扇开关标志
   IO1 *******x         副电源开关标志
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void IO1_Write(uint8 c)
{

    uint8 out[2];
    memset(out ,c ,sizeof out);
    FPGA_Write(IO_OFFSET+1 ,out ,1);

}

/*
********************************************************************************************************
** Function name:		IO2_Write
** Descriptions:		FPGA写入IO2
** input parameters:    IO2的值
    IO2 ********        放电电流值
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void IO2_Write(uint8 c)
{

    uint8 out[2];
    memset(out ,c ,sizeof out);
    FPGA_Write(IO_OFFSET+2 ,out ,1);

}

/*
********************************************************************************************************
** Function name:		IO3_Write
** Descriptions:		FPGA写入IO3
** input parameters:    IO3的值
    IO3 ****xxxx        高电压数值
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void IO3_Write(uint8 c)
{

    uint8 out[2];
    memset(out ,c ,sizeof out);
    FPGA_Write(IO_OFFSET+3 ,out ,1);

}

/*
********************************************************************************************************
** Function name:		IO4_Write
** Descriptions:		FPGA写入IO4
** input parameters:    IO4的值
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void IO4_Write(uint8 c)
{

    uint8 out[2];
    memset(out ,c ,sizeof out);
    FPGA_Write(IO_OFFSET+4 ,out ,1);

}

/*
********************************************************************************************************
** Function name:		IOZ0_Write
** Descriptions:		FPGA写入IOZ0
** input parameters:    IOZ0的值
    IOZ0 **x*****       Z轴ENZ
    IOZ0 ***x****       sl3
    IOZ0 *****xxx       Z轴间隙
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void IOZ0_Write(uint8 c)
{

    uint8 out[2];

    memset(out ,0x01 ,sizeof out);
    FPGA_Write(Z_OFFSET ,out ,1);

    memset(out ,c ,sizeof out);
    FPGA_Write(Z_OFFSET+1 ,out ,1);

}

/*
********************************************************************************************************
** Function name:		OSC0_Write
** Descriptions:		FPGA写入OSC0
** input parameters:    OSC0的值
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void OSC0_Write(uint8 c)
{

    uint8 out[2];

    memset(out ,0x01 ,sizeof out);
    FPGA_Write(OSC_OFFSET ,out ,1);

    memset(out ,c ,sizeof out);
    FPGA_Write(OSC_OFFSET+1 ,out ,1);

}

/*
********************************************************************************************************
** Function name:		OSC1_Write
** Descriptions:		FPGA写入OSC1
** input parameters:    OSC1的值
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void OSC1_Write(uint8 c)
{

    uint8 out[2];

    memset(out ,0x01 ,sizeof out);
    FPGA_Write(OSC_OFFSET ,out ,1);

    memset(out ,c ,sizeof out);
    FPGA_Write(OSC_OFFSET+2 ,out ,1);

}

/*
********************************************************************************************************
** Function name:		OSC2_Write
** Descriptions:		FPGA写入OSC2
** input parameters:    OSC2的值
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void OSC2_Write(uint8 c)
{

    uint8 out[2];

    memset(out ,0x02 ,sizeof out);
    FPGA_Write(OSC_OFFSET ,out ,1);

    memset(out ,c ,sizeof out);
    FPGA_Write(OSC_OFFSET+3 ,out ,1);
}

/*
********************************************************************************************************
** Function name:		Serial_Read
** Descriptions:		串口读取数据
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  数据
********************************************************************************************************
*/
uint8 Serial_Read(void)
{
    uint8 in[2];
    uint32 n = 0;

    memset(in ,0x00 ,sizeof in);

    if(fpga_fd < 0)
        return 0;

    lseek(fpga_fd, SERIAL_BASE + BUFFER_OFFSET, SEEK_SET);
    n = read(fpga_fd ,in ,1);

    return in[0];
}
