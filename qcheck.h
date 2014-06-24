#ifndef QMYCHECK_H
#define QMYCHECK_H

#include <QWidget>

class Qcheck : public QWidget
{
    Q_OBJECT
public:
    explicit Qcheck(QWidget *parent = 0);
    explicit Qcheck(QWidget *parent = 0 ,QString l = "",unsigned int i = 0);
    bool check;
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
    QColor t_color;
    QColor r_color;
    QColor l_color;
    QColor z_color;
    //QPixmap pix;
signals:

public slots:
    void updateCheck();
};

#endif // QMYCHECK_H
