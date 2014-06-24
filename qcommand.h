#ifndef QCOMMAND_H
#define QCOMMAND_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QFontMetricsF>
#include <QApplication>
#include <QTimer>
#include <QColor>
#include <QMap>

#define MESG_COLOR Qt::blue
#define INPUT_COLOR Qt::black
#define WARNING_COLOR Qt::darkMagenta
#define ALTER_COLOR Qt::red

class QCommand : public QWidget
{
    Q_OBJECT
public:
    explicit QCommand(QWidget *parent = 0);
    void setStatus(char);
    void setTarget(char,char);
    void setType(char);
    void setType(bool ,bool ,char ,bool ,char);
    QTimer *timer ;
    QString label ;            /*命令行的标题*/
    QString value ;            /*命令行的实际数据*/
    QString mesg ;             /*命令行的提示信息*/
    int wr_index;              /*命令行输入记录的写入索引*/
    int rd_index;              /*命令行输入记录的读出索引*/
    QMap<int, QString> record; /*命令行输入记录*/

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
    void checkString(QString);
    void recordAppend(QString);
    void initLabel();
    void showMesg(char , int);
    void finalSubmit(QString);

    /*标志光标闪烁*/
    bool flag;

    /*标志命令栏的状态，0x00提示状态，0x10为输入状态 ,0x20为警告状态 ,0x30为报警状态*/
    char status;

    /*
    标志输入的数据类型，x*******置1表示输入字符串，*x******置1为有符号置0为无符号，*****x**置1为整数置0为小数
    **xxx***中间三位标志整数有效数的位数，******xx后两位标志小数有效位
    */
    char type;

    /*
    数据的目标位置,****xxxx前四位标志目标槽，0001表示uint槽，0010表示long槽，其他保留，
    后四位表示目标索引，即在数组中的索引
    */
    char target;
private:
    QString status_label[5];
    QString mesg_label[10];

signals:
    void finish();

public slots:
    void flashCursor();
};

#endif // QCOMMAND_H
