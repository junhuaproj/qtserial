#ifndef MSGDB_H
#define MSGDB_H

#include "sqlite3.h"

class MsgDB
{
public:
    MsgDB();
    virtual ~MsgDB();

    void addTable(const char* table);
    void addMsg(const char* port,const char* data,int len);
protected:
    sqlite3* db;
};

#endif // MSGDB_H
