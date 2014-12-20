#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QThread>
#include "sparkinfo.h"
#include "sparkthread.h"
#include "fpga.h"
#include "unistd.h"
#include "sys/time.h"
#include "sys/reboot.h"

#define A_CYCLE 20      /*关机的延时计数周期*/
#define B_CYCLE 300000  /*睡眠的延时计数周期10S*/
#define C_CYCLE 5       /*读取速度的延时计数周期*/
#define D_CYCLE 100     /*关风扇延时的计数周期*/
#define E_CYCLE 100     /*重启的延时计数周期*/

/*边缘变化三态*/
#define FALL 0x7f
#define NONE 0x00
#define RISE 0xff

class ScanThread : public QThread
{
    Q_OBJECT
public:
    explicit ScanThread(QObject *parent = 0);

    unsigned int a_cycle;
    unsigned int b_cycle;
    unsigned int c_cycle;
    unsigned int d_cycle;
    unsigned int e_cycle;

    bool a_last;
    unsigned char a_edge;
    bool b_last;
    unsigned char b_edge;
    bool c_last;
    unsigned char c_edge;
    bool d_last;
    unsigned char d_edge;
    /*键盘与遥控操作的互斥锁，保证在键盘操作方向和模式时遥控板不起作用*/
    bool a_lock;

    void run();
    void AxisSwitch(HandAxis ,HandDirect);
    void DoMove(HandMove *);
    void DoStop(HandMove *);
    void Move();
    void Scan_Panel();
    static void Check_Alert();
    static long X_Count();
    static long Y_Count();
    static long Z_Count();
    static long Null_Count();
    static long X_Velocity();
    static long Y_Velocity();
    static long Z_Velocity();
    static long Z_Origin();
    static char Voltage_Read();
    static void Update_Relay();

    virtual ~ScanThread();
signals:
    void clear();

public slots:
    void keyPress(int);
    void keyRelease(int);

};

#endif // SCANTHREAD_H
