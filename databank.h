#ifndef DATABANK_H
#define DATABANK_H

#include <QtSql>
#include <QList>
#include "setting.h"
#include "sparkinfo.h"

class DataBank
{
public:
    DataBank();
    QList<int> listTable();
    bool checkExists(unsigned int);
    bool createTable(unsigned int);
    void tableSave(struct Table0 *,unsigned int);
    void tableLoad(struct Table0 *t,unsigned int);
    void tableMake(LONG64 ,UNINT32 ,UNINT32 ,UNINT32 ,UNINT32  ,UNINT32 ,UNINT32 );

    /*保存数据表的数据库*/
    QSqlDatabase database;
};

#endif // DATABANK_H
