#ifndef COORDIALOG_H
#define COORDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QIcon>

namespace Ui {
    class CoorDialog;
}

class CoorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CoorDialog(QWidget *parent = 0);
    unsigned int selected;
    void updateContent();
    QString toString(long);
    QStandardItemModel *model;
    ~CoorDialog();

protected:
    void keyPressEvent(QKeyEvent *);

private:
    Ui::CoorDialog *ui;
    QIcon a_icon;
    QIcon b_icon;
    QIcon c_icon;
};

#endif // COORDIALOG_H
