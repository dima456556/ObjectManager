#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <ctime>

class Object {
public:
    std::string name;
    double x, y;
    std::string type;
    std::time_t creationTime;

    Object(const std::string& name, double x, double y, const std::string& type, std::time_t creationTime)
        : name(name), x(x), y(y), type(type), creationTime(creationTime) {}

    double distanceFromOrigin() const {
        return std::sqrt(x * x + y * y);
    }
};
