#include "qalerts.h"

QAlerts::QAlerts(QWidget *parent) :
    QWidget(parent)
{
    initLabel();

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

    if(status & FIRE_SET){
        QPen pen(ALTER_COLOR);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(width()/16,height()/2),width()/16,height()/2-4);
        double h = fm.size(Qt::TextSingleLine,mesg_label[1]).height();
        double w = fm.size(Qt::TextSingleLine,mesg_label[1]).width();
        painter.setPen(TEXT_COLOR);
        painter.drawText(width()/16-w/2 , height()/2+h/2-4,mesg_label[1]);
    }
    if(status & UP_SET){
        QPen pen(ALTER_COLOR);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(width()*3/16,height()/2),width()/16,height()/2-4);
        double h = fm.size(Qt::TextSingleLine,mesg_label[2]).height();
        double w = fm.size(Qt::TextSingleLine,mesg_label[2]).width();
        painter.setPen(TEXT_COLOR);
        painter.drawText(width()*3/16-w/2 , height()/2+h/2-4,mesg_label[2]);
    }
    if(status & DOWN_SET){
        QPen pen(ALTER_COLOR);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(width()*5/16,height()/2),width()/16,height()/2-4);
        double h = fm.size(Qt::TextSingleLine,mesg_label[3]).height();
        double w = fm.size(Qt::TextSingleLine,mesg_label[3]).width();
        painter.setPen(TEXT_COLOR);
        painter.drawText(width()*5/16-w/2 , height()/2+h/2-4,mesg_label[3]);
    }

    painter.restore();
}

void QAlerts::alertCheck()
{
    /*火警*/
    if(spark_info->b_array[B_FIRE_ALERT])
        status |= FIRE_SET;
    else
        status &= FIRE_RESET;
    /*Z轴上限*/
    if(spark_info->b_array[B_Z_UP_ALERT])
        status |= UP_SET;
    else
        status &= UP_RESET;
    /*Z轴下限*/
    if(spark_info->b_array[B_Z_DOWN_ALERT])
        status |= DOWN_SET;
    else
        status &= DOWN_RESET;

    update();
}
