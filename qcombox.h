#ifndef QCOMBOX_H
#define QCOMBOX_H

#include <QWidget>

class Qcombox : public QWidget
{
    Q_OBJECT
public:
    explicit Qcombox(QWidget *parent = 0 ,QString l = "",unsigned int i = 0);
    QString getValue();
    QString value;
    unsigned int index;

protected:
    void drawBackground(QPainter *painter);
    void drawWidget(QPainter *painter);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    int     pad;
    QString label;
    QColor f_color;
    QColor b_color;
    QColor r_color;
    QColor l_color;
    QColor z_color;

signals:

public slots:
    void updateValue();
};

#endif // Qcombox_H
