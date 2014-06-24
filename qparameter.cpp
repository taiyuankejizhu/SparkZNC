#include "qparameter.h"
#include "sparkinfo.h"
#include "qdebug.h"

Qparameter::Qparameter(QWidget *parent) :
    QWidget(parent)
{
    pad   = 3;
    value_h = 0;
    value_t = 0;
    max_value_h = 255;
    min_value_h = 0;
    max_value_t = 255;
    min_value_t = 0;
    label = "Null";
    v_color = QColor(255,204,0);
    b_color = QColor(255,255,255);
    l_color = QColor(0,0,0);
    r_color = QColor(64,64,64);
    z_color = QColor(128,128,128);
    d_color = QColor(0,0,0);
}

Qparameter::Qparameter(QWidget *parent ,QString l ,unsigned int maxh ,unsigned int minh ,unsigned int ih,unsigned int maxt ,unsigned int mint ,unsigned int it):
    QWidget(parent)
{
    pad   = 3;

    max_value_h = maxh;
    min_value_h = minh;

    max_value_t = maxt;
    min_value_t = mint;

    label = l;
    index_h = ih;
    index_t = it;
    v_color = QColor(64,64,64);
    b_color = QColor(255,255,255);
    l_color = QColor(255,255,255);
    r_color = QColor(64,64,64);
    z_color = QColor(128,128,128);
    d_color = QColor(0,0,0);

    updateValue();
    //setMinimumSize(0 , 60);
}

void Qparameter::drawBackground(QPainter *painter)
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

    /*画分割线*/
    painter->setPen(r_color);
    painter->setBrush(r_color);
    painter->drawLine(QPoint(width()/2 , height()/2) ,QPoint(width()/2 , height()));

    painter->restore();
}

void Qparameter::drawWidget(QPainter *painter)
{
    painter->save();

    painter->setPen(v_color);
    painter->setBrush(v_color);
    painter->fillRect(pad,pad,(width()-2*pad),height()/2-pad,v_color);

    /*画标签*/
    painter->setPen(l_color);
    painter->setBrush(l_color);

    QFont font = qApp->font();
    font.setPointSize(12);
    painter->setFont(font);

    QFontMetricsF fm(font);
    double w = fm.size(Qt::TextSingleLine,label).width();
    double h = fm.size(Qt::TextSingleLine,label).height();

    painter->drawText(width()/2-w/2 ,height()/4+h/2-pad ,label);

    /*画数据*/
    painter->translate(width()/4,height()*3/4);
    painter->setPen(d_color);
    painter->setBrush(d_color);

    QString strh = "H:    ";
    strh.append(QString::number(value_h));

    w = fm.size(Qt::TextSingleLine,strh).width();
    h = fm.size(Qt::TextSingleLine,strh).height();
    painter->drawText(-w/2 ,h/2-pad ,strh);

    painter->translate(width()/2,0);
    QString strt = "T:    ";
    strt.append(QString::number(value_t));

    w = fm.size(Qt::TextSingleLine,strt).width();
    h = fm.size(Qt::TextSingleLine,strt).height();
    painter->drawText(-w/2 ,h/2-pad ,strt);

    painter->restore();
}

void Qparameter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    drawBackground(&painter);
    drawWidget(&painter);
}

void Qparameter::mousePressEvent(QMouseEvent *ev)
{
    ev->accept();
    update();
}

void Qparameter::mouseReleaseEvent(QMouseEvent *ev)
{
    ev->accept();
    update();
}

void Qparameter::updateValue()
{
    value_h = spark_info->uint_array[index_h];
    if(value_h > max_value_h)
        value_h = max_value_h;
    if(value_h < min_value_h)
        value_h = min_value_h;

    value_t = spark_info->uint_array[index_t];
    if(value_t > max_value_t)
        value_t = max_value_t;
    if(value_t < min_value_t)
        value_t = min_value_t;
    update();
}

unsigned int Qparameter::getValueH()
{
    return value_h;
}

unsigned int Qparameter::getValueT()
{
    return value_t;
}
