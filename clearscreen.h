#ifndef CLEARSCREEN_H
#define CLEARSCREEN_H

#include <QWidget>

namespace Ui {
class ClearScreen;
}

class ClearScreen : public QWidget
{
    Q_OBJECT
    
public:
    explicit ClearScreen(QWidget *parent = 0);
    ~ClearScreen();
    
private:
    Ui::ClearScreen *ui;
};

#endif // CLEARSCREEN_H
