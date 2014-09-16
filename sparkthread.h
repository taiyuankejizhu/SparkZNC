#ifndef SPARKTHREAD_H
#define SPARKTHREAD_H

#include <QThread>
#include <QTime>

#define SEARCH 0        /*放电开始后首先下降到最深位置，并判断当电行*/
#define WORK 1      /*放电头与工件接触后放电一段时间，根据放电行中的工时参数*/
#define UP 2      /*根据放电行参数中的排渣参数放电时间结束后上升一定高度*/
#define DOWN 3    /*上升到位后再下降到与工件接触*/
#define END 4        /*放电加工结束后回到开始时的位置*/

#define CURRENT_ROW spark_info->uint_array[UINT_CURRENT_ROM]    /*当前放电行的索引的宏定义*/

class SparkThread : public QThread
{
    Q_OBJECT
public:
    explicit SparkThread(QObject *parent = 0);

    /*放电开始之前的坐标位置，放电结束后回到这个位置*/
    long x_start;
    long y_start;
    long z_start;
    long z_diff;

    QTime timer;

    char state;
    char l_state;
    bool d_state;
    void run();
    static void Z_Position_Control(long);
    static void Z_Velocity_Control(long);
    static void Z_PID_Tune(char ,char ,char ,char);
    static void Set_Row(unsigned int);
    virtual ~SparkThread();

signals:

public slots:
    void sparkChange();

};

#endif // SPARKTHREAD_H
