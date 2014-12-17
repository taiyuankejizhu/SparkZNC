#include "coordialog.h"
#include "sparkinfo.h"
#include "ui_coordialog.h"
#include "setting.h"
#include "qdebug.h"

CoorDialog::CoorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CoorDialog)
{
    ui->setupUi(this);
    this->setGeometry(0 ,390 ,width() ,height());
    this->setWindowFlags(Qt::FramelessWindowHint);

    QPixmap a_pix = QPixmap(":/selected.png");
    a_icon = QIcon(a_pix);
    QPixmap b_pix = QPixmap(":/unselected.png");
    b_icon = QIcon(b_pix);

    QPixmap c_pix = QPixmap(":/axis.png");

    QPixmap ok_pix = QPixmap(":/ok.png");
    ok_icon = QIcon(ok_pix);
    QPixmap cancel_pix = QPixmap(":/cancel.png");
    cancel_icon = QIcon(cancel_pix);

    selected = spark_info->uint_array[UINT_COOR_INDEX];
    model = new QStandardItemModel(0 ,1);
    updateContent();
    valueChange("Init");

    connect(ui->listView ,SIGNAL(doubleClicked(QModelIndex)) ,this ,SLOT(itemChange(QModelIndex)));
    connect(ui->lineEdit_x ,SIGNAL(textChanged(QString)) ,this ,SLOT(valueChange(QString)));
    connect(ui->lineEdit_y ,SIGNAL(textChanged(QString)) ,this ,SLOT(valueChange(QString)));
    connect(ui->lineEdit_z ,SIGNAL(textChanged(QString)) ,this ,SLOT(valueChange(QString)));

    ui->label->setPixmap(c_pix);
    ui->buttonBox->button(ui->buttonBox->Ok)->setText(tr("确定(O)"));
    ui->buttonBox->button(ui->buttonBox->Ok)->setIcon(ok_icon);
    ui->buttonBox->button(ui->buttonBox->Ok)->setIconSize(QSize(32,32));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setText(tr("取消(C)"));
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIcon(cancel_icon);
    ui->buttonBox->button(ui->buttonBox->Cancel)->setIconSize(QSize(32,32));
}

void CoorDialog::keyPressEvent(QKeyEvent *k)
{
    QWidget *w = focusWidget();
    QString type(w->metaObject()->className());

    k->accept();
    switch(k->key())
    {
    case Qt::Key_Down:
        focusNextChild();
        break;
    case Qt::Key_Up:
        focusPreviousChild();
        break;
    case Qt::Key_F3:
        finished(1);
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        ui->lineEdit_x->setFocus();
        break;
    case Qt::Key_F1:
    case Qt::Key_F2:
    case Qt::Key_F4:
    case Qt::Key_F5:
    case Qt::Key_F6:
    case Qt::Key_F7:
    case Qt::Key_F8:
        finished(0);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if((unsigned int)ui->listView->currentIndex().row() != selected && type.contains("View")){
            selected = ui->listView->currentIndex().row();
            updateContent();
        }else if(type.contains("Edit")){
            focusNextChild();
        }
        break;
    default:
        break;
    }
}

void CoorDialog::valueChange(QString s)
{
    bool ok = false;
    s = "";
    if(!ui->lineEdit_x->text().isEmpty()){
        double t = ui->lineEdit_x->text().toDouble(&ok);
        x = t * 1000;
        if(t > 9999.999 && ok){
            t = 9999.999;
            ui->lineEdit_x->setText("9999.999");
        }
        if(t < -9999.999 && ok){
            t = -9999.999;
            ui->lineEdit_x->setText("-9999.999");
        }
    }else{
        x = 0;
    }
    if(!ui->lineEdit_y->text().isEmpty()){
        double t = ui->lineEdit_y->text().toDouble(&ok);
        y = t * 1000;
        if(t > 9999.999 && ok){
            t = 9999.999;
            ui->lineEdit_y->setText("9999.999");
        }
        if(t < -9999.999 && ok){
            t = -9999.999;
            ui->lineEdit_y->setText("-9999.999");
        }
    }else{
        y = 0;
    }
    if(!ui->lineEdit_z->text().isEmpty()){
        double t = ui->lineEdit_z->text().toDouble(&ok);
        z = t * 1000;
        if(t > 9999.999 && ok){
            t = 9999.999;
            ui->lineEdit_z->setText("9999.999");
        }
        if(t < -9999.999 && ok){
            t = -9999.999;
            ui->lineEdit_z->setText("-9999.999");
        }
    }else{
        z = 0;
    }
}

void CoorDialog::itemChange(QModelIndex i)
{
    selected = i.row();
    updateContent();
    /* finished(1); */
}

void CoorDialog::updateContent()
{
    unsigned int i;
    QString label = tr("坐标 ");
    QString value ;

    model->clear();

    for(i = 0;i < spark_info->uint_array[UINT_COOR_MAX];i++){
        QStandardItem* item = new QStandardItem();
        value.clear();
        value.append(QString::number(i)+":");

        /*读取当前坐标*/
        EightBytes  c_xi;
        EightBytes  c_yi;
        EightBytes  c_zi;

        memset(c_xi.bytes , 0 ,sizeof c_xi);
        memset(c_yi.bytes , 0 ,sizeof c_yi);
        memset(c_zi.bytes , 0 ,sizeof c_zi);

        FM25V02_READ(X_AXIS_ADDR + i*3*(sizeof c_xi), c_xi.bytes ,sizeof c_xi);
        FM25V02_READ(Y_AXIS_ADDR + i*3*(sizeof c_yi), c_yi.bytes ,sizeof c_yi);
        FM25V02_READ(Z_AXIS_ADDR + i*3*(sizeof c_zi), c_zi.bytes ,sizeof c_zi);

        c_xi.longs += spark_info->l_array[L_X_ABSOLUTE];
        c_yi.longs += spark_info->l_array[L_Y_ABSOLUTE];
        c_zi.longs += spark_info->l_array[L_Z_ABSOLUTE];

        value.append("      X:"+toString(c_xi.longs)+"      Y:"+toString(c_yi.longs)+"      Z:"+toString(c_zi.longs));

        if(i == selected){
            item->setIcon(a_icon);
            ui->lineEdit_x->setText(toString(c_xi.longs));
            ui->lineEdit_y->setText(toString(c_yi.longs));
            ui->lineEdit_z->setText(toString(c_zi.longs));
        }else
            item->setIcon(b_icon);
        item->setText(label+value);
        model->appendRow(item);
    }

    QModelIndex start;
    start = model->index(selected ,0);
    ui->listView->setCurrentIndex(start);

    ui->listView->setEditTriggers(QListView::NoEditTriggers);
    ui->listView->setModel(model);

    ui->listView->setFixedHeight(200);
    /*
    for(i = 0;i < model->rowCount();i++){
        ui->listView->setFixedHeight();
    }*/
}

QString CoorDialog::toString(LONG64 l)
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

CoorDialog::~CoorDialog()
{
    delete ui;
}
