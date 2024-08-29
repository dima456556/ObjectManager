#pragma once

#include "sqlite3.h"
#include <string>
#include <vector>
#include "DataBase.h"

class Object;

class SQLiteDatabase : public Database {
private:
    sqlite3* db;

public:
    void connect(const std::string& connectionString) override;
    void disconnect() override;
    void insertObject(const Object& obj) override;
    std::vector<Object> getObjects() override;
};
