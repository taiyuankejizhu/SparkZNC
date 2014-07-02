#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QThread>

#define A_CYCLE 5   /*关机的延时计数周期*/
#define B_CYCLE 10  /*睡眠的延时计数周期*/
#define C_CYCLE 5  /*读取速度的延时计数周期*/
#define D_CYCLE 128 /*关风扇延时*/

class ScanThread : public QThread
{
    Q_OBJECT
public:
    explicit ScanThread(QObject *parent = 0);

    unsigned int a_cycle;
    unsigned int b_cycle;
    unsigned int c_cycle;
    unsigned int d_cycle;

    void run();
    static long X_Count();
    static long Y_Count();
    static long Z_Count();
    static long X_Velocity();
    static long Y_Velocity();
    static long Z_Velocity();
    static long Z_Origin();
    static char Voltage_Read();
    static void Check_Alert();
    static void Update_Relay();

    virtual ~ScanThread();
signals:

public slots:

};

#endif // SCANTHREAD_H
