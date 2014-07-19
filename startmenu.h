#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>

namespace Ui {
class StartMenu;
}

class StartMenu : public QWidget
{
    Q_OBJECT
    
public:
    explicit StartMenu(QWidget *parent = 0);
    ~StartMenu();
    
private:
    Ui::StartMenu *ui;
};

#endif // STARTMENU_H
