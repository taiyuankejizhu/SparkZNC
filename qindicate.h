#ifndef QINDICATE_H
#define QINDICATE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QFontMetricsF>
#include <QApplication>

class Qindicate : public QWidget
{
    Q_OBJECT
public:
    explicit Qindicate(QWidget *parent = 0);
    explicit Qindicate(QWidget *parent = 0 ,QString l = "" ,unsigned int max = 255 ,unsigned int min = 0,unsigned int i = 0);
    unsigned int getValue();

    unsigned int value;
    /*数据在数组中的索引*/
    unsigned int index;
    unsigned int max_value;
    unsigned int min_value;

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
    QColor z_color;

signals:

public slots:
    void updateValue();

};

#endif // QINDICATE_H
