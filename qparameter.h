#ifndef QPARAMETER_H
#define QPARAMETER_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QFontMetricsF>
#include <QApplication>
#include <QPoint>

class Qparameter : public QWidget
{
    Q_OBJECT
public:
    explicit Qparameter(QWidget *parent = 0);
    explicit Qparameter(QWidget *parent = 0 ,QString l = "" ,unsigned int maxh = 255 ,unsigned int minh = 0,unsigned int ih = 0,unsigned int maxt = 255 ,unsigned int mint = 0,unsigned int it = 0);
    unsigned int getValueH();
    unsigned int getValueT();

    unsigned int value_h;
    unsigned int value_t;

    /*数据在数组中的索引*/
    unsigned int index_h;
    unsigned int index_t;

    unsigned int max_value_h;
    unsigned int min_value_h;

    unsigned int max_value_t;
    unsigned int min_value_t;

protected:
    void drawBackground(QPainter *painter);
    void drawWidget(QPainter *painter);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    int     pad;
    QString label;
    QColor v_color;
    QColor b_color;
    QColor r_color;
    QColor l_color;
    QColor d_color;
    QColor z_color;

signals:

public slots:
    void updateValue();

};

#endif // QPARAMETER_H
