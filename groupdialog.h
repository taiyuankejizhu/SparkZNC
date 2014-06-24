#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QMouseEvent>

namespace Ui {
    class GroupDialog;
}

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDialog(QWidget *parent = 0);
    ~GroupDialog();

protected:
    void keyPressEvent(QKeyEvent *);

private:
    Ui::GroupDialog *ui;
};

#endif // GROUPDIALOG_H
