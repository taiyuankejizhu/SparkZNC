#ifndef STARTMENU_H
#define STARTMENU_H

#include "cleardialog.h"
#include "keydialog.h"
#include "settingdialog.h"
#include "calibration.h"
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>

namespace Ui {
class StartMenu;
}

class StartMenu : public QWidget
{
    Q_OBJECT
    
public:
    explicit StartMenu(QWidget *parent = 0);
    ClearDialog *clear;
    KeyDialog *key;
    SettingDialog *setting;
    unsigned int selected;
    ~StartMenu();
    
protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent( QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void updateSelect();
    void updateIcon();
    void doFnPress(int);
    void doFnRelease(int);

signals:
    void finish(bool);
public slots:
    void selectItem();
    void dialogFinish(int);

private:
    Ui::StartMenu *ui;
    /*菜单选项的触发定时器*/
    QTimer *timer;
};

#endif // STARTMENU_H
