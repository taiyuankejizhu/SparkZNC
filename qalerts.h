#ifndef QALERTS_H
#define QALERTS_H

#include <QWidget>
#include <QPainter>
#include <QApplication>
#include "sparkinfo.h"

#define FIRE_SET 0x80
#define FIRE_RESET 0x7f
#define UP_SET 0x40
#define UP_RESET 0xbf
#define DOWN_SET 0x20
#define DOWN_RESET 0xdf
#define SHORT_SET 0x10
#define SHORT_RESET 0xef

#define ALTER_COLOR Qt::red
#define BACK_COLOR  Qt::white
#define TEXT_COLOR  Qt::darkMagenta

class QAlerts : public QWidget
{
    Q_OBJECT
public:
    explicit QAlerts(QWidget *parent = 0);

private:
    QString mesg_label[10];

protected:
    void paintEvent(QPaintEvent *);
    void initLabel();

    /*标志报警栏的状态，x*******第8位是火警，*x******第7位是上限位报警，**x*****第6位是下限位报警
    ***x****第5位是短路报警*/
    char status;
    
signals:
    
public slots:
    void alertCheck();
};

#endif // QALERTS_H
