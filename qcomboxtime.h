#ifndef QCOMBOXTIME_H
#define QCOMBOXTIME_H

#include "qcombox.h"
#include "sparkinfo.h"
#include <QWidget>
#include <QTimer>
#include <QTime>
#include "fm25v02.h"
#include "setting.h"

#define HOURS 0
#define MINUTES 1
#define SECONDS 2

class QcomboxTime :public Qcombox
{
    Q_OBJECT
public:
    explicit QcomboxTime(QWidget *parent = 0 ,QString l = "",bool b = false);

private:
    void initTime();
    void timeToString();
    bool flag;
    QTimer *timer ;
    QTime time ;

    SixBytes current;
    SixBytes target;

signals:

public slots:
    void valueUpdate();
    void updateTimer();
};

#endif // QCOMBOXTIME_H
