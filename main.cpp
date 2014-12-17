#include <QApplication>
#include <QtGui>
#include <QtGlobal>
#include <QFont>
#include <QTranslator>
#include <QProcess>
#include <QFile>
#include "maininterface.h"
#include "sparkinfo.h"
#include "myinputpanelcontext.h"
#include "keyboard.h"
#include "setting.h"
#include "calibration.h"
#include "qdebug.h"

/*定义全局变量SparkInfo，保存运行参数*/
SparkInfo *spark_info = new SparkInfo();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setCursorFlashTime(1);

#ifndef Q_WS_X11
    QFile file;
    file.setFileName(TSLIB_CALIBFILE);
    /*屏幕没有校验*/
    if(!file.exists()){
        Calibration cal;
        cal.exec();
    }
#endif

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    /*从命令行中设置系统语言*/
    QTranslator translator;
    bool b = false;
    b = translator.load(":/"+QString(argv[1])+".qm");
    if(b)
        a.installTranslator(&translator);
/*
    a.setStyleSheet(
        "QPushButton{border:3px solid rgb(0,0,0); background:rgb(255,255,255);border-radius: 10px;}"
        "QPushButton:hover{border-color:rgb(96,96,96); background:rgb(128,128,128);}"
        "QPushButton:focus{border-color:rgb(96,96,96); background:rgb(128,128,128);}"
        "QPushButton:checked{border-color:rgb(255,255,255);background:rgb(128,128,128);}"
        "QHeaderView::section{background-color:rgb(128,128,128);color:white;border:1px solid #6c6c6c;}"
        "QTableView::item:selected{background-color:rgb(255,206,0);}"
        "QTableView::item:focus{background-color:rgb(255,206,0);}"
                    );
*/
/*
    a.setStyleSheet(
        "QPushButton{border-image:url(:/nomal.png); color: black;}"
        "QPushButton:checked{border-image:url(:/press.png); color: black;}"
        "QPushButton:hover{border-image:url(:/hover.png); color: black;}"
        "QPushButton:pressed{border-image:url(:/press.png); color: black;}"
        "QHeaderView::section{background-color:rgb(128,128,128);color:white;border:1px solid #6c6c6c;}"
        "QTableView::item:selected{background-color:rgb(255,206,0);}"
        "QTableView::item:focus{background-color:rgb(255,206,0);}"
                    );
*/

    a.setStyleSheet(
        "QPushButton{border-image:url(:/nomal.png); color: black;}"
        "QPushButton:checked{border-image:url(:/press.png); color: black;}"
        "QPushButton:pressed{border-image:url(:/press.png); color: black;}"
        "QHeaderView::section{background-color:rgb(128,128,128);color:white;border:1px solid #6c6c6c;}"
        "QTableView::item:selected{background-color:rgb(255,206,0);}"
        "QTableView::item:focus{background-color:rgb(255,206,0);}"
                    );

    QFont font;
    font.setFamily("SparkFont");
    a.setFont(font);

    MainInterface m;

    qApp->installEventFilter(&m);

#ifndef Q_WS_X11
    QRect screen_size = QApplication::desktop()->screenGeometry();
    m.setGeometry(0, 0, screen_size.width(), screen_size.height());
    m.setCursor(QCursor(Qt::BlankCursor));
#endif

    /*软键盘输入上下文*/
    MyInputPanelContext *ic = new MyInputPanelContext(&m);
    a.setInputContext(ic);

    m.show();

    return a.exec();
}
