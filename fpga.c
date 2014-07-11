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
    printf("ISR is no need info!");
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

    memset(in ,0x00 ,sizeof in);

    if(fpga_fd < 0)
        return 0;

    lseek(fpga_fd, SERIAL_BASE + BUFFER_OFFSET, SEEK_SET);
    read(fpga_fd ,in ,1);

    return in[0];
}
