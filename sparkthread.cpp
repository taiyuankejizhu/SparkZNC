#include "sparkthread.h"
#include "sparkinfo.h"
#include "qdebug.h"

SparkThread::SparkThread(QObject *parent) :
    QThread(parent)
{
    /*状态机初始状态为SEARCH*/
    state = SEARCH;

    x_start = 0;
    y_start = 0;
    z_start = 0;

}

void SparkThread::run()
{
    /*
    在没有开始放电加工或放电加工结束之后，当前深度值（L_DEEP_CURRENT）与当前Z轴位置（L_Z_CURRENT）保持相同。
    放电加工开始后当前深度值（L_DEEP_CURRENT）应为放电加工的最深深度。而目标深度（L_DEEP_TARGET）在没有放电
    开始时或放电结束时也应与当前Z轴位置（L_Z_CURRENT）保持相同，当放电开始后应为当前放电行的深度参数。

    当停止放电时，当前深度值（L_DEEP_CURRENT）与当前Z轴位置（L_Z_CURRENT）保持相同。当前的目标深度（L_DEEP_TARGET）
    与当前Z轴位置（L_Z_CURRENT）保持相同。且状态机恢复到初始状态（SEARCH）。
    */

    x_start = spark_info->l_array[L_X_CURRENT];
    y_start = spark_info->l_array[L_Y_CURRENT];
    z_start = spark_info->l_array[L_Z_CURRENT];

    state = SEARCH;

    while(spark_info->b_array[B_START]){

        if(spark_info->l_array[L_DEEP_CURRENT] >= spark_info->table.Shendu[spark_info->uint_array[UINT_CURRENT_ROM]]){

            if(spark_info->uint_array[UINT_CURRENT_ROM] < spark_info->uint_array[UINT_END_ROW]){
                spark_info->setUInt(UINT_CURRENT_ROM ,++spark_info->uint_array[UINT_CURRENT_ROM]);
                spark_info->setLong(L_DEEP_TARGET ,spark_info->table.Shendu[spark_info->uint_array[UINT_CURRENT_ROM]]);
            }
            else{
                //spark_info->setUInt(UINT_CURRENT_ROM ,spark_info->uint_array[UINT_END_ROW]);
                spark_info->setUInt(UINT_CURRENT_ROM ,TABLE_ROWS);
                spark_info->setUInt(UINT_END_ROW ,TABLE_ROWS);
                spark_info->setUInt(UINT_START_ROW ,TABLE_ROWS);
                spark_info->setLong(L_DEEP_TARGET ,spark_info->table.Shendu[spark_info->uint_array[UINT_CURRENT_ROM]]);

                spark_info->reverseBool(B_START);
                break;
            }

        }

        switch(state){
        case SEARCH:
            state = WORK;
            break;
        case WORK:
            state = UP;
            break;
        case UP:
            state = DOWN;
            break;
        case DOWN:
            state = WORK;
            break;
        case END:
            break;
        default:
            break;
        }

        spark_info->setLong(L_DEEP_CURRENT ,++spark_info->l_array[L_DEEP_CURRENT]);
        msleep(200);
    }
    /*todo some thing*/
}

/*放电进程监听spark_info全局变量的开始结束信号*/
void SparkThread::sparkChange()
{
    if(spark_info->b_array[B_START]&&!isRunning()){
        start();
    }
    else if(!spark_info->b_array[B_START]&&isRunning()){
    }
}

SparkThread::~SparkThread()
{

}
