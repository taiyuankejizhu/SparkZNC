#include "sparkinfo.h"
#include "qdebug.h"

SparkInfo::SparkInfo(QObject *parent) :
    QObject(parent)
{

    memcpy(b_array , bool_init ,sizeof b_array);
    memcpy(uint_array , uint_init ,sizeof uint_array);
    memcpy(l_array , long_init ,sizeof l_array);
    memset(c_array ,0x00 ,sizeof c_array);

    carryInit();
    tableInit();

#ifdef ARM
    fm25v02Init();
#endif

    /*当前表的索引改变时，更新表的数据*/
    connect(this ,SIGNAL(tableIndexChange()) ,this ,SLOT(updateTable()));

}

/*初始化铁电芯片中的运行参数*/
void SparkInfo::fm25v02Init()
{
    /*铁电芯片初始化*/
    FM25V02_Init();

    /*读取当前坐标的索引*/
    FourBytes c_axis;
    memset(c_axis.bytes , 0 ,sizeof c_axis);
    FM25V02_READ(CURRENT_AXIS_ADDR , c_axis.bytes ,sizeof c_axis);

    spark_info->uint_array[UINT_COOR_INDEX] = c_axis.uint;

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

    spark_info->l_array[L_X_CURRENT] = c_x.longs;
    spark_info->l_array[L_Y_CURRENT] = c_y.longs;
    spark_info->l_array[L_Z_CURRENT] = c_z.longs;
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

void SparkInfo::tableInit()
{
    int i = 0;
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
    }
    else
        updateTable();
}

/*自动生成表格*/
void SparkInfo::tableAuto(long deep ,unsigned int current,unsigned int area,unsigned int effect)
{
    tableClear();
    unsigned int i = 0;
    for(i = 0;i < effect;i++){
        table.Shendu[i] = deep-(effect-1-i)*100;
        table.Dianliu[i] = current;
        table.Maikuan[i] = 0;
        table.Xiuzhi[i] = 0;
        table.Jianxi[i] = 0;
        table.Sudu[i] = 0;
        table.Shenggao[i] = 0;
        table.Gongshi[i] = 0;
        table.Mianji[i] = area;
        table.Jixing[i] = 0;
        table.Gaoya[i] = 0;
        table.Index[i] = i+1;
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

void SparkInfo::updateTable()
{
    emit tableChange();
}

void SparkInfo::setBool(unsigned int i,bool b)
{
    if(i < B_LENGTH){
        /*布尔数组状态更新*/
        if(i != B_UPDATE && b_array[i] != b)
            b_array[B_UPDATE] = true;
        b_array[i] = b;
        if(i == B_START|| i == B_TIME){
            emit startChange();
        }
        else
            emit boolChange();
    }
}

void SparkInfo::reverseBool(unsigned int i)
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
        }
        else
            emit boolChange();
    }
}

void SparkInfo::setLong(unsigned int i,long l)
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
#ifdef ARM
        EightBytes wr;
        wr.longs = 0;
        wr.longs = spark_info->l_array[i];

        switch(i){
        case L_X_CURRENT:
            FM25V02_WRITE(X_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof wr), wr.bytes, sizeof wr);
            break;
        case L_Y_CURRENT:
            FM25V02_WRITE(Y_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof wr), wr.bytes, sizeof wr);
            break;
        case L_Z_CURRENT:
            FM25V02_WRITE(Z_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof wr), wr.bytes, sizeof wr);
            break;
        default:
            break;
        }
#endif
            }

            emit xyzChange(i);
        }
        else if(i == L_X_ABSOLUTE||i == L_Y_ABSOLUTE||i == L_Z_ABSOLUTE){
            emit xyzChange(i);
        }
        else if(i == L_X_REMAIN||i == L_Y_REMAIN||i == L_Z_REMAIN){
            emit xyzChange(i);
        }
        else if(i == L_X_COUNTER||i == L_Y_COUNTER||i == L_Z_COUNTER){
            /*更新当前位置，Position = Offset + Count*/
            if(i == L_X_COUNTER)
                spark_info->setLong(L_X_CURRENT ,spark_info->l_array[L_X_OFFSET]+spark_info->l_array[L_X_COUNTER]);
            if(i == L_Y_COUNTER)
                spark_info->setLong(L_Y_CURRENT ,spark_info->l_array[L_Y_OFFSET]+spark_info->l_array[L_Y_COUNTER]);
            if(i == L_Z_COUNTER)
                spark_info->setLong(L_Z_CURRENT ,spark_info->l_array[L_Z_OFFSET]+spark_info->l_array[L_Z_COUNTER]);
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

void SparkInfo::setUInt(unsigned int i, unsigned int u)
{
    unsigned int tmp;

    if(i < UINT_LENGTH){
        uint_array[i] = u;
        /*组别变化后，当前行、开始行、结束行重置*/
        if(i == UINT_TAB_INDEX){
            uint_array[UINT_CURRENT_ROM] = 0;
            uint_array[UINT_START_ROW] = 0;
            uint_array[UINT_END_ROW] = 0;
            emit tableIndexChange();
            emit tableRowChange();
        }
        else if(i == UINT_COOR_INDEX){
            /*读取当前坐标*/
            EightBytes  c_x;
            EightBytes  c_y;
            EightBytes  c_z;
            memset(c_x.bytes , 0 ,sizeof c_x);
            memset(c_y.bytes , 0 ,sizeof c_y);
            memset(c_z.bytes , 0 ,sizeof c_z);

#ifdef ARM
            FM25V02_READ(X_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof c_x), c_x.bytes ,sizeof c_x);
            FM25V02_READ(Y_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof c_y), c_y.bytes ,sizeof c_y);
            FM25V02_READ(Z_AXIS_ADDR + spark_info->uint_array[UINT_COOR_INDEX]*3*(sizeof c_z), c_z.bytes ,sizeof c_z);
#endif

            spark_info->setLong(L_X_CURRENT ,c_x.longs);
            spark_info->setLong(L_Y_CURRENT ,c_y.longs);
            spark_info->setLong(L_Z_CURRENT ,c_z.longs);
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
        else
            emit uintChange();
    }
}