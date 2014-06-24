#include "qcombox.h"
#include "sparkinfo.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFontMetricsF>
#include <QApplication>

Qcombox::Qcombox(QWidget *parent ,QString l ,unsigned int i):
    QWidget(parent)
{
    pad   = 3 ;
    value = "" ;
    label = l;
    index = i;
    f_color = QColor(64,64,64);
    b_color = QColor(255,255,255);
    l_color = QColor(255,255,255);
    r_color = QColor(64,64,64);
    z_color = QColor(128,128,128);
}

void Qcombox::drawBackground(QPainter *painter)
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

void Qcombox::drawWidget(QPainter *painter)
{
    painter->save();

    painter->setPen(f_color);
    painter->setBrush(f_color);

    painter->fillRect(pad,pad,width()-2*pad,(height()-2*pad)/2,f_color);

    /*Paint the label name*/
    painter->translate(width()/2, height()/4);
    painter->setPen(l_color);
    painter->setBrush(l_color);

    QFont font = qApp->font();
    font.setPointSize(12);
    painter->setFont(font);

    QFontMetricsF fm(font);
    double w = fm.size(Qt::TextSingleLine,label).width();
    double h = fm.size(Qt::TextSingleLine,label).height();

    painter->drawText(-w/2 ,h/2-pad ,label);

    /*Paint the label value*/
    //painter->translate(width()/2, (height()*3)/4);

    painter->setPen(l_color);
    painter->setBrush(l_color);

    w = fm.size(Qt::TextSingleLine,value).width();
    h = fm.size(Qt::TextSingleLine,value).height();
    painter->drawText(-w/2 ,h/2 + height()/2-pad ,value);

    painter->restore();
}

void Qcombox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    drawBackground(&painter);
    drawWidget(&painter);
}

void Qcombox::mousePressEvent(QMouseEvent *ev)
{
        ev->accept();
        update();
}

void Qcombox::mouseReleaseEvent(QMouseEvent *ev)
{
        ev->accept();
        update();
}

void Qcombox::updateValue()
{

    QString s;

    int m = 0;
    long tmp = 0;
    long g = 10000000;
    char ch = '0';
    bool first = false;

    tmp = spark_info->l_array[index];

    if(tmp < 0)
        s.append('-');

    for(m = 0;m < 7;m++){
        if(m == 4){
            if(!first){
                s.append('0');
                s.append('.');
                first = true;
            }
            else
                s.append('.');
        }
        tmp = spark_info->l_array[index] % g;
        g = g / 10;
        tmp = tmp / g;
        tmp = abs(tmp);
        ch = tmp & 0xFF;
        ch = ch + 48;

        if(ch != '0'){
            first =true;
        }

        if(first)
            s.append(ch);
    }

    value = s;
    update();
}

QString Qcombox::getValue()
{
    return value;
}
