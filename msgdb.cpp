#include "msgdb.h"
#include <time.h>
#include <QDebug>

MsgDB::MsgDB() {
    sqlite3_open("./serialport.db",&db);
}
MsgDB::~MsgDB()
{
    if(db)
        sqlite3_close(db);
}
void MsgDB::addTable(const char* table)
{
    char sql[128];
    sprintf(sql,"create table t_port_%s(logt int,data blob)",table);
    sqlite3_exec(db,sql,NULL,NULL,NULL);
}
void MsgDB::addMsg(const char* port,const char* data,int len)
{
    sqlite3_stmt* stmt;
    char sql[128];
    time_t t;
    time(&t);
    sprintf(sql,"insert into t_port_%s(logt,data)values(%d,?)",port,t);
    int ret=sqlite3_prepare(db,sql,strlen(sql),&stmt,NULL);
    if(ret==SQLITE_OK)
    {
        sqlite3_bind_blob(stmt,1,data,len,NULL);
        if(SQLITE_DONE!=sqlite3_step(stmt))
        {
            qDebug()<<"msg error";
        }
        sqlite3_finalize(stmt);
    }
}
