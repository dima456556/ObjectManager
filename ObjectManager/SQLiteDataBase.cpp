#include "SQLiteDataBase.h"
#include "sqlite3.h"
#include "DataBase.h"
#include "Object.h"

void SQLiteDatabase::connect(const std::string& connectionString){
    sqlite3_open(connectionString.c_str(), &db);
    std::string sql = "CREATE TABLE IF NOT EXISTS objects ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT, x REAL, y REAL, type TEXT, creation_time INTEGER);";
    sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
}

void SQLiteDatabase::disconnect() {
    sqlite3_close(db);
}

void SQLiteDatabase::insertObject(const Object& obj){
    std::string sql = "INSERT INTO objects (name, x, y, type, creation_time) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, obj.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, obj.x);
    sqlite3_bind_double(stmt, 3, obj.y);
    sqlite3_bind_text(stmt, 4, obj.type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 5, obj.creationTime);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

std::vector<Object> SQLiteDatabase::getObjects(){
    std::vector<Object> objects;
    std::string sql = "SELECT name, x, y, type, creation_time FROM objects;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        double x = sqlite3_column_double(stmt, 1);
        double y = sqlite3_column_double(stmt, 2);
        std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::time_t creationTime = sqlite3_column_int64(stmt, 4);
        objects.emplace_back(name, x, y, type, creationTime);
    }
    sqlite3_finalize(stmt);
    return objects;
}
