#include "maininterface.h"
#include "ui_maininterface.h"
#include "qdebug.h"

/*数据表格中的深度栏自动排序规则*/
class QuoteItem: public QStandardItem
{
public:
    QuoteItem(){}
    QuoteItem(const QString &text):QStandardItem(text)
    {
    }
    QuoteItem(const QuoteItem &other):QStandardItem(other)
    {
    }
    QuoteItem &operator=(const QuoteItem &other)
    {
        QStandardItem::operator=(other);
        return *this;
    }
    virtual bool operator<(const QStandardItem &other) const
    {
        const QVariant l = data(Qt::DisplayRole), r = other.data(Qt::DisplayRole);
        if (column() == other.column())
        {
            return l.toDouble() < r.toDouble();
        }
        return QStandardItem::operator<(other);
    }
};

MainInterface::MainInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainInterface)
{
    ui->setupUi(this);

    key_pressed = false;
    start_or_end = true;

    target = 0;

    FPGA_Init();

    XYZ_Update(L_X_CURRENT);
    XYZ_Update(L_Y_CURRENT);
    XYZ_Update(L_Z_CURRENT);

    XYZ_Update(L_X_ABSOLUTE);
    XYZ_Update(L_Y_ABSOLUTE);
    XYZ_Update(L_Z_ABSOLUTE);

    XYZ_Update(L_X_REMAIN);
    XYZ_Update(L_Y_REMAIN);
    XYZ_Update(L_Z_REMAIN);

    axisIndexUpdate();

    connect(spark_info ,SIGNAL(xyzChange(int)) ,this ,SLOT(XYZ_Update(int)));
    connect(spark_info ,SIGNAL(coorIndexChange()) ,this ,SLOT(axisIndexUpdate()));

    model = new QStandardItemModel;
    /*数据表索引发生改变*/
    connect(spark_info ,SIGNAL(tableChange()) ,this ,SLOT(tableDataUpdate()));
    /*数据表的加工段和当前加工行发生改变*/
    connect(spark_info ,SIGNAL(tableRowChange()),this ,SLOT(tableRollUpdate()));

    /*初始化功能栏*/
    initFuncBar();

    /*初始化为显示态*/
    tableStateUpdate(TABLE_SHOW);

    spark = new SparkThread();
    connect(spark_info ,SIGNAL(startChange()) ,spark ,SLOT(sparkChange()));

    scan = new ScanThread();
    scan->start();
    connect(this ,SIGNAL(keyPress(int)) ,scan ,SLOT(keyPress(int)));
    connect(this ,SIGNAL(keyRelease(int)) ,scan ,SLOT(keyRelease(int)));
    connect(scan ,SIGNAL(cursor(bool)) ,this ,SLOT(cursorSwitch(bool)));

    mesg = new MesgBox(this);
    mesg ->setHidden(false);

    menu = new StartMenu(this);
    menu ->setHidden(true);
    connect(menu ,SIGNAL(finish(bool)) ,this ,SLOT(menuShow(bool)));

    timer = new QTimer(this);
    timer->setInterval(300);
    connect(timer ,SIGNAL(timeout()) ,this ,SLOT(menuTimeout()));

    ui->verticalLayout->addWidget(mesg);
    ui->verticalLayout->addWidget(menu);

    command = new QCommand(this);
    command->setHidden(false);
    /*命令行输入结束时的信号连接*/
    connect(command ,SIGNAL(finish()) ,this ,SLOT(commandFinish()));

    alerts = new QAlerts(this);
    alerts->setHidden(false);
    connect(spark_info ,SIGNAL(boolChange()) ,alerts ,SLOT(alertCheck()));

    clear = new ClearScreen(parent);
    clear->setHidden(true);

#ifndef Q_WS_X11
    QRect screen_size = qApp->desktop()->screenGeometry();
    clear->setGeometry(0, 0, screen_size.width(), screen_size.height());
    clear->setCursor(QCursor(Qt::BlankCursor));

    watcher = new QFileSystemWatcher(this);
    watcher->addPath(DEV_DIR);
    connect(watcher ,SIGNAL(directoryChanged(QString)) ,this ,SLOT(mouseChange(QString)));
    is_usb = false;

    if(!QFile::exists(USB_MOUSE)){
        setCursor(QCursor(Qt::BlankCursor));
    }else{
        setCursor(QCursor(Qt::ArrowCursor));
    }
#else
    clear->setGeometry(0 ,0 ,1024 ,768);
#endif
    connect(scan ,SIGNAL(clear()) ,clear ,SLOT(show()));

    ui->horizontalLayout_3->addWidget(command);
    ui->horizontalLayout_4->addWidget(alerts);
}

void MainInterface::initFuncBar()
{
    toplevel *top ;
    seconlevel_f3 *sec_f3;
    seconlevel_f4 *sec_f4;
    seconlevel_f5 *sec_f5;
    seconlevel_f6 *sec_f6;

    top = new toplevel(this);
    top->setHidden(true);
    connect(top ,SIGNAL(menu(bool)) ,this ,SLOT(menuShow(bool)));

    sec_f3 = new seconlevel_f3(this);
    sec_f3 ->setHidden(true);

    sec_f4 = new seconlevel_f4(this);
    sec_f4 ->setHidden(true);

    sec_f5 = new seconlevel_f5(this);
    sec_f5 ->setHidden(true);

    sec_f6 = new seconlevel_f6(this);
    sec_f6 ->setHidden(true);

    barmap.clear();
    barmap.insert(23 ,sec_f3);
    barmap.insert(24 ,sec_f4);
    barmap.insert(25 ,sec_f5);
    barmap.insert(26 ,sec_f6);
    barmap.insert(28 ,top);

    barui = barmap[28];

    barui->setHidden(false);
    ui->horizontalLayout->addWidget(barui);
    connect(this ,SIGNAL(barF0()) ,barui ,SLOT(F0()));
    emit barF0();
}

void MainInterface::keyPressEvent( QKeyEvent *k )
{
    QPalette palette;
    QColor color(255 ,0 ,0);
    palette.setColor(QPalette::WindowText,color);

    /*去除按键重复事件*/
    if(!k->isAutoRepeat()){
        k->accept();
        QPushButton *Fn ;

        switch(k->key())
        {
        case Qt::Key_F1:
            Fn = barui->findChild<QPushButton *> ("pushButton_F1");
            if(Fn != NULL)
                Fn->click();
            break;
        case Qt::Key_F2:
            Fn = barui->findChild<QPushButton *> ("pushButton_F2");
            if(Fn != NULL)
                Fn->click();
            break;
        case Qt::Key_F3:
            Fn = barui->findChild<QPushButton *> ("pushButton_F3");
            if(Fn != NULL)
                Fn->click();
            break;
        case Qt::Key_F4:
            Fn = barui->findChild<QPushButton *> ("pushButton_F4");
            if(Fn != NULL)
                Fn->click();
            break;
        case Qt::Key_F5:
            Fn = barui->findChild<QPushButton *> ("pushButton_F5");
            if(Fn != NULL)
                Fn->click();
            break;
        case Qt::Key_F6:
            Fn = barui->findChild<QPushButton *> ("pushButton_F6");
            if(Fn != NULL)
                Fn->click();
            break;
        case Qt::Key_F7:
            Fn = barui->findChild<QPushButton *> ("pushButton_F7");
            if(Fn != NULL)
                Fn->click();
            break;
        case Qt::Key_F8:
            Fn = barui->findChild<QPushButton *> ("pushButton_F8");
            if(Fn != NULL){
                if(barmap.key(barui) != 28){
                    Fn->click();
                }else{
                    if(!timer->isActive())
                        timer->start();
                }
            }
            break;
        case Qt::Key_X:
            ui->label_cux->setPalette(palette);

            command ->setFocus();
            command ->setStatus(0x10);
            command ->setType(false ,true ,0x04 ,true ,0x03);
            target = L_SLOT << 8 | L_X_CURRENT;
            command ->setTarget(L_SLOT ,L_X_CURRENT);

            break;
        case Qt::Key_Y:
            ui->label_cuy->setPalette(palette);

            command ->setFocus();
            command ->setStatus(0x10);
            command ->setType(false ,true ,0x04 ,true ,0x03);
            target = L_SLOT << 8 | L_Y_CURRENT;
            command ->setTarget(L_SLOT ,L_Y_CURRENT);

            break;
        case Qt::Key_Z:
            ui->label_cuz->setPalette(palette);

            command ->setFocus();
            command ->setStatus(0x10);
            command ->setType(false ,true ,0x04 ,true ,0x03);
            target = L_SLOT << 8 | L_Z_CURRENT;
            command ->setTarget(L_SLOT ,L_Z_CURRENT);

            break;
        case Qt::Key_F9:
        case Qt::Key_F10:
            break;
        /*放电监听F11键*/
        case Qt::Key_F11:
            spark_info->setBool(B_START ,false);
            break;
        case Qt::Key_F12:
            spark_info->setBool(B_START ,true);
            break;
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
            emit keyPress(k->key());
            break;
        /*这里的Enter键用来响应放电数据表的段选和删除行*/
        case Qt::Key_Enter:
            tableRowClick(ui->tableView->currentIndex());
            break;
        default :
            break;
        }
    }
    else
        k->ignore();
}

void MainInterface::keyReleaseEvent(QKeyEvent *k)
{

    if(!k->isAutoRepeat()){
        k->accept();

        switch(k->key())
        {
        case Qt::Key_F1:
            break;
        case Qt::Key_F2:
    //        ui->pushButton_F2->setEnabled(true);
            break;
        case Qt::Key_F3:
    //        ui->pushButton_F3->setEnabled(true);
            break;
        case Qt::Key_F4:
    //        ui->pushButton_F4->setEnabled(true);
            break;
        case Qt::Key_F5:
    //        ui->pushButton_F5->setEnabled(true);
            break;
        case Qt::Key_F6:
    //        ui->pushButton_F6->setEnabled(true);
            break;
        case Qt::Key_F7:
    //        ui->pushButton_F7->setEnabled(true);
            break;
        case Qt::Key_F8:
            if(timer->isActive()){
                timer->stop();
            }
            break;
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Left:
        case Qt::Key_Right:
            emit keyRelease(k->key());
            break;
        default :
            break;
        }
    }
    else
        k->ignore();
}

/*全局时间过滤器，用于监听按键鼠标动作，唤醒屏幕和点亮蜂鸣器*/
bool MainInterface::eventFilter(QObject *o, QEvent *e)
{
    QPalette palette;
    QColor color(255 ,0 ,0);

    if(e->type() == QEvent::KeyPress){
        scan->b_cycle = 0;
        if((spark_info->uint_array[UINT_BRIGHTNESS] & 0xf0) != 0){
            spark_info->setUInt(UINT_BRIGHTNESS ,spark_info->uint_array[UINT_BRIGHTNESS] & 0x0f);
            clear->hide();
            setFocus();
        }
    }else if(e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseMove){
        scan->b_cycle = 0;
        if((spark_info->uint_array[UINT_BRIGHTNESS] & 0xf0) != 0){
            spark_info->setUInt(UINT_BRIGHTNESS ,spark_info->uint_array[UINT_BRIGHTNESS] & 0x0f);
            clear->hide();
            setFocus();
        }

        if(e->type() == QEvent::MouseButtonPress){
            if(o->objectName() == "label_cux"){
                qDebug()<<"x";
                if(target != (L_SLOT << 8 | L_X_CURRENT)){
                    color = QColor(255 ,0 ,0);
                    palette.setColor(QPalette::WindowText,color);
                    ui->label_cux->setPalette(palette);
                    color = QColor(24 ,93 ,255);
                    palette.setColor(QPalette::WindowText,color);
                    ui->label_cuy->setPalette(palette);
                    ui->label_cuz->setPalette(palette);
                }
                QApplication::sendEvent(command, new QEvent(QEvent::MouseButtonDblClick));

                command ->setFocus();
                command ->setStatus(0x10);
                command ->setType(false ,true ,0x04 ,true ,0x03);
                target = L_SLOT << 8 | L_X_CURRENT;
                command ->setTarget(L_SLOT ,L_X_CURRENT);
            }else if(o->objectName() == "label_cuy"){
                qDebug()<<"y";
                if(target != (L_SLOT << 8 | L_Y_CURRENT)){
                    color = QColor(255 ,0 ,0);
                    palette.setColor(QPalette::WindowText,color);
                    ui->label_cuy->setPalette(palette);
                    color = QColor(24 ,93 ,255);
                    palette.setColor(QPalette::WindowText,color);
                    ui->label_cux->setPalette(palette);
                    ui->label_cuz->setPalette(palette);
                }
                QApplication::sendEvent(command, new QEvent(QEvent::MouseButtonDblClick));

                command ->setFocus();
                command ->setStatus(0x10);
                command ->setType(false ,true ,0x04 ,true ,0x03);
                target = L_SLOT << 8 | L_Y_CURRENT;
                command ->setTarget(L_SLOT ,L_Y_CURRENT);
            }else if(o->objectName() == "label_cuz"){
                qDebug()<<"z";
                if(target != (L_SLOT << 8 | L_Z_CURRENT)){
                    color = QColor(255 ,0 ,0);
                    palette.setColor(QPalette::WindowText,color);
                    ui->label_cuz->setPalette(palette);
                    color = QColor(24 ,93 ,255);
                    palette.setColor(QPalette::WindowText,color);
                    ui->label_cux->setPalette(palette);
                    ui->label_cuy->setPalette(palette);
                }
                QApplication::sendEvent(command, new QEvent(QEvent::MouseButtonDblClick));

                command ->setFocus();
                command ->setStatus(0x10);
                command ->setType(false ,true ,0x04 ,true ,0x03);
                target = L_SLOT << 8 | L_Z_CURRENT;
                command ->setTarget(L_SLOT ,L_Z_CURRENT);
            }
        }
    }

    return qApp->eventFilter(o ,e);
}

void MainInterface::commandFinish()
{
    /*恢复输入之前的屏幕焦点*/
    /*tableStateUpdate(table_state);*/
    QPalette palette;
    QColor color(24 ,93 ,255);
    palette.setColor(QPalette::WindowText,color);

    ui->label_cux->setPalette(palette);
    ui->label_cuy->setPalette(palette);
    ui->label_cuz->setPalette(palette);

    target = 0;
}

/*鼠标显示与隐藏，由ScanThread激发*/
void MainInterface::cursorSwitch(bool b)
{

    if(b){
        setCursor(QCursor(Qt::ArrowCursor));
    }else{
        /*b 为false时隐藏鼠标*/
        setCursor(QCursor(Qt::BlankCursor));
    }

}

void MainInterface::mouseChange(QString s)
{

#ifndef Q_WS_X11
    if(s == DEV_DIR){
        qDebug()<<"MouseChange:"<<s;
        if(QFile::exists(USB_MOUSE) && !is_usb){
            QWSServer::setMouseHandler(QMouseDriverFactory::create("mouseman",USB_MOUSE));
            is_usb = true;
            setCursor(QCursor(Qt::ArrowCursor));
        }
        else{
            is_usb = false;
            setCursor(QCursor(Qt::BlankCursor));
        }
    }
#else
    qDebug()<<"DirChange:"<<s;
#endif

}

void MainInterface::commandSwitch(char status ,char type ,char slot,char index)
{
    command ->setFocus();

    command ->setStatus(status);
    command ->setType(type);
    command ->setTarget(slot ,index);
}

/*插入行*/
void MainInterface::tableAddRow(int i)
{
    i = model->rowCount() - 1;
    QuoteItem* item1 = new QuoteItem(toString(spark_info->table.Shendu[i]));
    item1->setTextAlignment(Qt::AlignCenter);
    QStandardItem* item2 = new QStandardItem(QString::number(spark_info->table.Dianliu[i]));
    item2->setTextAlignment(Qt::AlignCenter);
    QStandardItem* item3 = new QStandardItem(QString::number(spark_info->table.Maikuan[i]));
    item3->setTextAlignment(Qt::AlignCenter);
    QStandardItem* item4 = new QStandardItem(QString::number(spark_info->table.Xiuzhi[i]));
    item4->setTextAlignment(Qt::AlignCenter);
    QStandardItem* item5 = new QStandardItem(QString::number(spark_info->table.Jianxi[i]));
    item5->setTextAlignment(Qt::AlignCenter);
    QStandardItem* item6 = new QStandardItem(QString::number(spark_info->table.Sudu[i]));
    item6->setTextAlignment(Qt::AlignCenter);
    QStandardItem* item7 = new QStandardItem(QString::number(spark_info->table.Shenggao[i]));
    item7->setTextAlignment(Qt::AlignCenter);
    QStandardItem* item8 = new QStandardItem(QString::number(spark_info->table.Gongshi[i]));
    item8->setTextAlignment(Qt::AlignCenter);
    QStandardItem* item9 = new QStandardItem(QString::number(spark_info->table.Mianji[i]));
    item9->setTextAlignment(Qt::AlignCenter);
    QStandardItem* item10 = new QStandardItem(QString::number(spark_info->table.Jixing[i]));
    item10->setTextAlignment(Qt::AlignCenter);
    QStandardItem* item11 = new QStandardItem(QString::number(spark_info->table.Gaoya[i]));
    item11->setTextAlignment(Qt::AlignCenter);
    QList<QStandardItem*> item;
    item <<item1<<item2<<item3<<item4<<item5<<item6<<item7<<item8<<item9<<item10<<item11;
    model->appendRow(item);

    int width = 71;
    ui->tableView->setColumnWidth(0,width+width);
    ui->tableView->setColumnWidth(1,width);
    ui->tableView->setColumnWidth(2,width);
    ui->tableView->setColumnWidth(3,width);
    ui->tableView->setColumnWidth(4,width);
    ui->tableView->setColumnWidth(5,width);
    ui->tableView->setColumnWidth(6,width);
    ui->tableView->setColumnWidth(7,width);
    ui->tableView->setColumnWidth(8,width);
    ui->tableView->setColumnWidth(9,width);
    ui->tableView->setColumnWidth(10,width);

    for(int i=0 ;i < TABLE_ROWS;i++)
        ui->tableView->setRowHeight(i ,28);

    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setFixedHeight(17);
    //ui->tableView->setEnabled(false);

    ui->tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setFixedWidth(20);

}

void MainInterface::tableStateUpdate(char c)
{
    tableDataUpdate();

    int width = 71;
    ui->tableView->setColumnWidth(0,width+width);
    ui->tableView->setColumnWidth(1,width);
    ui->tableView->setColumnWidth(2,width);
    ui->tableView->setColumnWidth(3,width);
    ui->tableView->setColumnWidth(4,width);
    ui->tableView->setColumnWidth(5,width);
    ui->tableView->setColumnWidth(6,width);
    ui->tableView->setColumnWidth(7,width);
    ui->tableView->setColumnWidth(8,width);
    ui->tableView->setColumnWidth(9,width);
    ui->tableView->setColumnWidth(10,width);

    for(int i=0 ;i < TABLE_ROWS;i++)
        ui->tableView->setRowHeight(i ,28);

    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setFixedHeight(17);

    ui->tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->tableView->verticalHeader()->setFixedWidth(20);

    table_state = c;
    QModelIndex start;

    if(table_state == TABLE_SHOW){
        disconnect(model,0,this ,0);

        ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);
        ui->tableView->setFocusPolicy(Qt::NoFocus);
        ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
        setFocus();
        tableRollUpdate();
    }
    else if(table_state == TABLE_EDIT){
        disconnect(model,0,this ,0);

        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
        ui->tableView->setFocusPolicy(Qt::StrongFocus);
        ui->tableView->setEditTriggers(QTableView::CurrentChanged);
        start = ui->tableView->model()->index(0 ,0);
        ui->tableView->setCurrentIndex(start);
        ui->tableView->setFocus();

        connect(model ,SIGNAL(dataChanged(QModelIndex,QModelIndex)) ,this ,SLOT(tableItemChange(QModelIndex,QModelIndex)));
    }
    else if(table_state == TABLE_SELECT){
        disconnect(model,0,this ,0);
        disconnect(ui->tableView,0,this ,0);

        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setFocusPolicy(Qt::StrongFocus);
        ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
        ui->tableView->selectRow(0);
        ui->tableView->setFocus();

        tableRollUpdate();
        connect(ui->tableView ,SIGNAL(clicked(QModelIndex)) ,this ,SLOT(tableRowClick(QModelIndex)));
    }
    else if(table_state == TABLE_DELETE){
        disconnect(model,0,this ,0);
        disconnect(ui->tableView,0,this ,0);

        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setFocusPolicy(Qt::StrongFocus);
        ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
        ui->tableView->selectRow(0);
        ui->tableView->setFocus();

        tableRollUpdate();
        connect(ui->tableView ,SIGNAL(clicked(QModelIndex)) ,this ,SLOT(tableRowClick(QModelIndex)));
    }
    else if(table_state == TABLE_ADD){
        disconnect(model,0,this ,0);
        disconnect(ui->tableView,0,this ,0);

        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setFocusPolicy(Qt::StrongFocus);
        ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
        ui->tableView->selectRow(0);
        ui->tableView->setFocus();

        tableRollUpdate();
        connect(ui->tableView ,SIGNAL(clicked(QModelIndex)) ,this ,SLOT(tableRowClick(QModelIndex)));
    }
    else{
        disconnect(model,0,this ,0);
        disconnect(ui->tableView,0,this ,0);

        ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);
        ui->tableView->setFocusPolicy(Qt::NoFocus);
        ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
        setFocus();
        tableRollUpdate();
    }
}

void MainInterface::tableDataUpdate()
{
    model->clear();

    /*设置表头*/
    model->setColumnCount(TABLE_COLUMN);
    model->setHeaderData(0,Qt::Horizontal,tr("设定深度"));
    model->setHeaderData(1,Qt::Horizontal,tr("电流"));
    model->setHeaderData(2,Qt::Horizontal,tr("脉宽"));
    model->setHeaderData(3,Qt::Horizontal,tr("休止"));
    model->setHeaderData(4,Qt::Horizontal,tr("间隙"));
    model->setHeaderData(5,Qt::Horizontal,tr("伺服"));
    model->setHeaderData(6,Qt::Horizontal,tr("排渣"));
    model->setHeaderData(7,Qt::Horizontal,tr("工时"));
    model->setHeaderData(8,Qt::Horizontal,tr("面积"));
    model->setHeaderData(9,Qt::Horizontal,tr("极性"));
    model->setHeaderData(10,Qt::Horizontal,tr("高压"));

    for(int i=0 ;i < TABLE_ROWS;i++)
    {

        if(spark_info->table.Index[i] > 0){

            QuoteItem* item1 = new QuoteItem(toString(spark_info->table.Shendu[i]));
            item1->setTextAlignment(Qt::AlignCenter);

            QStandardItem* item2 = new QStandardItem(QString::number(spark_info->table.Dianliu[i]));
            item2->setTextAlignment(Qt::AlignCenter);
            QStandardItem* item3 = new QStandardItem(QString::number(spark_info->table.Maikuan[i]));
            item3->setTextAlignment(Qt::AlignCenter);
            QStandardItem* item4 = new QStandardItem(QString::number(spark_info->table.Xiuzhi[i]));
            item4->setTextAlignment(Qt::AlignCenter);
            QStandardItem* item5 = new QStandardItem(QString::number(spark_info->table.Jianxi[i]));
            item5->setTextAlignment(Qt::AlignCenter);
            QStandardItem* item6 = new QStandardItem(QString::number(spark_info->table.Sudu[i]));
            item6->setTextAlignment(Qt::AlignCenter);
            QStandardItem* item7 = new QStandardItem(QString::number(spark_info->table.Shenggao[i]));
            item7->setTextAlignment(Qt::AlignCenter);
            QStandardItem* item8 = new QStandardItem(QString::number(spark_info->table.Gongshi[i]));
            item8->setTextAlignment(Qt::AlignCenter);
            QStandardItem* item9 = new QStandardItem(QString::number(spark_info->table.Mianji[i]));
            item9->setTextAlignment(Qt::AlignCenter);
            QStandardItem* item10 = new QStandardItem(QString::number(spark_info->table.Jixing[i]));
            item10->setTextAlignment(Qt::AlignCenter);
            QStandardItem* item11 = new QStandardItem(QString::number(spark_info->table.Gaoya[i]));
            item11->setTextAlignment(Qt::AlignCenter);
            QList<QStandardItem*> item;
            item <<item1<<item2<<item3<<item4<<item5<<item6<<item7<<item8<<item9<<item10<<item11;
            model->appendRow(item);
        }
        else{
            qDebug()<<i;
            continue;
        }
    }

    model->sort(0 ,Qt::AscendingOrder);

    ui->tableView->setModel(model);
}

void MainInterface::tableRollUpdate()
{
    unsigned int row = 0;
    unsigned int column = 0;
    if(table_state == TABLE_SHOW){
        if(spark_info->b_array[B_SELECT]){
            for(row = 0;row < abs(model->rowCount());row++){
                QBrush brush;
                if(row == spark_info->uint_array[UINT_CURRENT_ROM]){
                    brush = QBrush(SELECT_COLOR);
                }
                else if(row < spark_info->uint_array[UINT_START_ROW]){
                    brush = QBrush(UNSELECT_COLOR);
                }
                else if(row < spark_info->uint_array[UINT_CURRENT_ROM]){
                    brush = QBrush(OK_COLOR);
                }
                else if(row <= spark_info->uint_array[UINT_END_ROW]){
                    brush = QBrush(UNOK_COLOR);
                }
                else{
                    brush = QBrush(UNSELECT_COLOR);
                }
                for(column = 0;column < 11;column ++){
                    QStandardItem* item = model->item(row ,column);
                    item->setBackground(brush);
                }
            }
        }
        /*没有加工选择 */
        else{
            for(row = 0;row < abs(model->rowCount());row++){
                QBrush brush;
                brush = QBrush(UNSELECT_COLOR);
                for(column = 0;column < 11;column ++){
                    QStandardItem* item = model->item(row ,column);
                    item->setBackground(brush);
                }
            }
        }
    }
    else if(table_state == TABLE_SELECT||table_state == TABLE_DELETE||table_state == TABLE_EDIT||table_state == TABLE_ADD){
        for(row = 0;row < abs(model->rowCount());row++){
            QBrush brush;
            if(row < spark_info->uint_array[UINT_START_ROW]){
                brush = QBrush(UNSELECT_COLOR);
            }
            else if(row <= spark_info->uint_array[UINT_END_ROW]){
                brush = QBrush(UNOK_COLOR);
            }
            else{
                brush = QBrush(UNSELECT_COLOR);
            }
            for(column = 0;column < 11;column ++){
                QStandardItem* item = model->item(row ,column);
                item->setBackground(brush);
            }
        }
    }
}

/*段选时初始化start_or_end为真，则选择开始行*/
void MainInterface::tableSelect(bool b)
{
    start_or_end = b;
}

/*数据发生改变时重新排序*/
void MainInterface::tableItemChange(QModelIndex tl ,QModelIndex br)
{
    qDebug()<<"tableItemChange";
    if(tl.column() == 0){
        model->sort(tl.column() ,Qt::AscendingOrder);
        submitTable();
    }
    else{
        br.column();
        submitTable();
    }
}

/*段选、增加段、删除段操作*/
void MainInterface::tableRowClick(QModelIndex r)
{
    int current;

    if(table_state == TABLE_EDIT){
        qDebug()<<r.row();
    }
    else if(table_state == TABLE_SELECT){
        if(start_or_end){
            spark_info->setUInt(UINT_START_ROW ,r.row());
            start_or_end = false;
        }
        else{
            spark_info->setUInt(UINT_END_ROW ,r.row());
            start_or_end = true;
        }
        spark_info->setBool(B_SELECT ,true);
    }
    else if(table_state == TABLE_DELETE){
        if(model->rowCount() > 0){
            current = r.row();
            model->removeRow(current);
            submitTable();
        }
        else{
            qDebug()<<"Null"<<endl;
        }
    }
    else if (table_state == TABLE_ADD) {
        if(model->rowCount() < 10){
            current = r.row();
            tableAddRow(current);
            submitTable();
        }
        else{
            qDebug()<<"Full"<<endl;
        }
    }
    else{
        qDebug()<<"0";
    }
}

/*检查并提交数据表数据*/
void MainInterface::submitTable()
{
    int i = 0, j = 0;
    int min = 0,max = 0;
    bool ok = false;
    bool first = true;

    /*清空数据表，当数据表中的Index字段为0时，该行即为空行*/
    spark_info->tableClear();

    /*根据表格内容填充有效数据行*/
    for(i = 0;i < model->rowCount();i++){

        j = 0;

        /*根据行的背景颜色判断开始行和结束行*/
        QBrush brush = model->item(i , 0)->background();
        if(brush.operator == (QBrush(UNOK_COLOR))&&first){
            qDebug()<<"start"<<i;
            min = i;
            first =false;
        }
        else if(brush.operator == (QBrush(UNOK_COLOR))&&!first){
            max = i;
            qDebug()<<"end"<<i;
        }

        spark_info->table.Shendu[i] = model->item(i,j++)->text().toDouble(&ok) * 1000;
        spark_info->table.Dianliu[i] = model->item(i,j++)->text().toUInt(&ok ,10);
        spark_info->table.Maikuan[i] = model->item(i,j++)->text().toUInt(&ok ,10);
        spark_info->table.Xiuzhi[i] = model->item(i,j++)->text().toUInt(&ok ,10);
        spark_info->table.Jianxi[i] = model->item(i,j++)->text().toUInt(&ok ,10);
        spark_info->table.Sudu[i] = model->item(i,j++)->text().toUInt(&ok ,10);
        spark_info->table.Shenggao[i] = model->item(i,j++)->text().toUInt(&ok ,10);
        spark_info->table.Gongshi[i] = model->item(i,j++)->text().toUInt(&ok ,10);
        spark_info->table.Mianji[i] = model->item(i,j++)->text().toUInt(&ok ,10);
        spark_info->table.Jixing[i] = model->item(i,j++)->text().toUInt(&ok ,10);
        spark_info->table.Gaoya[i] = model->item(i,j++)->text().toUInt(&ok ,10);
        /*Index字段不为0*/
        spark_info->table.Index[i] = i + 1;
    }

    spark_info->tableSave();

    if(table_state == TABLE_DELETE ){
        spark_info->setUInt(UINT_START_ROW , min);
        spark_info->setUInt(UINT_END_ROW , max);
    }
}

void MainInterface::funcbarUpdate(int i)
{
    disconnect(barui ,SLOT(F0()));
    ui->horizontalLayout->removeWidget(barui);
    barui->setHidden(true);
    //delete barui;

    barui = barmap[i];
    barui ->setHidden(false);
    ui->horizontalLayout->addWidget(barui);
    connect(this ,SIGNAL(barF0()) ,barui ,SLOT(F0()));
    emit barF0();

}

void MainInterface::menuTimeout()
{
    QPushButton *Fn ;
    Fn = barui->findChild<QPushButton *> ("pushButton_F8");
    if(Fn != NULL){
        Fn->click();
    }

    if(timer->isActive())
        timer->stop();
}

void MainInterface::menuShow(bool b)
{
    QPushButton *Fn ;
    Fn = barui->findChild<QPushButton *> ("pushButton_F8");
    if(Fn != NULL){
        if(menu->isHidden()){
            menu->setHidden(false);
            mesg->setHidden(true);
            menu->setFocus();
        }else{
            menu->setHidden(true);
            mesg->setHidden(false);
            //this->setFocus();
        }
        if(b){
            if(menu->isHidden())
                Fn->setChecked(false);
            else
                Fn->setChecked(true);
        }
    }
}

void MainInterface::XYZ_Update(int i)
{

    QString s;
    s = toString(spark_info->l_array[i]);

    switch(i){
    case L_X_CURRENT:
        ui->label_cux->setText(s);
        break;
    case L_Y_CURRENT:
        ui->label_cuy->setText(s);
        break;
    case L_Z_CURRENT:
        ui->label_cuz->setText(s);
        break;
    case L_X_ABSOLUTE:
        ui->label_mex->setText(s);
        break;
    case L_Y_ABSOLUTE:
        ui->label_mey->setText(s);
        break;
    case L_Z_ABSOLUTE:
        ui->label_mez->setText(s);
        break;
    case L_X_REMAIN:
        ui->label_rex->setText(s);
        break;
    case L_Y_REMAIN:
        ui->label_rey->setText(s);
        break;
    case L_Z_REMAIN:
        ui->label_rez->setText(s);
        break;
    default:
        break;
    }

}

QString MainInterface::toString(LONG64 l)
{
    QString s;

    int m = 0;
    long long tmp = 0;
    long long g = 10000000;
    char ch = '0';
    bool first = false;

    tmp = l;

    if(tmp < 0)
        s.append('-');

    for(m = 0;m < 7;m++){
        if(m == 4){
            if(!first){
                s.append('0');
                s.append('.');
                first = true;
            }
            else
                s.append('.');
        }
        tmp = l % g;
        g = g / 10;
        tmp = tmp / g;
        tmp = abs(tmp);
        ch = tmp & 0xFF;
        ch = ch + 48;

        if(ch != '0'){
            first =true;
        }

        if(first)
            s.append(ch);
    }

    return s;
}

void MainInterface::axisIndexUpdate()
{
    QString s;
    s = QString::number(spark_info->uint_array[UINT_COOR_INDEX],10);

    ui->label_cuix->setText(s);
    ui->label_cuiy->setText(s);
    ui->label_cuiz->setText(s);
}

MainInterface::~MainInterface()
{
    delete ui;
}
