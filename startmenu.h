#ifndef STARTMENU_H
#define STARTMENU_H

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
    unsigned int selected;
    ~StartMenu();
    
protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent( QKeyEvent *k );
    void updateSelect();
    void updateIcon();

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
