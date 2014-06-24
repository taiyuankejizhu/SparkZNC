#ifndef _FPGA_H_
#define _FPGA_H_

typedef char uint8;

typedef unsigned short uint16;

typedef unsigned int uint32;

typedef long uint64;

//#define PAGE_1 0x00
#define GPMI_BASE 0x0000
#define ISA_BASE 0x0240

#define X_OFFSET 0x00
#define Y_OFFSET 0x04
#define Z_OFFSET 0x08
#define OSC_OFFSET 0x10
#define IO_OFFSET 0x18
#define SPI_OFFSET 0x20

#define SERIAL_BASE 0x02f7
#define BUFFER_OFFSET 0x00

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
** Function name:		Voltage_Read
** Descriptions:		FPGA读取电压
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  电压值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint8 Voltage_Read(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		X_Count
** Descriptions:		FPGA读取X轴计数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  X轴计数值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint64 X_Count(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		Y_Count
** Descriptions:		FPGA读取Y轴计数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  Y轴计数值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint64 Y_Count(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		Z_Count
** Descriptions:		FPGA读取Z轴计数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  Z轴计数值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint64 Z_Count(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		X_Velocity
** Descriptions:		FPGA读取X速度数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  X轴速度值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint64 X_Velocity(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		Y_Velocity
** Descriptions:		FPGA读取Y速度数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  Y轴速度值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint64 Y_Velocity(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		Z_Velocity
** Descriptions:		FPGA读取Z速度数值
** input parameters:    无
** output parameters:   无
** Returned value:      uint64  Z轴速度值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint64 Z_Velocity(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		IO0_Write
** Descriptions:		FPGA写入IO0
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  IO0的值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void IO0_Write(uint8);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		IO1_Write
** Descriptions:		FPGA写入IO1
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  IO1的值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void IO1_Write(uint8);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		IO2_Write
** Descriptions:		FPGA写入IO2
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  IO2的值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void IO2_Write(uint8);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		IO3_Write
** Descriptions:		FPGA写入IO3
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  IO3的值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void IO3_Write(uint8);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		IO4_Write
** Descriptions:		FPGA写入IO4
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  IO4的值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void IO4_Write(uint8);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		IOZ0_Write
** Descriptions:		FPGA写入IOZ0
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  IOZ0的值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void IOZ0_Write(uint8);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		OSC0_Write
** Descriptions:		FPGA写入OSC0
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  OSC0的值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void OSC0_Write(uint8);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		OSC1_Write
** Descriptions:		FPGA写入OSC1
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  OSC1的值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void OSC1_Write(uint8);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		OSC2_Write
** Descriptions:		FPGA写入OSC2
** input parameters:    无
** output parameters:   无
** Returned value:      uint8  OSC2的值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void OSC2_Write(uint8);

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
