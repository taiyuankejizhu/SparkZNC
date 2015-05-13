#include "sparkinfo.h"
#include "qdebug.h"

SparkInfo::SparkInfo(QObject *parent) :
    QObject(parent)
{

    memcpy(b_array , bool_init ,sizeof b_array);
    memcpy(uint_array , uint_init ,sizeof uint_array);
    memcpy(l_array , long_init ,sizeof l_array);
    memset(c_array ,0x00 ,sizeof c_array);

    memset(target.bytes , 0 ,sizeof target);
    memset(useup.bytes , 0 ,sizeof useup);

    hand.Axis = Hand_NULL_AXIS;
    hand.Position_Control = NULL;
    hand.Velocity_Control = NULL;
    hand.Count = NULL;

    fm25v02Init();

    carryInit();
    tableInit();

    sysInit();

    /*当前表的索引改变时，更新表的数据*/
    connect(this ,SIGNAL(tableIndexChange()) ,this ,SLOT(tableLoad()));

}

void SparkInfo::sysInit()
{
    /*初始化蜂鸣器*/
    QFile beep(BEEP_FILE);
    QFile backlight(BACKLIGHT_FILE);

    if (beep.exists())
    {
        beep_fb = open(BEEP_FILE, O_RDWR);
        if (beep_fb < 0)
        {
            printf("open device beep fail");
        }
    }

    if(backlight.exists())
    {
        backlight_fb = open(BACKLIGHT_FILE, O_RDWR);
        if(backlight_fb < 0)
        {
            printf("open device backlight fail");
        }else{
            ioctl(backlight_fb, (uint_array[UINT_BRIGHTNESS] & 0xf0) >> 4,
                  20 + 20 * (uint_array[UINT_BRIGHTNESS] & 0x0f));
        }
    }

}

/*初始化铁电芯片中的运行参数*/
void SparkInfo::fm25v02Init()
{
    /*铁电芯片初始化*/
    FM25V02_Init();

    /*读取时间溢出*/
    FM25V02_READ(TARGET_TIME_ADDR , target.bytes ,sizeof target);
    /*读取已经放电时间*/
    FM25V02_READ(USEDP_TIME_ADDR , useup.bytes ,sizeof useup);

    /*读取初始化X轴方向*/
    char tmp = 0;
    FM25V02_READ(X_ORIENT_ADDR ,&tmp ,sizeof tmp);
    if(tmp == CFALSE)
        b_array[B_X_ORIENT] = false;
    else
        b_array[B_X_ORIENT] = true;

    /*读取初始化Y轴方向*/
    tmp = 0;
    FM25V02_READ(Y_ORIENT_ADDR ,&tmp ,sizeof tmp);
    if(tmp == CFALSE)
        b_array[B_Y_ORIENT] = false;
    else
        b_array[B_Y_ORIENT] = true;

    /*读取初始化Z轴方向*/
    tmp = 0;
    FM25V02_READ(Z_ORIENT_ADDR ,&tmp ,sizeof tmp);
    if(tmp == CFALSE)
        b_array[B_Z_ORIENT] = false;
    else
        b_array[B_Z_ORIENT] = true;

    /*读取初始化光栅刻度*/
    tmp = 0;
    FM25V02_READ(SCALE_ADDR ,&tmp ,sizeof tmp);
    uint_array[UINT_SCALE] = tmp;

    /*读取当前坐标的索引*/
    FourBytes c_axis;
    memset(c_axis.bytes , 0 ,sizeof c_axis);
    FM25V02_READ(CURRENT_AXIS_ADDR , c_axis.bytes ,sizeof c_axis);

    uint_array[UINT_COOR_INDEX] = c_axis.uint;

    /*读取当前组别的索引*/
    FourBytes t_axis;
    memset(t_axis.bytes , 0 ,sizeof t_axis);
    FM25V02_READ(CURRENT_TAB_ADDR , t_axis.bytes ,sizeof t_axis);

    uint_array[UINT_TAB_INDEX] = t_axis.uint;

    EightBytes  c_x;
    EightBytes  c_y;
    EightBytes  c_z;

    /*读取光栅计数值*/
    memset(c_x.bytes , 0 ,sizeof c_x);
    memset(c_y.bytes , 0 ,sizeof c_y);
    memset(c_z.bytes , 0 ,sizeof c_z);

    FM25V02_READ(X_ABSOLUTE_ADDR, c_x.bytes ,sizeof c_x);
    FM25V02_READ(Y_ABSOLUTE_ADDR, c_y.bytes ,sizeof c_y);
    FM25V02_READ(Z_ABSOLUTE_ADDR, c_z.bytes ,sizeof c_z);

    l_array[L_X_ABS_OFFSET] = c_x.longs;
    l_array[L_Y_ABS_OFFSET] = c_y.longs;
    l_array[L_Z_ABS_OFFSET] = c_z.longs;

    l_array[L_X_ABSOLUTE] = c_x.longs;
    l_array[L_Y_ABSOLUTE] = c_y.longs;
    l_array[L_Z_ABSOLUTE] = c_z.longs;

    /*读取当前坐标*/
    memset(c_x.bytes , 0 ,sizeof c_x);
    memset(c_y.bytes , 0 ,sizeof c_y);
    memset(c_z.bytes , 0 ,sizeof c_z);

    FM25V02_READ(X_AXIS_ADDR + uint_array[UINT_COOR_INDEX]*3*(sizeof c_x), c_x.bytes ,sizeof c_x);
    FM25V02_READ(Y_AXIS_ADDR + uint_array[UINT_COOR_INDEX]*3*(sizeof c_y), c_y.bytes ,sizeof c_y);
    FM25V02_READ(Z_AXIS_ADDR + uint_array[UINT_COOR_INDEX]*3*(sizeof c_z), c_z.bytes ,sizeof c_z);

    l_array[L_X_OFFSET] = c_x.longs;
    l_array[L_Y_OFFSET] = c_y.longs;
    l_array[L_Z_OFFSET] = c_z.longs;

    l_array[L_X_CURRENT] = l_array[L_X_OFFSET] + l_array[L_X_ABSOLUTE];
    l_array[L_Y_CURRENT] = l_array[L_Y_OFFSET] + l_array[L_Y_ABSOLUTE];
    l_array[L_Z_CURRENT] = l_array[L_Z_OFFSET] + l_array[L_Z_ABSOLUTE];
}

void SparkInfo::carryInit()
{
    c_array[C_Z_OT0] = 0xe0;
    c_array[C_Z_OT1] = 0x00;

    c_array[C_U_OT0] = 0x07;
    c_array[C_U_OT1] = 0x0b;
    c_array[C_U_OT2] = 0x00;

    c_array[C_P_IO0] = 0x10;
    c_array[C_P_IO1] = 0x80;
    c_array[C_P_IO2] = 0x00;
    c_array[C_P_IO3] = 0x00;
    c_array[C_P_IO4] = 0x00;
}

bool SparkInfo::checkTime()
{
    LONG64 t = 0;
    LONG64 u = 0;
    /*读取时间溢出*/
    FM25V02_READ(TARGET_TIME_ADDR , target.bytes ,sizeof target);
    /*读取已经放电时间*/
    FM25V02_READ(USEDP_TIME_ADDR , useup.bytes ,sizeof useup);

    t |= target.ushorts[0];
    t = t << 16;
    u = useup.ushorts[0];
    u = u << 16;

    t |= target.ushorts[1];
    u |= useup.ushorts[1];

    /*放电时间溢出*/
    if(u > t){
        return false;
    }
    else {
        return true;
    }
}

void SparkInfo::tableInit()
{
    int i = 0;

    Table0 table0;
    table0.Nummber[0] = 1;
    table0.Shendu[0] = 100;
    table0.Jixing[0] = 0;

    DataBank bank;
    bank.createTable(9);
    bank.tableSave(&table0 ,9);
    //bank.tableLoad(&table0 ,9);

    /*默认的数据表*/
    if(uint_array[UINT_TAB_INDEX] == 0){
        for(; i < 10; i++){
            table.Shendu[i] = table_init.Shendu[i];
            table.Dianliu[i] = table_init.Dianliu[i];
            table.Maikuan[i] = table_init.Maikuan[i];
            table.Xiuzhi[i] = table_init.Xiuzhi[i];
            table.Jianxi[i] = table_init.Jianxi[i];
            table.Sudu[i] = table_init.Sudu[i];
            table.Shenggao[i] = table_init.Shenggao[i];
            table.Gongshi[i] = table_init.Gongshi[i];
            table.Mianji[i] = table_init.Mianji[i];
            table.Jixing[i] = table_init.Jixing[i];
            table.Gaoya[i] = table_init.Gaoya[i];
            table.Index[i] = table_init.Index[i];
        }
        tableSave();
    }
    else
        tableLoad();
}

/*自动生成表格*/
void SparkInfo::tableAuto(LONG64 deep ,UNINT32 current,UNINT32 area,UNINT32 effect)
{
    tableClear();

    unsigned int i = 0;
    unsigned int group = 0;       /*根据电流选组别*/
    unsigned int lines;       /*确定要显示的行数*/
    switch(current / 2)
    {
        case 0:
        case 1:group = 0;break;
        case 2:group = 1;break;
        case 3:group = 2;break;
        case 4:case 5:group = 3;break;
        case 6:case 7:group = 4;break;
        case 8:case 9:group = 5;break;
        case 10:case 11:case 12:group = 6;break;
        case 13:case 14:case 15:case 16:case 17:group = 7;break;
        case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:group = 8;break;
        case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:group = 9;break;
        case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:
        case 44:case 45:case 46:case 47:case 48:case 49:case 50:group = 10;break;
        case 51:case 52:case 53:case 54:case 55:case 56:case 57:case 58:case 59:case 60:
        case 61:case 62:case 63:case 64:case 65:case 66:case 67:case 68:case 69:case 70:
        case 71:case 72:case 73:case 74:case 75:group = 11;break;
        default:break;
    }

    lines = search[group].Index[effect % 4];

    for(i = 0;i < lines;i++)
    {
        table.Shendu[i] = (deep - (search[group].Shendu[i] - search[group].Shendu[lines - 1]));
        table.Dianliu[i] = current * search[group].Dianliu[i];
        if(effect > 4)
            table.Maikuan[i] = search[group].Maikuan[i] * 0.8;
        else
            table.Maikuan[i] = search[group].Maikuan[i];
        table.Xiuzhi[i] = search[group].Xiuzhi[i];
        table.Jianxi[i] = search[group].Jianxi[i];
        table.Sudu[i] = 5;
        table.Shenggao[i] = 8;
        table.Gongshi[i] = search[group].Gongshi[i];
        table.Mianji[i] = area;
        table.Jixing[i] = 0;
        if(!table.Dianliu[i])
            table.Dianliu[i]++;
        if(effect > 4)
            table.Gaoya[i] = 4;
        else
            table.Gaoya[i] = 5;
        table.Index[i] = i + 1;
    }

    tableSave();
}

/*保存数据表数据*/
void SparkInfo::tableSave()
{
    bool state = false;

    /*写方式打开数据表文件*/
    QFile file(QDir::currentPath()+TABLES_DIR+QString::number(uint_array[UINT_TAB_INDEX]));
    state = file.open(QIODevice::WriteOnly);

    if(state){
        /*序列化数据表结构体*/
        QByteArray bytes;
        bytes.resize(sizeof table);
        memcpy(bytes.data() ,&table ,sizeof table);

        file.reset();
        file.write(bytes);
        file.close();
    }
}

/*加载数据表格*/
void SparkInfo::tableLoad()
{
    bool state =false;

    /*读方式打开数据表文件*/
    QFile file(QDir::currentPath()+TABLES_DIR+QString::number(uint_array[UINT_TAB_INDEX]));
    state = file.open(QIODevice::ReadOnly);

    if(state){
        /*序列化数据表结构体*/
        QByteArray bytes;

        file.reset();
        if(file.size() < sizeof table){
            tableClear();
            tableSave();
        }
        else{
            bytes = file.read(sizeof table);
            memcpy(&table ,bytes.data() ,sizeof table);
        }
        file.close();

        emit tableChange();
    }
}

/*清空数据表数据*/
void SparkInfo::tableClear()
{
    int i = 0;
    for(; i < 10; i++){
        table.Shendu[i] = 0;
        table.Dianliu[i] = 0;
        table.Maikuan[i] = 0;
        table.Xiuzhi[i] = 0;
        table.Jianxi[i] = 0;
        table.Sudu[i] = 0;
        table.Shenggao[i] = 0;
        table.Gongshi[i] = 0;
        table.Mianji[i] = 0;
        table.Jixing[i] = 0;
        table.Gaoya[i] = 0;
        table.Index[i] = 0 ;
    }
}

void SparkInfo::setBool(UNINT32 i,bool b)
{
    /*检查数据是否改变*/
    bool check = false;

    if(i < B_LENGTH){
        if(b_array[i] == b){
            check = false;
            b_array[i] = b;
        }
        else{
            check = true;
            b_array[i] = b;
        }

        /*布尔数组状态更新*/
        if(i != B_UPDATE && b_array[i] != b)
            b_array[B_UPDATE] = true;
        if(i == B_START|| i == B_TIME){
            emit startChange();
            emit boolChange();
        }
        if(check){
            emit boolChange();
        }
    }
}

void SparkInfo::reverseBool(UNINT32 i)
{
    if(i < B_LENGTH){
        /*布尔数组状态更新*/
        if(i != B_UPDATE)
            b_array[B_UPDATE] = true;
        if(b_array[i])
            b_array[i] = false;
        else
            b_array[i] = true;
        if(i == B_START|| i == B_TIME){
            emit startChange();
            emit boolChange();
        }
        else{
            emit boolChange();
        }
    }
}

void SparkInfo::setLong(UNINT32 i,LONG64 l)
{
    /*检查数据是否改变*/
    bool check = false;

    if(i < L_LENGTH){
        if(l_array[i] == l){
            check = false;
            l_array[i] = l;
        }
        else{
            check = true;
            l_array[i] = l;
        }
        if(i == L_X_CURRENT||i == L_Y_CURRENT||i == L_Z_CURRENT){

            /*只有在数据改变时才会写入铁电芯片*/
            if(check){
                EightBytes wr;
                wr.longs = 0;
                wr.longs = spark_info->l_array[i];

                switch(i){
                case L_X_CURRENT:
                    wr.longs -= spark_info->l_array[L_X_ABSOLUTE];
                    spark_info->l_array[L_X_OFFSET] = wr.longs;
                    FM25V02_WRITE(X_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof wr), wr.bytes, sizeof wr);
                    break;
                case L_Y_CURRENT:
                    wr.longs -= spark_info->l_array[L_Y_ABSOLUTE];
                    spark_info->l_array[L_Y_OFFSET] = wr.longs;
                    FM25V02_WRITE(Y_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof wr), wr.bytes, sizeof wr);
                    break;
                case L_Z_CURRENT:
                    wr.longs -= spark_info->l_array[L_Z_ABSOLUTE];
                    spark_info->l_array[L_Z_OFFSET] = wr.longs;
                    FM25V02_WRITE(Z_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof wr), wr.bytes, sizeof wr);
                    break;
                default:
                    break;
                }
            }

            emit xyzChange(i);
        }
        else if(i == L_X_ABSOLUTE||i == L_Y_ABSOLUTE||i == L_Z_ABSOLUTE){

            /*只有在数据改变时才会写入铁电芯片*/
            if(check){
                EightBytes wr;
                wr.longs = 0;
                wr.longs = spark_info->l_array[i];

                switch(i){
                case L_X_ABSOLUTE:
                    FM25V02_WRITE(X_ABSOLUTE_ADDR, wr.bytes, sizeof wr);
                    break;
                case L_Y_ABSOLUTE:
                    FM25V02_WRITE(Y_ABSOLUTE_ADDR, wr.bytes, sizeof wr);
                    break;
                case L_Z_ABSOLUTE:
                    FM25V02_WRITE(Z_ABSOLUTE_ADDR, wr.bytes, sizeof wr);
                    break;
                default:
                    break;
                }
            }

            emit xyzChange(i);
        }
        else if(i == L_X_REMAIN||i == L_Y_REMAIN||i == L_Z_REMAIN){
            emit xyzChange(i);
        }
        else if(i == L_X_COUNTER||i == L_Y_COUNTER||i == L_Z_COUNTER){
            /*更新绝对位置，Position = Offset + Count*/
            if(i == L_X_COUNTER)
                spark_info->setLong(L_X_ABSOLUTE ,spark_info->l_array[L_X_ABS_OFFSET] + spark_info->l_array[L_X_COUNTER]);
            if(i == L_Y_COUNTER)
                spark_info->setLong(L_Y_ABSOLUTE ,spark_info->l_array[L_Y_ABS_OFFSET] + spark_info->l_array[L_Y_COUNTER]);
            if(i == L_Z_COUNTER)
                spark_info->setLong(L_Z_ABSOLUTE ,spark_info->l_array[L_Z_ABS_OFFSET] + spark_info->l_array[L_Z_COUNTER]);

            /*更新当前位置，Position = Offset + Abs*/
            if(i == L_X_COUNTER){
                spark_info->l_array[L_X_CURRENT] = spark_info->l_array[L_X_OFFSET] + spark_info->l_array[L_X_ABSOLUTE];
                emit xyzChange(L_X_CURRENT);
            }
            if(i == L_Y_COUNTER){
                spark_info->l_array[L_Y_CURRENT] = spark_info->l_array[L_Y_OFFSET] + spark_info->l_array[L_Y_ABSOLUTE];
                emit xyzChange(L_Y_CURRENT);
            }
            if(i == L_Z_COUNTER){
                spark_info->l_array[L_Z_CURRENT] = spark_info->l_array[L_Z_OFFSET] + spark_info->l_array[L_Z_ABSOLUTE];
                emit xyzChange(L_Z_CURRENT);
            }

            emit xyzChange(i);
        }
        else if(i == L_DEEP_CURRENT){
            setLong(L_Z_REMAIN ,l_array[L_DEEP_TARGET] - l_array[L_DEEP_CURRENT]);
            emit longChange();
        }
        else if(i == L_DEEP_TARGET){
            setLong(L_Z_REMAIN ,l_array[L_DEEP_TARGET] - l_array[L_DEEP_CURRENT]);
            emit longChange();
        }
        else{
            emit longChange();
        }
    }
}

void SparkInfo::setUInt(UNINT32 i, UNINT32 u)
{
    unsigned int tmp;
    /*检查数据是否改变*/
    bool check = false;

    if(i < UINT_LENGTH){
        if(uint_array[i] == u){
            check = false;
            uint_array[i] = u;
        }
        else{
            check = true;
            uint_array[i] = u;
        }

        /*组别变化后，当前行、开始行、结束行重置*/
        if(i == UINT_TAB_INDEX){
            uint_array[UINT_CURRENT_ROM] = 0;
            uint_array[UINT_START_ROW] = 0;
            uint_array[UINT_END_ROW] = 0;

            /*保存当前组别的索引*/
            FourBytes t_axis;
            memset(t_axis.bytes , 0 ,sizeof t_axis);
            t_axis.uint = uint_array[UINT_TAB_INDEX];
            FM25V02_WRITE(CURRENT_TAB_ADDR , t_axis.bytes ,sizeof t_axis);

            emit tableIndexChange();
            emit tableRowChange();
        }
        else if(i == UINT_COOR_INDEX){
            /*保存当前坐标的索引*/
            FourBytes c_axis;
            memset(c_axis.bytes , 0 ,sizeof c_axis);
            c_axis.uint = uint_array[UINT_COOR_INDEX];
            FM25V02_WRITE(CURRENT_AXIS_ADDR , c_axis.bytes ,sizeof c_axis);

            /*读取当前坐标*/
            EightBytes  c_x;
            EightBytes  c_y;
            EightBytes  c_z;
            memset(c_x.bytes , 0 ,sizeof c_x);
            memset(c_y.bytes , 0 ,sizeof c_y);
            memset(c_z.bytes , 0 ,sizeof c_z);

            FM25V02_READ(X_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof c_x), c_x.bytes ,sizeof c_x);
            FM25V02_READ(Y_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof c_y), c_y.bytes ,sizeof c_y);
            FM25V02_READ(Z_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof c_z), c_z.bytes ,sizeof c_z);

            spark_info->l_array[L_X_CURRENT] = c_x.longs + spark_info->l_array[L_X_ABSOLUTE];
            spark_info->l_array[L_Y_CURRENT] = c_y.longs + spark_info->l_array[L_Y_ABSOLUTE];
            spark_info->l_array[L_Z_CURRENT] = c_z.longs + spark_info->l_array[L_Z_ABSOLUTE];

            emit xyzChange(L_X_CURRENT);
            emit xyzChange(L_Y_CURRENT);
            emit xyzChange(L_Z_CURRENT);

            emit coorIndexChange();
        }
        /*保证当前行号在开始行和结束行之间*/
        else if(i==UINT_CURRENT_ROM){
            if(uint_array[UINT_CURRENT_ROM] < uint_array[UINT_START_ROW]){
                uint_array[UINT_CURRENT_ROM] = uint_array[UINT_START_ROW];
            }
            else if(uint_array[UINT_CURRENT_ROM] > uint_array[UINT_END_ROW]){
                uint_array[UINT_CURRENT_ROM] = uint_array[UINT_END_ROW];
            }
            emit tableRowChange();
        }
        else if(i == UINT_START_ROW){
            uint_array[UINT_END_ROW] = uint_array[UINT_START_ROW];
            uint_array[UINT_CURRENT_ROM] = uint_array[UINT_START_ROW];
            emit tableRowChange();
        }
        /*保证开始行号小于结束行号*/
        else if(i == UINT_END_ROW){
            if(uint_array[UINT_START_ROW] > uint_array[UINT_END_ROW]){
                tmp = uint_array[UINT_START_ROW];
                uint_array[UINT_START_ROW] = uint_array[UINT_END_ROW];
                uint_array[UINT_END_ROW] = tmp;
            }
            uint_array[UINT_CURRENT_ROM] = uint_array[UINT_START_ROW];
            emit tableRowChange();
        }
        /*切换屏幕亮度值*/
        else if(i == UINT_BRIGHTNESS){
            if(backlight_fb > 0){
                ioctl(backlight_fb, (uint_array[UINT_BRIGHTNESS] & 0xf0) >> 4,
                      20 + 20 * (uint_array[UINT_BRIGHTNESS] & 0x0f));
            }
        }
        /*手动操作模式转换*/
        else if(i == UINT_SPEED){
            switch(uint_array[UINT_SPEED]){
            case 10:
                hand.Mode = Hand_Position_MODE;
                hand.Step = 10;
                break;
            case 20:
                hand.Mode = Hand_Velocity_MODE;
                hand.Speed = 200;
                break;
            case 30:
                hand.Mode = Hand_Position_MODE;
                hand.Step = 30;
                break;
            case 40:
                hand.Mode = Hand_Velocity_MODE;
                hand.Speed = 400;
                break;
            case 50:
                hand.Mode = Hand_Position_MODE;
                hand.Step = 50;
                break;
            default:
                break;
            }
        }
        if(check)
            emit uintChange();
    }
}
