#include "groupdialog.h"
#include "sparkinfo.h"
#include "setting.h"
#include "ui_groupdialog.h"
#include "qdebug.h"

GroupDialog::GroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupDialog)
{
    ui->setupUi(this);
    this->setGeometry(0 ,390 ,width() ,height());
    this->setWindowFlags(Qt::FramelessWindowHint);

    QPixmap a_pix = QPixmap(":/selected.png");
    a_icon = QIcon(a_pix);
    QPixmap b_pix = QPixmap(":/unselected.png");
    b_icon = QIcon(b_pix);

    QPixmap c_pix = QPixmap(":/table.png");

    QPixmap ok_pix = QPixmap(":/ok.png");
    ok_icon = QIcon(ok_pix);
    QPixmap cancel_pix = QPixmap(":/cancel.png");
    cancel_icon = QIcon(cancel_pix);

    selected = spark_info->uint_array[UINT_COOR_INDEX];
    model = new QStandardItemModel(0 ,1);
    updateContent();

    connect(ui->tableView ,SIGNAL(doubleClicked(QModelIndex)) ,this ,SLOT(itemChange(QModelIndex)));
    ui->label->setPixmap(c_pix);
    ui->buttonBox->button(ui->buttonBox->Ok)->setText(tr("确定(O)"));
    ui->buttonBox->button(ui->buttonBox->Ok)->setIcon(ok_icon);
    ui->buttonBox->button(ui->buttonBox->Ok)->setIconSize(QSize(32,32));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setText(tr("取消(C)"));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIcon(cancel_icon);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIconSize(QSize(32,32));
}

void GroupDialog::keyPressEvent(QKeyEvent *k)
{
    k->accept();
    switch(k->key())
    {
    case Qt::Key_Down:
    case Qt::Key_Left:
        focusNextChild();
        break;
    case Qt::Key_Up:
    case Qt::Key_Right:
        focusPreviousChild();
        break;
    case Qt::Key_F1:
        finished(1);
        break;
    case Qt::Key_F3:
    case Qt::Key_F2:
    case Qt::Key_F4:
    case Qt::Key_F5:
    case Qt::Key_F6:
    case Qt::Key_F7:
    case Qt::Key_F8:
        finished(0);
    case Qt::Key_Enter:
        selected = ui->tableView->currentIndex().row()*COLUMN_COUNT + ui->tableView->currentIndex().column();
        updateContent();
        break;
    default:
        break;
    }
}

void GroupDialog::itemChange(QModelIndex i)
{
    selected = i.row()*COLUMN_COUNT + i.column();
    updateContent();
    finished(1);
}

void GroupDialog::updateContent()
{
    unsigned int i = 0 ,j = 0;
    int k = 0 , y = 0;
    QString value ;

    model->clear();
    model->setColumnCount(COLUMN_COUNT);

    for(i = 0;i < spark_info->uint_array[UINT_TAB_MAX];i += COLUMN_COUNT){
        QList<QStandardItem*> items;
        for(j = 0;j < COLUMN_COUNT;j++){
            value.clear();
            value.append(QString::number(i + j));

            QStandardItem* item = new QStandardItem();
            if(i + j == selected)
                item->setIcon(a_icon);
            else
                item->setIcon(b_icon);
            QBrush brush;
            if(checkExists(i + j))
                brush = QBrush(EXIST_COLOR);
            else
                brush = QBrush(UNEXIST_COLOR);
            item->setBackground(brush);
            item->setText(value);
            items<<item;
        }
        model->appendRow(items);
    }

    QModelIndex start;
    start = model->index(selected / COLUMN_COUNT,selected % COLUMN_COUNT);
    ui->tableView->setCurrentIndex(start);

    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableView->setModel(model);

    for(k = 0;k < model->columnCount();k++){
        ui->tableView->setColumnWidth(k ,74);
    }
    for(y = 0;y < model->rowCount();y++){
        ui->tableView->setRowHeight(y ,24);
    }
}

/*检查表格数据是否已经存盘*/
bool GroupDialog::checkExists(unsigned int i)
{
    bool bit = false, ok = false;
    int f = 0;

    QDir dir(QDir::currentPath()+TABLES_DIR);
    if(!dir.exists()){
        dir.mkpath(dir.absolutePath());
    }
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();

    f = 0;
    while(f < list.size()){
        QFileInfo file = list.at(f);
        QString name = file.fileName();
        if(i == name.toUInt(&ok ,10)){
            bit = true;
            break;
        }
        else
            bit = false;
        f++;
    }

    return bit;
}

bool GroupDialog::createFile(unsigned int i)
{
    bool state;
    QByteArray bytes;
    char zero = 0x00;
    bytes.resize(sizeof(Table));
    memcpy(bytes.data() ,&zero ,sizeof(Table));

    QFile file(QDir::currentPath()+TABLES_DIR+QString::number(i));
    state = file.open(QIODevice::WriteOnly);
    file.close();

    return state;
}

GroupDialog::~GroupDialog()
{
    delete ui;
}
