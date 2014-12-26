#ifndef HOMEDIALOG_H
#define HOMEDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPushButton>
#include <QIcon>
#include "sparkinfo.h"

namespace Ui {
class HomeDialog;
}

class HomeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HomeDialog(QWidget *parent = 0);
    ~HomeDialog();
    
protected:
    void keyPressEvent(QKeyEvent *);

private:
    Ui::HomeDialog *ui;
    QIcon ok_icon;
    QIcon cancel_icon;

public slots:
    void Up();
    void Down();
};

#endif // HOMEDIALOG_H
