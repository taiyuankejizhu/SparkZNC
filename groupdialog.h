#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QIcon>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QBrush>
#include <QColor>

#define COLUMN_COUNT 11

#define EXIST_COLOR QColor(0x00 ,0xFF ,0x00)
#define UNEXIST_COLOR QColor(0xFF ,0xFF ,0xFF)

namespace Ui {
    class GroupDialog;
}

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDialog(QWidget *parent = 0);
    unsigned int selected;
    QStandardItemModel *model;
    void updateContent();
    bool checkExists(unsigned int);
    bool createFile(unsigned int);
    ~GroupDialog();

protected:
    void keyPressEvent(QKeyEvent *);

private:
    Ui::GroupDialog *ui;
    QIcon a_icon;
    QIcon b_icon;
    QIcon ok_icon;
    QIcon cancel_icon;

public slots:
    void itemChange(QModelIndex);
};

#endif // GROUPDIALOG_H
