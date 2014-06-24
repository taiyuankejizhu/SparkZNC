#ifndef COORDIALOG_H
#define COORDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QMouseEvent>

namespace Ui {
    class CoorDialog;
}

class CoorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CoorDialog(QWidget *parent = 0);
    ~CoorDialog();

protected:
    void keyPressEvent(QKeyEvent *);

private:
    Ui::CoorDialog *ui;
};

#endif // COORDIALOG_H
