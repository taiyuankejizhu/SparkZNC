#ifndef _FPGA_H_
#define _FPGA_H_

typedef char uint8;

typedef unsigned short uint16;

typedef unsigned int uint32;

typedef long uint64;

//#define PAGE_1 0x00
#define GPMC_BASE 0x0000
#define GPMI_BASE 0x0000
#define ISA_BASE 0x0240

#define X_OFFSET 0x00
#define Y_OFFSET 0x40
#define Z_OFFSET 0x80
#define OSC_OFFSET 0xC0
#define IO_OFFSET 0xE0

#define SERIAL_BASE 0x02f7
#define BUFFER_OFFSET 0x00
#define SPI_OFFSET 0x20

/*
********************************************************************************************************
** Function name:		FPGA_Init
** Descriptions:		初始化FPGA接口
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint8 FPGA_Init(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		FPGA_Info
** Descriptions:		读取FPGA信息
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void FPGA_Info(void);

#ifdef __cplusplus

}

#endif

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
#ifdef __cplusplus

extern "C" {

#endif

uint32 FPGA_Read(uint32 , uint8 *, uint32);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		FPGA_Write
** Descriptions:		FPGA读取数据
** input parameters:    pBuf:   写入数据的指针
**                      num:    要写数据的长度
** output parameters:   无
** Returned value:      uint32  写入的数据长度
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint32 FPGA_Write(uint32 , uint8 *,uint32);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		Serial_Read
** Descriptions:		串口读取数据
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  数据
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint8 Serial_Read(void);

#ifdef __cplusplus

}

#endif

#endif
