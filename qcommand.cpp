#include "qcommand.h"
#include "sparkinfo.h"
#include "qdebug.h"

QCommand::QCommand(QWidget *parent) :
    QWidget(parent)
{
    initLabel();
    value = "";
    mesg = mesg_label[0];

    wr_index = 0;
    rd_index = 0;

    flag = false;

    timer = new QTimer(this);
    timer->setInterval(500);

    connect(timer ,SIGNAL(timeout()) ,this ,SLOT(flashCursor()));

    setStatus(0x00);
    setType(false ,true ,0x04 ,true ,0x03);
    setTarget(L_SLOT ,L_DEEP_CURRENT);
}

/*输出字符串初始化*/
void QCommand::initLabel()
{
    status_label[0] = tr("空");
    status_label[1] = tr("提示：");
    status_label[2] = tr("请输入：");
    status_label[3] = tr("警告：");
    status_label[4] = tr("报警：");

    mesg_label[0] = tr("空");
    mesg_label[1] = tr("输入正确！");
    mesg_label[2] = tr("输入不能为空！")  ;
    mesg_label[3] = tr("输入格式有误！");
}

void QCommand::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.save();

    switch(status){
    case 0x00:
        painter.setPen(MESG_COLOR);
        painter.setBrush(MESG_COLOR);
        break;
    case 0x10:
        painter.setPen(INPUT_COLOR);
        painter.setBrush(INPUT_COLOR);
        break;
    case 0x20:
        painter.setPen(WARNING_COLOR);
        painter.setBrush(WARNING_COLOR);
        break;
    case 0x30:
        painter.setPen(ALTER_COLOR);
        painter.setBrush(ALTER_COLOR);
        break;
    default:
        break;
    }

    QFont font = qApp->font();
    font.setPointSize(14);
    painter.setFont(font);

    QFontMetricsF fm(font);
    double h = fm.size(Qt::TextSingleLine,label).height();
    double w = fm.size(Qt::TextSingleLine,label).width();

    painter.translate(3, height()/2-4);
    painter.drawText(3 ,h/2 ,label);

    if(status == 0x10){
        painter.drawText(w+3 , h/2 ,value);

        h = fm.size(Qt::TextSingleLine,label+value).height();
        w = fm.size(Qt::TextSingleLine,label+value).width();

        if(flag)
            painter.drawText(w+3 ,h/2 ,"|");
        else
            painter.drawText(w+3 ,h/2 ,"");
    }
    else{
        painter.drawText(w+3 , h/2 ,mesg);
    }

    painter.restore();
}

void QCommand::flashCursor()
{
    if(flag){
        flag = false;
    }
    else{
        flag = true;
    }
    update();
}

void QCommand::keyPressEvent(QKeyEvent *k)
{
    k->accept();
    if(status == 0x10){
        switch(k->key())
        {
        case Qt::Key_0:
            //if(!value.contains(".")&&value.contains("0"))
                //break;
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
            value.append(QString::number(k->key()-48));
            break;
        case Qt::Key_Period:
            if(type & 0x04){
                if(!value.contains(".")){
                    if(value.isEmpty())
                        value.append("0.");
                    else if(value.length() == 1&&(value.startsWith("-")||value.startsWith("+")))
                        value.append("0.");
                    else
                        value.append(".");
                }
            }
            break;
        case Qt::Key_Plus:
            if(type & 0x40){
                if(value.startsWith("-")||value.startsWith("+"))
                    value.replace(0,1,"+");
                else
                    value.insert(0,"+");
            }
            break;
        case Qt::Key_Minus:
            if(type & 0x40){
                if(value.startsWith("+")||value.startsWith("-"))
                    value.replace(0,1,"-");
                else
                    value.insert(0,"-");
            }
            break;
        case Qt::Key_Backspace:
            value.chop(1);
            break;
        case Qt::Key_Right:
        case Qt::Key_Up:
            value.clear();
            if(record.contains(rd_index)){
                QString tmp = record.find(rd_index).value();
                value.append(tmp);
            }
            if(rd_index > 0)
                rd_index--;
            else
                rd_index = record.size();
            break;
        case Qt::Key_Left:
        case Qt::Key_Down:
            value.clear();
            if(record.contains(rd_index)){
                QString tmp = record.find(rd_index).value();
                value.append(tmp);
            }
            if(rd_index < record.size())
                rd_index++;
            else
                rd_index = 0;
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            parentWidget()->setFocus();
            checkString(value);
            value.clear();
            break;
        default:
            break;
        }
    }
    update();
}

void QCommand::checkString(QString s)
{
    int i = 0 , j = 0 ,k = 0;
    char c_j , c_k;
    bool dot = false;

    c_j = type >> 3;
    c_k = type;
    if(!s.isEmpty()){
        /*检测字符串中是否有小数点，以及小数点前后的位数*/
        for(;i < s.length();i++){
            if(s.at(i) == '.')
                dot = true;
            if(s.at(i) >= '0'&&s.at(i) <= '9'){
                if(!dot)j++;
                if(dot)k++;
            }
        }

        /*检查输入字符串的整数小数类型是否与设定一致*/
        if(dot != (bool)(type & 0x04)){
            showMesg(0x20 ,3);
            return;
        }

        if(j <= (c_j&0x07) && j >=1 && k <= (c_k&0x03)){
            /*输入数据为小数*/
            if(dot && k >=1){
                finalSubmit(s);
                recordAppend(s);
                showMesg(0x00 ,1);
                emit finish();
            }
            /*输入数据为整数*/
            else if(!dot){
                finalSubmit(s);
                recordAppend(s);
                showMesg(0x00 ,1);
                emit finish();
            }
            else{
                showMesg(0x20 ,3);
                emit finish();
            }
        }
        else{
            showMesg(0x20 ,3);
            emit finish();
        }
    }
    else{
        showMesg(0x20 ,2);
        emit finish();
    }
}

void QCommand::finalSubmit(QString s)
{
    char tmp = 0;
    tmp = target & 0xF0;
    tmp = tmp >>4;
    bool ok = false;
    double d = 0;
    unsigned int i = 0;
    long l = 0;
    int fcount = type & 0x03;

    switch(tmp){
    case B_SLOT:
        break;
    case UINT_SLOT:
        i = s.toUInt(&ok ,10);
        if(ok)
            spark_info->setUInt(target & 0x0F ,i);
        break;
    case L_SLOT:
        d = s.toDouble(&ok);
        for(; fcount > 0;fcount --)
            d = d*10;
        l = (long)d;
        if(ok){
            if((target & 0x0F)==L_X_CURRENT||(target & 0x0F)==L_Y_CURRENT||(target & 0x0F)==L_Z_CURRENT){
                /*设置当前位置*/
                spark_info->setLong(target & 0x0F , l);
                /*设置当前位置的偏移 ，Offset = Position - Count*/
                spark_info->setLong((target & 0x0F)+(L_X_OFFSET-L_X_CURRENT) ,l - spark_info->l_array[(target & 0x0F)+(L_X_COUNTER-L_X_CURRENT)]);
            }
            else
                spark_info->setLong(target & 0x0F , l);
        }
        break;
    default :
        break;
    }
}

void QCommand::recordAppend(QString r)
{
    if(wr_index > 9)
        wr_index = 0;
    if(record.contains(wr_index)){
        record.remove(wr_index);
        record.insert(wr_index,r);
    }
    else
        record.insert(wr_index,r);
    rd_index = wr_index;
    wr_index++;
}

void QCommand::showMesg(char c ,int i)
{

    if(c != 0x10){
        setStatus(c);
        mesg.clear();
        mesg.append(mesg_label[i]);
    }
    update();
}

void QCommand::setStatus(char c)
{
    status = c;

    switch(status)
    {
    case 0x00:
        label = status_label[1];
        if(timer->isActive())
            timer->stop();
        break;
    case 0x10:
        label = status_label[2];
        if(!timer->isActive())emit
            timer->start();
        break;
    case 0x20:
        label = status_label[3];
        if(timer->isActive())
            timer->stop();
        break;
    case 0x30:
        label = status_label[4];
        if(timer->isActive())
            timer->stop();
        break;
    default :
        label = status_label[1];
        if(timer->isActive())
            timer->stop();
        break;
    }
    update();
}

void QCommand::setType(char c)
{
    type = c;
}

void QCommand::setType(bool isString,bool isSigned,char intCount,bool isDot,char floatCount)
{
    type = 0x00;
    if(isString)
        type = type|0x80;
    if(isSigned)
        type = type|0x40;
    type = type|(intCount<<3);
    if(isDot)
        type = type|0x04;
    type = type|floatCount;
}

void QCommand::setTarget(char slot,char index)
{
    target =0;
    target =target|(slot<<4);
    target =target|index;
}
