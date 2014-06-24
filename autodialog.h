#ifndef AUTODIALOG_H
#define AUTODIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QMouseEvent>
#include <QKeyEvent>

namespace Ui {
    class AutoDialog;
}

class AutoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoDialog(QWidget *parent = 0);
    long deep;
    unsigned int current;
    unsigned int area;
    unsigned int effect;
    ~AutoDialog();

protected:
    void keyPressEvent(QKeyEvent *);

public slots:
    void valueChange();

private:
    Ui::AutoDialog *ui;
};

#endif // AUTODIALOG_H
