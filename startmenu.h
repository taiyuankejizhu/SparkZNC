#ifndef STARTMENU_H
#define STARTMENU_H

#include "cleardialog.h"
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
    void finish();
public slots:
    void selectItem();

private:
    Ui::StartMenu *ui;
    /*菜单选项的触发定时器*/
    QTimer *timer;
};

#endif // STARTMENU_H
