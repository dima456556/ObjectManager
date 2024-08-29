#pragma once
#include <vector>
#include <string>

class Object;

//интерфейс позволит использовать разные базы данных
class Database {
public:
    virtual void connect(const std::string& connectionString) = 0;
    virtual void disconnect() = 0;
    virtual void insertObject(const Object& obj) = 0;
    virtual std::vector<Object> getObjects() = 0;
    virtual ~Database() {}
};
