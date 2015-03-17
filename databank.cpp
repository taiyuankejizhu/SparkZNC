#include "databank.h"

DataBank::DataBank()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(DATABASE);
}

QList<int> DataBank::listTable()
{
    QList<int> list;
    bool ok = false;

    if(!database.open()){
        qDebug()<<"database database is error";
    }else{
        qDebug()<<"database database is ok";
    }

    /*查询数据库中的所有表名*/
    QSqlQueryModel model;
    model.setQuery("select name from sqlite_master where type='table'",database);

    for(int i = 0; i < model.rowCount(); ++i){
        QString name = model.record(i).value("name").toString();
        list.append(name.mid(5,2).toInt(&ok ,10));
    }

    database.close();

    return list;
}

bool DataBank::checkExists(unsigned int i)
{
    bool bit = false;
    bool ok = false;

    if(!database.open()){
        qDebug()<<"database database is error";
    }else{
        qDebug()<<"database database is ok";
    }

    /*查询数据库中的所有表名*/
    QSqlQueryModel model;
    model.setQuery("select name from sqlite_master where type='table'",database);

    for(int j = 0; j < model.rowCount(); ++j){
        QString name = model.record(j).value("name").toString();
        if((unsigned int)name.mid(5,2).toInt(&ok ,10) == i){
            bit = true;
        }
    }

    database.close();
    return bit;
}

bool DataBank::createTable(unsigned int i)
{
    bool bit = true;
    QString name = "table";
    name.append(QString::number(i ,10));
    QString sql = QString("create table '%1' (id integer primary key,number integer,"
                          "shendu integer,jixing integer,dianliu integer,"
                          "maikuan integer,xiuzhi integer,jianxi integer,"
                          "sudu integer,shenggao integer,gongshi integer,"
                          "lv integer,gaoya integer,ppp integer,wclc integer)").arg(name);

    if(!database.open()){
        qDebug()<<"database database is error";
    }else{
        qDebug()<<"database database is ok";
    }

    QSqlQuery query(database);
    bit = query.exec(sql);   //创建一个表

    database.close();
    return bit;
}

void DataBank::tableSave(struct Table0 *t,unsigned int i)
{
    bool bit = true;
    QString name = "table";
    name.append(QString::number(i ,10));
    QString sql = QString("insert into '%1' (number, shendu, jixing"
                          ", dianliu, maikuan, xiuzhi, jianxi, sudu, shenggao"
                          ", gongshi, lv, gaoya, ppp, wclc)"
                          "VALUES (:number, :shendu, :jixing, :dianliu"
                          ", :maikuan, :xiuzhi, :jianxi, :sudu, :shenggao"
                          ", :gongshi, :lv, :gaoya, :ppp, :wclc)").arg(name);

    if(!database.open()){
        qDebug()<<"database database is error";
    }else{
        qDebug()<<"database database is ok";
    }

    QSqlQuery query(database);
    for(int j = 0;j < 10;j++){
        if(t->Nummber[j] != 0){
            bit = query.prepare(sql);
            query.bindValue(":number", t->Nummber[j]);
            query.bindValue(":shendu", t->Shendu[j]);
            query.bindValue(":jixing", t->Jixing[j]);
            query.bindValue(":dianliu", t->Dianliu[j]);
            query.bindValue(":maikuan", t->Maikuan[j]);
            query.bindValue(":xiuzhi", t->Xiuzhi[j]);
            query.bindValue(":jianxi", t->Jianxi[j]);
            query.bindValue(":sudu", t->Sudu[j]);
            query.bindValue(":shenggao", t->Shenggao[j]);
            query.bindValue(":gongshi", t->Gongshi[j]);
            query.bindValue(":lv", t->LV[j]);
            query.bindValue(":gaoya", t->Gaoya[j]);
            query.bindValue(":ppp", t->PPP[j]);
            query.bindValue(":wclc", t->WCLC[j]);
            query.exec();
        }
    }

    database.close();
}

void DataBank::tableLoad(struct Table0 *t,unsigned int i)
{
    QString name = "table";
    name.append(QString::number(i ,10));
    QString sql = QString("select * from '%1'").arg(name);

    if(!database.open()){
        qDebug()<<"database database is error";
    }else{
        qDebug()<<"database database is ok";
    }

    QSqlQueryModel model;
    model.setQuery(sql,database);

    for(int i = 0; i < model.rowCount(); ++i){
        t->Nummber[i] = model.record(i).value("number").toInt();
        t->Shendu[i] = model.record(i).value("shendu").toLongLong();
        t->Jixing[i] = model.record(i).value("jixing").toInt();
        t->Dianliu[i] = model.record(i).value("dianliu").toInt();
        t->Maikuan[i] = model.record(i).value("maikuan").toInt();
        t->Xiuzhi[i] = model.record(i).value("xiuzhi").toInt();
        t->Jianxi[i] = model.record(i).value("jianxi").toInt();
        t->Sudu[i] = model.record(i).value("sudu").toInt();
        t->Shenggao[i] = model.record(i).value("shenggao").toInt();
        t->Gongshi[i] = model.record(i).value("gongshi").toInt();
        t->LV[i] = model.record(i).value("lv").toInt();
        t->Gaoya[i] = model.record(i).value("gaoya").toInt();
        t->PPP[i] = model.record(i).value("ppp").toInt();
        t->WCLC[i] = model.record(i).value("wclc").toInt();
    }

    database.close();
}

void DataBank::tableMake(LONG64 ideep,UNINT32 imaterial,UNINT32 iacreage,UNINT32 ieffect,UNINT32 iry ,UNINT32 iovc,UNINT32 ishape)
{
    bool ok = false;
    QString sql = QString("select * from '%1'");
    QString name = "";
    if(imaterial == 0){
        name = "cust";
    }else{
        name = "crst";
    }
    name.append(QString::number(ishape,10));
    sql.arg(name);

    if(!database.open()){
        qDebug()<<"spark database is error";
    }else{
        qDebug()<<"spark database is ok";
    }

    QSqlQueryModel model;
    model.setQuery(sql,database);

    for(int i = 0; i < model.rowCount(); ++i){
        int id = model.record(i).value("h_area").toInt();
        QString name = model.record(i).value("no").toString();
        qDebug() << id << name;
    }

/*
    QSqlQuery query(database);
    query.prepare("select * from cust0 where h_area > :value1 and l_area < :value2 and cnf = 0");
    query.bindValue(":value1" ,2);
    query.bindValue(":value2" ,2);
    b = query.exec();
    if(!b){
        qDebug()<<"table is error";
    }else{
        qDebug()<<"table is ok";
        while(query.next()){
            qDebug()<<query.value(S_OFS).toString()<<query.value(PP).toString();
        }
    }
*/

    database.close();
}
