#include "fm25v02.h"
#include "setting.h"
#include "fcntl.h"
#include "sys/ioctl.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"
#include "linux/types.h"
#include <linux/spi/spidev.h>

// FM25V02 Op-code Commands
#define		WREN			0x06	// Set Write Enable Latch
#define		WRDI			0x04	// Write Disable
#define		RDSR			0x05	// Read Status Register
#define		WRSR			0x01	// Write Status Register
#define		READ			0x03	// Read Memory Data.
#define 	FSTRD			0x0B	// Fast Read Memory Data.
#define		WRITE			0x02 	// Write Memory Data
#define		SLEEP			0xB9	// Enter Sleep Mode.
#define		RDID			0x9F	// Read Device ID.
#define 	SNR			0xC3	// Read S/N.

/*铁电存储器的文件描述符*/
static int spi_fb;
static uint8 mode;
static uint8 bits = 8;
static uint32 speed = 20000000;

static struct spi_ioc_transfer xfer [2];

/*
********************************************************************************************************
** Function name:		DelayNS
** Descriptions:		长软件延时
** input parameters:    uiDly延时控制值，值越大，延时越长
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void Delay (uint32  c)
{
    uint32  i;

    for(; c > 0; c--){
        for(i = 0; i < 100; i++);
    }
}

/*
********************************************************************************************************
** Function name:		FM25V02_Init
** Descriptions:		初始化SPI1接口，设置为主机
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
uint8 FM25V02_Init(void)
{
    // Initialization FM Device Control.
    int ret = 0;

    spi_fb = open(SPI_FILE, O_RDWR);
    if(spi_fb < 0){
        printf("Can't open SPI device ,load parameter by default!\n");
        return 0;
    }
    else{
        /*spi mode*/
        ret = ioctl(spi_fb, SPI_IOC_RD_MODE, &mode);
        if (ret == -1)
                printf("can't get spi mode");

        ret = ioctl(spi_fb, SPI_IOC_WR_MODE, &mode);
        if (ret == -1)
                printf("can't set spi mode");

        /*bits per word*/
        ret = ioctl(spi_fb, SPI_IOC_WR_BITS_PER_WORD, &bits);
        if (ret == -1)
                printf("can't set bits per word");

        ret = ioctl(spi_fb, SPI_IOC_RD_BITS_PER_WORD, &bits);
        if (ret == -1)
                printf("can't get bits per word");

        /*max speed hz*/
        ret = ioctl(spi_fb, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
        if (ret == -1)
                printf("can't set max speed hz");

        ret = ioctl(spi_fb, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
        if (ret == -1)
                printf("can't get max speed hz");

        printf("spi mode: %d\n", mode);
        printf("bits per word: %d\n", bits);
        printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

        memset (xfer , 0 , sizeof xfer );

        return 1;
    }
}

/*
********************************************************************************************************
** Function name:		FM25V02_Exit
** Descriptions:		关闭SPI1接口
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void FM25V02_Exit(void)
{
    /*关闭FM25V02*/
    FM25V02_PWDN();

    if(spi_fb >= 0)
        close(spi_fb);
    printf("FM25V02_Exit()!");
}

/*
********************************************************************************************************
** Function name:		MSPISendData
** Descriptions:		SPI主模式发送数据
** input parameters:    uiData： 将要发送的数据
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void MSPISendData(uint8 data)
{
    printf("MSPISendData()\n");
    uint8 txz[1] = {0};
    uint8 rxz[1] = {0};

    memset(txz , 0 ,sizeof txz);
    memset(rxz , 0 ,sizeof rxz);
    memset(xfer , 0 , sizeof xfer);

    txz[0] = data;

    int ret;

    xfer[0].tx_buf = (unsigned long)txz;
    xfer[0].len = 1;

    printf("send:%.2X\n" ,txz[0]);
    ret = ioctl(spi_fb, SPI_IOC_MESSAGE(1), &xfer[0]);
    puts("");
}

/*
********************************************************************************************************
** Function name:		FM25V02_WREN()
** Descriptions:		F25V02写允许命令
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void FM25V02_WREN(void)
{
    MSPISendData(WREN);
    printf("FM25V02_WREN()\n");
}

/*
********************************************************************************************************
** Function name:		FM25V02_WRDI()
** Descriptions:		F25V02写禁止命令
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void FM25V02_WRDI(void)
{
    MSPISendData(WRDI);
    printf("FM25V02_WRDI()\n");
}

/*
********************************************************************************************************
** Function name:		FM25V02_WRSR()
** Descriptions:		F25V02寄存器配置
** input parameters:    寄存器配置内容(uint8)
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void FM25V02_WRSR(uint8 Reg_Status)
{
    FM25V02_WREN();
    printf("FM25V02_WRSR()\n");
    uint8 txz[2] = {0};
    uint8 rxz[1] = {0};

    memset(txz , 0 ,sizeof txz);
    memset(rxz , 0 ,sizeof rxz);
    memset(xfer , 0 , sizeof xfer);

    txz[0] = WRSR;
    txz[1] = Reg_Status;

    int ret;

    xfer[0].tx_buf = (unsigned long)txz;
    xfer[0].len = 2;

    ret = ioctl(spi_fb, SPI_IOC_MESSAGE(1), &xfer[0]);
    printf("send:%.2X ,%.2X\n" ,txz[0],txz[1]);
    FM25V02_WRDI();
}

/*
********************************************************************************************************
** Function name:		FM25V02_RDSR()
** Descriptions:		F25V02寄存器配置内容读取
** input parameters:    无
** output parameters:   无
** Returned value:      寄存器配置值
********************************************************************************************************
*/
uint8 FM25V02_RDSR(void)
{
    printf("FM25V02_RDSR()\n");
    uint8 txz[1] = {0};
    uint8 rxz[1] = {0};

    memset(txz , 0 ,sizeof txz);
    memset(rxz , 0 ,sizeof rxz);
    memset(xfer , 0 , sizeof xfer);

    txz[0] = RDSR;

    int ret;

    xfer[0].tx_buf = (unsigned long)txz;
    xfer[0].len = 1;

    xfer[1].rx_buf = (unsigned long)rxz;
    xfer[1].len = 1;

    ret = ioctl(spi_fb, SPI_IOC_MESSAGE(2), xfer);
    printf("send:%.2X\n" ,txz[0]);
    printf("receive:%.2X\n" ,rxz[0]);
    return rxz[0];
}

/*
********************************************************************************************************
** Function name:		FM25V02_PWDN()
** Descriptions:		F25V02进入睡眠模式
** input parameters:    无
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void FM25V02_PWDN(void)
{
    MSPISendData(SLEEP);
    printf("FM25V02_PWDN()\n");
}

/*
********************************************************************************************************
** Function name:		FM25V02_WRITE()
** Descriptions:		对F25V02内固定地址写入一个字节内容
** input parameters:    写入地址18位，用三字节表示其中高6位无效；写入的数据
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void FM25V02_WRITE(uint32 WAddr, uint8 *pBuf, uint32 num)
{
        FM25V02_WREN();
        uint8  WAddrL, WAddrH;

        // FM25V02 ADDR: 0x000000 ~ 0x007FFF.
        // FM25V05 ADDR: 0x000000 ~ 0x00FFFF.
        // FM25V10 ADDR: 0x000000 ~ 0x01FFFF.
        // FM25H10 ADDR: 0x000000 ~ 0x03FFFF.
        WAddrL =   WAddr & 0x000000FF;
        WAddrH = ((WAddr >>  8) & 0x0000007F);

        printf("L:%.2X \n" ,WAddrL);
        printf("H:%.2X \n" ,WAddrH);

        uint8 txz[15] = {0};
        uint8 rxz[1] = {0};

        memset(txz , 0 ,sizeof txz);
        memset(rxz , 0 ,sizeof rxz);
        memset(xfer , 0 , sizeof xfer);

        txz[0] = WRITE;
        txz[1] = WAddrH;
        txz[2] = WAddrL;

        uint32 ret;
        for(ret = 0;ret < num;ret++)
                txz[ret+3] = pBuf[ret];


        xfer[0].tx_buf = (unsigned long)txz;
        xfer[0].len = num+3;

        ret = ioctl(spi_fb, SPI_IOC_MESSAGE(1), &xfer[0]);

        printf("write:");
        for (ret = 0; ret < num+3; ret++) {
                if (!(ret % num+3))
                        puts("");
                printf("%.2X ", txz[ret]);
        }
        puts("");
        FM25V02_WRDI();
}

/*
********************************************************************************************************
** Function name:		FM25V02_READ()
** Descriptions:		从F25V02内固定地址读出一个字节内容
** input parameters:    读数地址18位，用三字节表示其中高6位无效
** output parameters:   无
** Returned value:      无
********************************************************************************************************
*/
void FM25V02_READ(uint32 RAddr, uint8 *pBuf, uint32 num)
{
        uint8 RAddrL, RAddrH;

        // FM25V02 ADDR: 0x000000 ~ 0x007FFF.
        // FM25V05 ADDR: 0x000000 ~ 0x00FFFF.
        // FM25V10 ADDR: 0x000000 ~ 0x01FFFF.
        // FM25H10 ADDR: 0x000000 ~ 0x03FFFF.
        RAddrL =   RAddr & 0x000000FF;
        RAddrH = ((RAddr >>  8) & 0x0000007F);

        printf("L:%.2X \n" ,RAddrL);
        printf("H:%.2X \n" ,RAddrH);

        uint8 txz[3] = {0};
        uint8 rxz[1] = {0};

        memset(txz , 0 ,sizeof txz);
        memset(rxz , 0 ,sizeof rxz);
        memset(xfer , 0 , sizeof xfer);

        txz[0] = READ;
        txz[1] = RAddrH;
        txz[2] = RAddrL;

        uint32 ret;

        xfer[0].tx_buf = (unsigned long)txz;
        xfer[0].len = 3;

        xfer[1].rx_buf = (unsigned long)pBuf;
        xfer[1].len = num;

        ret = ioctl(spi_fb, SPI_IOC_MESSAGE(2), xfer);

        printf("read:");
        for (ret = 0; ret < num; ret++) {
                if (!(ret % num))
                        puts("");
                printf("%.2X ", pBuf[ret]);
        }
        puts("");
}
