#ifndef QALERTS_H
#define QALERTS_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QApplication>
#include "sparkinfo.h"

#define FIRE_SET 0x80
#define FIRE_RESET 0x7f
#define Z_UP_SET 0x40
#define Z_UP_RESET 0xbf
#define Z_DOWN_SET 0x20
#define Z_DOWN_RESET 0xdf
#define SHORT_SET 0x10
#define SHORT_RESET 0xef
#define LEVER_SET 0x08
#define LEVER_RESET 0xf7
#define CARBORN_SET 0x04
#define CARBORN_RESET 0xfb
#define Y_UP_SET 0x02
#define Y_UP_RESET 0xfd
#define Y_DOWN_SET 0x01
#define Y_DOWN_RESET 0xfe

#define ALTER_COLOR Qt::red
#define BACK_COLOR  Qt::white
#define TEXT_COLOR  Qt::darkMagenta

class QAlerts : public QWidget
{
    Q_OBJECT
public:
    explicit QAlerts(QWidget *parent = 0);
    QTimer *timer ;
    bool flag;

private:
    QString mesg_label[10];

protected:
    void paintEvent(QPaintEvent *);
    void initLabel();

    /*标志报警栏的状态，x*******第8位是火警，*x******第7位是上限位报警，**x*****第6位是下限位报警
    ***x****第5位是短路报警 ，****x***第4位是油位报警 ，*****x**第3位是积碳报警*/
    char status;
    
signals:
    
public slots:
    void flashLamp();
    void alertCheck();
};

#endif // QALERTS_H
