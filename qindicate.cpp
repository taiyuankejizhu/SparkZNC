#include "qindicate.h"
#include "sparkinfo.h"

Qindicate::Qindicate(QWidget *parent) :
    QWidget(parent)
{
    pad   = 3;
    value = 0;
    max_value = 255;
    min_value = 0;
    label = "Null";
    v_color = QColor(255,204,0);
    b_color = QColor(255,255,255);
    l_color = QColor(0,0,0);
    r_color = QColor(64,64,64);
    z_color = QColor(128,128,128);
}

Qindicate::Qindicate(QWidget *parent ,QString l ,unsigned int max ,unsigned int min ,unsigned int i):
    QWidget(parent)
{
    pad   = 3;
    value = 0;
    max_value = max;
    min_value = min;
    label = l;
    index = i;
    v_color = QColor(255,204,0);
    b_color = QColor(255,255,255);
    l_color = QColor(0,0,0);
    r_color = QColor(64,64,64);
    z_color = QColor(128,128,128);
}

void Qindicate::drawBackground(QPainter *painter)
{
    painter->save();

    painter->setPen(r_color);
    painter->setBrush(r_color);
    painter->fillRect(0,0,width(),height(),r_color);

    painter->setPen(z_color);
    painter->setBrush(z_color);
    painter->fillRect(pad,pad,width()-2*pad,height()-2*pad,z_color);

    int i;
    painter->setPen(b_color);
    painter->setBrush(b_color);
    /*Draw 3D effect border*/
    for(i = 0;i < pad;i++){
        painter->drawLine(0,i,width()-i-1,i);
    }

    for(i = 0;i < pad;i++){
        painter->drawLine(i,0,i,height()-i-1);
    }

    painter->restore();
}

void Qindicate::drawWidget(QPainter *painter)
{
    painter->save();

    painter->setPen(v_color);
    painter->setBrush(v_color);
    painter->fillRect(pad+(width()-2*pad)/2+1,pad,(width()-2*pad)/2,height()-2*pad,v_color);

    /*画标签*/
    painter->setPen(l_color);
    painter->setBrush(l_color);

    QFont font = qApp->font();
    font.setPointSize(12);
    painter->setFont(font);

    QFontMetricsF fm(font);
    double w = fm.size(Qt::TextSingleLine,label).width();
    double h = fm.size(Qt::TextSingleLine,label).height();

    painter->translate(width()/4, height()/2);
    painter->drawText(-w/2 ,h/2-pad ,label);
    /*画数据*/
    QString str = QString::number(value);

    w = fm.size(Qt::TextSingleLine,str).width();
    h = fm.size(Qt::TextSingleLine,str).height();
    painter->drawText(width()/2-w/2 ,h/2-pad ,str);

    painter->restore();
}

void Qindicate::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    drawBackground(&painter);
    drawWidget(&painter);
}

void Qindicate::mousePressEvent(QMouseEvent *ev)
{
    ev->accept();
    update();
}

void Qindicate::mouseReleaseEvent(QMouseEvent *ev)
{
    ev->accept();
    update();
}

void Qindicate::updateValue()
{
    value = spark_info->uint_array[index];
    if(value > max_value)
        value = max_value;
    if(value < min_value)
        value = min_value;
    update();
}

unsigned int Qindicate::getValue()
{
    return value;
}
