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
    if(status & Z_UP_SET){
        QPen pen(ALTER_COLOR);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(width()*3/16,height()/2),width()/16,height()/2-4);
        double h = fm.size(Qt::TextSingleLine,mesg_label[2]).height();
        double w = fm.size(Qt::TextSingleLine,mesg_label[2]).width();
        painter.setPen(TEXT_COLOR);
        painter.drawText(width()*3/16-w/2 , height()/2+h/2-4,mesg_label[2]);
    }
    if(status & Z_DOWN_SET){
        QPen pen(ALTER_COLOR);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(width()*5/16,height()/2),width()/16,height()/2-4);
        double h = fm.size(Qt::TextSingleLine,mesg_label[3]).height();
        double w = fm.size(Qt::TextSingleLine,mesg_label[3]).width();
        painter.setPen(TEXT_COLOR);
        painter.drawText(width()*5/16-w/2 , height()/2+h/2-4,mesg_label[3]);
    }
    if(status & SHORT_SET){
        QPen pen(ALTER_COLOR);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(width()*7/16,height()/2),width()/16,height()/2-4);
        double h = fm.size(Qt::TextSingleLine,mesg_label[4]).height();
        double w = fm.size(Qt::TextSingleLine,mesg_label[4]).width();
        painter.setPen(TEXT_COLOR);
        painter.drawText(width()*7/16-w/2 , height()/2+h/2-4,mesg_label[4]);
    }
    if(status & LEVER_SET){
        QPen pen(ALTER_COLOR);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(width()*9/16,height()/2),width()/16,height()/2-4);
        double h = fm.size(Qt::TextSingleLine,mesg_label[5]).height();
        double w = fm.size(Qt::TextSingleLine,mesg_label[5]).width();
        painter.setPen(TEXT_COLOR);
        painter.drawText(width()*9/16-w/2 , height()/2+h/2-4,mesg_label[5]);
    }
    if(status & CARBORN_SET){
        QPen pen(ALTER_COLOR);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(width()*11/16,height()/2),width()/16,height()/2-4);
        double h = fm.size(Qt::TextSingleLine,mesg_label[6]).height();
        double w = fm.size(Qt::TextSingleLine,mesg_label[6]).width();
        painter.setPen(TEXT_COLOR);
        painter.drawText(width()*11/16-w/2 , height()/2+h/2-4,mesg_label[6]);
    }
    if(status & Y_UP_SET){
        QPen pen(ALTER_COLOR);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(width()*13/16,height()/2),width()/16,height()/2-4);
        double h = fm.size(Qt::TextSingleLine,mesg_label[7]).height();
        double w = fm.size(Qt::TextSingleLine,mesg_label[7]).width();
        painter.setPen(TEXT_COLOR);
        painter.drawText(width()*13/16-w/2 , height()/2+h/2-4,mesg_label[7]);
    }
    if(status & Y_DOWN_SET){
        QPen pen(ALTER_COLOR);
        pen.setWidth(4);
        painter.setPen(pen);
        painter.drawEllipse(QPointF(width()*15/16,height()/2),width()/16,height()/2-4);
        double h = fm.size(Qt::TextSingleLine,mesg_label[8]).height();
        double w = fm.size(Qt::TextSingleLine,mesg_label[8]).width();
        painter.setPen(TEXT_COLOR);
        painter.drawText(width()*15/16-w/2 , height()/2+h/2-4,mesg_label[8]);
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
        status |= Z_UP_SET;
    else
        status &= Z_UP_RESET;
    /*Z轴下限*/
    if(spark_info->b_array[B_Z_DOWN_ALERT])
        status |= Z_DOWN_SET;
    else
        status &= Z_DOWN_RESET;

    update();
}
