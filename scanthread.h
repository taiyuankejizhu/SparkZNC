#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QThread>

#define A_CYCLE 5   /*关机的延时计数周期*/
#define B_CYCLE 10  /*睡眠的延时计数周期*/
#define C_CYCLE 5  /*读取速度的延时计数周期*/

class ScanThread : public QThread
{
    Q_OBJECT
public:
    explicit ScanThread(QObject *parent = 0);

    unsigned int a_cycle;
    unsigned int b_cycle;
    unsigned int c_cycle;

    void run();
signals:

public slots:

};

#endif // SCANTHREAD_H
