#include "qalerts.h"
#include "qdebug.h"
#define COUNT 18
#define GAP 17
#define OFFSET 15
#define PAD 4
QAlerts::QAlerts(QWidget *parent) :
    QWidget(parent)
{
    initLabel();

    flag = false;

    timer = new QTimer(this);
    timer->setInterval(1000);

    connect(timer ,SIGNAL(timeout()) ,this ,SLOT(flashLamp()));

    status = 0x00;
}

/*输出字符串初始化*/
void QAlerts::initLabel()
{
    mesg_label[0] = tr("空");
    mesg_label[1] = tr("火警");
    mesg_label[2] = tr("Z上");
    mesg_label[3] = tr("Z下");
    mesg_label[4] = tr("短路");
    mesg_label[5] = tr("油位");
    mesg_label[6] = tr("积碳");
    mesg_label[7] = tr("Y上");
    mesg_label[8] = tr("Y下");
}

void QAlerts::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();

    QFont font = qApp->font();
    font.setPointSize(11);
    painter.setFont(font);

    QFontMetricsF fm(font);
    if(flag){
        if(status & FIRE_SET){
            QPen pen(ALTER_COLOR);
            pen.setWidth(4);
            painter.setPen(pen);
            painter.drawEllipse(QPointF(OFFSET+width()/GAP,height()/2),width()/COUNT,height()/2-4);
            double h = fm.size(Qt::TextSingleLine,mesg_label[1]).height();
            double w = fm.size(Qt::TextSingleLine,mesg_label[1]).width();
            painter.setPen(TEXT_COLOR);
            painter.drawText(OFFSET+width()/GAP-w/2 , height()/2+h/2-4,mesg_label[1]);
        }
        if(status & Z_UP_SET){
            QPen pen(ALTER_COLOR);
            pen.setWidth(4);
            painter.setPen(pen);
            painter.drawEllipse(QPointF(OFFSET+width()*3/GAP,height()/2),width()/COUNT,height()/2-4);
            double h = fm.size(Qt::TextSingleLine,mesg_label[2]).height();
            double w = fm.size(Qt::TextSingleLine,mesg_label[2]).width();
            painter.setPen(TEXT_COLOR);
            painter.drawText(OFFSET+width()*3/GAP-w/2 , height()/2+h/2-4,mesg_label[2]);
        }
        if(status & Z_DOWN_SET){
            QPen pen(ALTER_COLOR);
            pen.setWidth(4);
            painter.setPen(pen);
            painter.drawEllipse(QPointF(OFFSET+width()*5/GAP,height()/2),width()/COUNT,height()/2-4);
            double h = fm.size(Qt::TextSingleLine,mesg_label[3]).height();
            double w = fm.size(Qt::TextSingleLine,mesg_label[3]).width();
            painter.setPen(TEXT_COLOR);
            painter.drawText(OFFSET+width()*5/GAP-w/2 , height()/2+h/2-4,mesg_label[3]);
        }
        if(status & SHORT_SET){
            QPen pen(ALTER_COLOR);
            pen.setWidth(4);
            painter.setPen(pen);
            painter.drawEllipse(QPointF(OFFSET+width()*7/GAP,height()/2),width()/COUNT,height()/2-4);
            double h = fm.size(Qt::TextSingleLine,mesg_label[4]).height();
            double w = fm.size(Qt::TextSingleLine,mesg_label[4]).width();
            painter.setPen(TEXT_COLOR);
            painter.drawText(OFFSET+width()*7/GAP-w/2 , height()/2+h/2-4,mesg_label[4]);
        }
        if(status & LEVER_SET){
            QPen pen(ALTER_COLOR);
            pen.setWidth(4);
            painter.setPen(pen);
            painter.drawEllipse(QPointF(OFFSET+width()*9/GAP,height()/2),width()/COUNT,height()/2-4);
            double h = fm.size(Qt::TextSingleLine,mesg_label[5]).height();
            double w = fm.size(Qt::TextSingleLine,mesg_label[5]).width();
            painter.setPen(TEXT_COLOR);
            painter.drawText(OFFSET+width()*9/GAP-w/2 , height()/2+h/2-4,mesg_label[5]);
        }
        if(status & CARBORN_SET){
            QPen pen(ALTER_COLOR);
            pen.setWidth(4);
            painter.setPen(pen);
            painter.drawEllipse(QPointF(OFFSET+width()*11/GAP,height()/2),width()/COUNT,height()/2-4);
            double h = fm.size(Qt::TextSingleLine,mesg_label[6]).height();
            double w = fm.size(Qt::TextSingleLine,mesg_label[6]).width();
            painter.setPen(TEXT_COLOR);
            painter.drawText(OFFSET+width()*11/GAP-w/2 , height()/2+h/2-4,mesg_label[6]);
        }
        if(status & Y_UP_SET){
            QPen pen(ALTER_COLOR);
            pen.setWidth(4);
            painter.setPen(pen);
            painter.drawEllipse(QPointF(OFFSET+width()*13/GAP,height()/2),width()/COUNT,height()/2-4);
            double h = fm.size(Qt::TextSingleLine,mesg_label[7]).height();
            double w = fm.size(Qt::TextSingleLine,mesg_label[7]).width();
            painter.setPen(TEXT_COLOR);
            painter.drawText(OFFSET+width()*13/GAP-w/2 , height()/2+h/2-4,mesg_label[7]);
        }
        if(status & Y_DOWN_SET){
            QPen pen(ALTER_COLOR);
            pen.setWidth(4);
            painter.setPen(pen);
            painter.drawEllipse(QPointF(OFFSET+width()*15/GAP,height()/2),width()/COUNT,height()/2-4);
            double h = fm.size(Qt::TextSingleLine,mesg_label[8]).height();
            double w = fm.size(Qt::TextSingleLine,mesg_label[8]).width();
            painter.setPen(TEXT_COLOR);
            painter.drawText(OFFSET+width()*15/GAP-w/2 , height()/2+h/2-4,mesg_label[8]);
        }
    }
    else{
        QPen pen(BACK_COLOR);
        painter.setPen(pen);
        painter.drawRect(PAD ,PAD ,width() - 4*PAD ,height() -4*PAD);
    }

    painter.restore();
}

void QAlerts::alertCheck()
{
    char tmp = status;
    /*火警*/
    if(spark_info->b_array[B_FIRE_ALERT])
        status |= FIRE_SET;
    else
        status &= FIRE_RESET;
    /*Z轴上限*/
    if(spark_info->b_array[B_Z_UP_ALERT])
        status |= Z_UP_SET;
    else
        status &= Z_UP_RESET;
    /*Z轴下限*/
    if(spark_info->b_array[B_Z_DOWN_ALERT])
        status |= Z_DOWN_SET;
    else
        status &= Z_DOWN_RESET;

    if(status != 0x00){
        if(tmp == 0x00)
            timer->start();
    }
    else{
        timer->stop();
    }

    update();
}

void QAlerts::flashLamp()
{
    if(flag){
        flag = false;
    }
    else{
        flag = true;
    }
    qDebug()<<flag;
    update();
}
