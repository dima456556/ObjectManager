#include "crow.h"
#include "SQLiteDataBase.h"
#include "object.h"
#include "sort.h"
#include <memory>
#include <sstream>

// Добавление заголовков CORS
void add_cors_headers(crow::response& res) {
    res.add_header("Access-Control-Allow-Origin", "*");
    res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
    crow::SimpleApp app;

    std::unique_ptr<Database> db;

    // Выбор базы данных
    std::string dbType = "sqlite"; 
    //Можно сделать через переменные окружения например:
    //const char* dbType = std::getenv("DB_TYPE");
    if (dbType == "sqlite") {
        db = std::make_unique<SQLiteDatabase>();
        db->connect("objects.db"); //если файла базы данных нет то он создастся в текущем каталоге
    }

    auto handle_options = [](const crow::request&) {
        crow::response res;
        add_cors_headers(res);
        res.code = 200;
        return res;
        };

    // Обработка запросов добавления объектов
    CROW_ROUTE(app, "/add_object")
        .methods("OPTIONS"_method)
        (handle_options);

    CROW_ROUTE(app, "/add_object")
        .methods("POST"_method)
        ([&db](const crow::request& req) {
        crow::response res;
        add_cors_headers(res);
        res.set_header("Content-Type", "text/html; charset=UTF-8");

        // Парсим данные формы
        std::string body = req.body;
        std::string name, x, y, type;
        std::istringstream body_stream(body);
        std::getline(body_stream, name, '&');
        std::getline(body_stream, x, '&');
        std::getline(body_stream, y, '&');
        std::getline(body_stream, type);

        // Удаляем префиксы `name=`, `x=`, и т.д.
        name = name.substr(name.find('=') + 1);
        x = x.substr(x.find('=') + 1);
        y = y.substr(y.find('=') + 1);
        type = type.substr(type.find('=') + 1);

        // Преобразуем в нужные типы данных
        double x_val = std::stod(x);
        double y_val = std::stod(y);
        auto time = std::time(nullptr);

        Object obj(name, x_val, y_val, type, time);
        db->insertObject(obj);

        res.code = 200;
        res.body = "Object added successfully";
        return res;
            });

    // Обработка запросов получения объектов
    CROW_ROUTE(app, "/get_objects")
        .methods("OPTIONS"_method)
        (handle_options);

    CROW_ROUTE(app, "/get_objects")
        .methods("GET"_method)
        ([&db]() {
        crow::response res;
        add_cors_headers(res);
        res.set_header("Content-Type", "text/html; charset=UTF-8");

        auto objects = db->getObjects();
        std::ostringstream oss;
        for (const auto& obj : objects) {
            oss << "Name: " << obj.name << ", X: " << obj.x << ", Y: " << obj.y << ", Type: " << obj.type << ", Creation Time: " << obj.creationTime << "\n";
        }
        res.body = oss.str();
        return res;
            });

    // Группировка по расстоянию
    CROW_ROUTE(app, "/group_by_distance")
        .methods("OPTIONS"_method)
        (handle_options);

    CROW_ROUTE(app, "/group_by_distance")
        .methods("POST"_method)
        ([&db](const crow::request& req) {
        crow::response res;
        add_cors_headers(res);

        auto objects = db->getObjects();
        auto groupedObjects = groupByDistance(objects);

        std::ostringstream oss;
        for (const auto& [groupName, groupObjects] : groupedObjects) {
            oss << "<h3>Group: " << groupName << "</h3>\n<ul>\n";
            for (const auto& obj : groupObjects) {
                oss << "<li>Name: " << obj.name << ", X: " << obj.x << ", Y: " << obj.y << ", Type: " << obj.type << ", Creation Time: " << obj.creationTime << "</li>\n";
            }
            oss << "</ul>\n";
        }

        res.body = oss.str();
        return res;
            });

    // Группировка по имени
    CROW_ROUTE(app, "/group_by_name")
        .methods("OPTIONS"_method)
        (handle_options);

    CROW_ROUTE(app, "/group_by_name")
        .methods("POST"_method)
        ([&db](const crow::request& req) {
        crow::response res;
        add_cors_headers(res);

        auto objects = db->getObjects();
        auto groupedObjects = groupByName(objects);

        std::ostringstream oss;
        for (const auto& [groupName, groupObjects] : groupedObjects) {
            oss << "<h3>Group: " << groupName << "</h3>\n<ul>\n";
            for (const auto& obj : groupObjects) {
                oss << "<li>Name: " << obj.name << ", X: " << obj.x << ", Y: " << obj.y << ", Type: " << obj.type << ", Creation Time: " << obj.creationTime << "</li>\n";
            }
            oss << "</ul>\n";
        }

        res.body = oss.str();
        return res;
            });

    // Группировка по типу
    CROW_ROUTE(app, "/group_by_type")
        .methods("OPTIONS"_method)
        (handle_options);

    CROW_ROUTE(app, "/group_by_type")
        .methods("POST"_method)
        ([&db](const crow::request& req) {
        crow::response res;
        add_cors_headers(res);
        res.set_header("Content-Type", "text/html; charset=UTF-8");

        std::string body = req.body;
        std::string threshold_str;
        std::istringstream body_stream(body);
        std::getline(body_stream, threshold_str, '&');

        threshold_str = threshold_str.substr(threshold_str.find('=') + 1);
        int threshold = std::stoi(threshold_str);

        auto objects = db->getObjects();
        auto groupedObjects = groupByType(objects, threshold);

        std::ostringstream oss;
        for (const auto& [groupName, groupObjects] : groupedObjects) {
            oss << "<h3>Group: " << groupName << "</h3>\n<ul>\n";
            for (const auto& obj : groupObjects) {
                oss << "<li>Name: " << obj.name << ", X: " << obj.x << ", Y: " << obj.y << ", Type: " << obj.type << ", Creation Time: " << obj.creationTime << "</li>\n";
            }
            oss << "</ul>\n";
        }

        res.body = oss.str();
        return res;
            });

    // Группировка по времени создания
    CROW_ROUTE(app, "/group_by_time")
        .methods("OPTIONS"_method)
        (handle_options);

    CROW_ROUTE(app, "/group_by_time")
        .methods("POST"_method)
        ([&db](const crow::request& req) {
        crow::response res;
        add_cors_headers(res);

        auto objects = db->getObjects();
        auto groupedObjects = groupByCreationTime(objects);

        std::ostringstream oss;
        for (const auto& [groupName, groupObjects] : groupedObjects) {
            oss << "<h3>Group: " << groupName << "</h3>\n<ul>\n";
            for (const auto& obj : groupObjects) {
                oss << "<li>Name: " << obj.name << ", X: " << obj.x << ", Y: " << obj.y << ", Type: " << obj.type << ", Creation Time: " << obj.creationTime << "</li>\n";
            }
            oss << "</ul>\n";
        }

        res.body = oss.str();
        return res;
            });

    app.port(8080).multithreaded().run();

    db->disconnect();
    return 0;
}
