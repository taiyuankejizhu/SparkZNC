#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include "mesgbox.h"
#include "toplevel.h"
#include "seconlevel_f3.h"
#include "seconlevel_f4.h"
#include "seconlevel_f5.h"
#include "seconlevel_f6.h"
#include "keyboard.h"
#include "qcommand.h"
#include "sparkthread.h"
#include "scanthread.h"
#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QKeyEvent>
#include <QMap>
#include <QPushButton>
#include <QFile>
#include <QAbstractItemView>
#include <QItemSelectionModel>

#include "fcntl.h"
#include "sys/ioctl.h"
#include "fm25v02.h"
#include "fpga.h"
#include "setting.h"

#define TABLE_ROWS 10
#define TABLE_COLUMN 11

#define TABLE_SHOW 0x00
#define TABLE_EDIT 0x01
#define TABLE_SELECT 0x02
#define TABLE_DELETE 0x03
#define TABLE_ADD   0x04

#define UNOK_COLOR QColor(0xFF ,0x00 ,0x00)
#define OK_COLOR QColor(0x00 ,0xFF ,0x00)
#define SELECT_COLOR QColor(0xFF ,0xCC ,0x00)
#define UNSELECT_COLOR QColor(0xFF ,0xFF ,0xFF)

namespace Ui {
    class MainInterface;
}

class MainInterface : public QWidget
{
    Q_OBJECT

public:
    explicit MainInterface(QWidget *parent = 0);
    void initFuncBar();
    void initHardware();
    void submitTable();
    QString toString(long);
    QStandardItemModel *model;
    /*标志数据表格的状态，0x00显示态，0x01数据编辑态，0x02段选态,0x03行删除态,0x04行增加态*/
    char table_state;
    KeyBoard *keyboard;
    MesgBox *mesg;
    QCommand *command;
    /*放电加工线程*/
    SparkThread *spark;
    /*数据扫描和运行时线程*/
    ScanThread *scan;
    ~MainInterface();

private:
    /*标志段选开始行和结束行*/
    bool start_or_end;
    /*标志键盘是否已经被按下*/
    bool key_pressed;
    Ui::MainInterface *ui;
    void keyPressEvent( QKeyEvent *k );
    void keyReleaseEvent( QKeyEvent *k );
    QWidget *barui;
    QMap<int , QWidget *> barmap;

signals:
    void barF0();

public slots:
    void commandFinish();
    void commandSwitch(char ,char ,char ,char);
    void tableSelect(bool);
    void tableAddRow(int);
    void tableStateUpdate(char);
    void tableDataUpdate();
    void tableRollUpdate();
    void tableItemChange(QModelIndex tl ,QModelIndex br);
    void funcbarUpdate(int);
    void XYZ_Update(int);
};

#endif // MAININTERFACE_H
