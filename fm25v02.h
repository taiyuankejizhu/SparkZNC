#ifndef _FM25V02_H_
#define _FM25V02_H_

typedef char uint8;

typedef unsigned short uint16;

typedef unsigned int uint32;

/*
********************************************************************************************************
** Function name:		FM25V02_Init
** Descriptions:		初始化SPI1接口，设置为主机
** input parameters:    无
** output parameters:   无
** Returned value:      文件打开结果
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

extern uint8 FM25V02_Init(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		FM25V02_Exit
** Descriptions:		关闭SPI1接口
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void FM25V02_Exit(void);

#ifdef __cplusplus

}

#endif
/*
********************************************************************************************************
** Function name:		FM25V02_WRSR()
** Descriptions:		F25V02寄存器配置
** input parameters:    寄存器配置内容(int)
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void FM25V02_WRSR(uint8 Reg_Status);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		FM25V02_RDSR()
** Descriptions:		F25V02寄存器配置内容读取
** input parameters:    无
** output parameters:   无
** Returned value:      寄存器配置值
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

uint8 FM25V02_RDSR(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		FM25V02_WREN
** Descriptions:		开SPI1写使能
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void FM25V02_WREN(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		FM25V02_WRDI
** Descriptions:		SPI1写禁止命令
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void FM25V02_WRDI(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		FM25V02_PWDN()
** Descriptions:		F25V02进入睡眠模式
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void FM25V02_PWDN(void);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		FM25V02_WRITE()
** Descriptions:		对F25V02内固定地址写入一个字节内容
** input parameters:    写入地址18位，用三字节表示其中高6位无效；写入的数据
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void FM25V02_WRITE(uint32 WAddr, uint8 *pBuf, uint32 num);

#ifdef __cplusplus

}

#endif

/*
********************************************************************************************************
** Function name:		FM25V02_READ()
** Descriptions:		从F25V02内固定地址读出一个字节内容
** input parameters:    读数地址18位，用三字节表示其中高6位无效
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
#ifdef __cplusplus

extern "C" {

#endif

void FM25V02_READ (uint32 RAddr, uint8 *pBuf, uint32 num);

#ifdef __cplusplus

}

#endif

#endif
