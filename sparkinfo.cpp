#include "sparkinfo.h"
#include "qdebug.h"

SparkInfo::SparkInfo(QObject *parent) :
    QObject(parent)
{

    memcpy(b_array , bool_init ,sizeof b_array);
    memcpy(uint_array , uint_init ,sizeof uint_array);
    memcpy(l_array , long_init ,sizeof l_array);
    memcpy(c_array , char_init ,sizeof c_array);
    tableInit();

    /*当前表的索引改变时，更新表的数据*/
    connect(this ,SIGNAL(tableIndexChange()) ,this ,SLOT(updateTable()));

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
            table.Jixin[i] = table_init.Jixin[i];
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
        table.Jixin[i] = 0;
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
        table.Jixin[i] = 0;
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
        if(i != 0)
            b_array[0] = true;
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
        if(i != 0)
            b_array[0] = true;
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
    if(i < L_LENGTH){
        l_array[i] = l;
        if(i == L_X_CURRENT||i == L_Y_CURRENT||i == L_Z_CURRENT){
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
        if(i == UINT_TAB_INDEX){
            uint_array[UINT_CURRENT_ROM] = 0;
            uint_array[UINT_START_ROW] = 0;
            uint_array[UINT_END_ROW] = 0;
            emit tableIndexChange();
            emit tableRowChange();
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
