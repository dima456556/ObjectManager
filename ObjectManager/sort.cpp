#include "sort.h"
#include <map>
#include <string>
#include "Object.h"
#include <algorithm>
#include <ctime>

std::map<std::string, std::vector<Object>> groupByDistance(const std::vector<Object>& objects) {
    std::map<std::string, std::vector<Object>> groups;

    for (const auto& obj : objects) {
        double dist = obj.distanceFromOrigin();
        std::string groupName;

        if (dist < 100) {
            groupName = "Up to 100 units";
        }
        else if (dist < 1000) {
            groupName = "Up to 1000 units";
        }
        else if (dist < 10000) {
            groupName = "Up to 10000 units";
        }
        else {
            groupName = "Too far away";
        }
        groups[groupName].push_back(obj);
    }

    //Сортировка внутри групп по дистанции
    for (auto& [groupName, groupObjects] : groups) {
        std::sort(groupObjects.begin(), groupObjects.end(), [](const Object& a, const Object& b) {
            return a.distanceFromOrigin() < b.distanceFromOrigin();
            });
    }

    return groups;
}


std::map<std::string, std::vector<Object>> groupByName(const std::vector<Object>& objects) {
    std::map<std::string, std::vector<Object>> groups;

    for (const auto& obj : objects) {
        char firstLetter = std::toupper(obj.name[0]);
        std::string groupName;

        if (std::isalpha(firstLetter)) {
            groupName = std::string(1, firstLetter);
        }
        else {
            groupName = "#";
        }
        groups[groupName].push_back(obj);
    }
    
    //сортировка внутри групп по имени
    for (auto& [groupName, groupObjects] : groups) {
        std::sort(groupObjects.begin(), groupObjects.end(), [](const Object& a, const Object& b) {
            return a.name < b.name;
            });
    }

    return groups;
}


std::map<std::string, std::vector<Object>> groupByType(const std::vector<Object>& objects, int threshold) {
    std::map<std::string, std::vector<Object>> groups;
    std::vector<Object> other;

    std::map<std::string, int> typeCounts;
    for (const auto& obj : objects) {
        typeCounts[obj.type]++;
    }

    for (const auto& obj : objects) {
        if (typeCounts[obj.type] >= threshold) {
            groups[obj.type].push_back(obj);
        }
        else {
            other.push_back(obj);
        }
    }

    //сортировка внутри групп по имени
    for (auto& [groupName, groupObjects] : groups) {
        std::sort(groupObjects.begin(), groupObjects.end(), [](const Object& a, const Object& b) {
            return a.name < b.name;
            });
    }

    if (!other.empty()) {
        groups["Other"] = other;
    }

    return groups;
}


std::map<std::string, std::vector<Object>> groupByCreationTime(const std::vector<Object>& objects) {
    std::map<std::string, std::vector<Object>> groups;
    std::time_t now = std::time(nullptr);
    std::tm tmNow;
    gmtime_s(&tmNow, &now);

    for (const auto& obj : objects) {
        std::tm tmCreation;
        gmtime_s(&tmCreation, &obj.creationTime);
        std::string groupName;

        if (tmCreation.tm_year == tmNow.tm_year) {
            if (tmCreation.tm_mon == tmNow.tm_mon) {
                if (tmCreation.tm_mday == tmNow.tm_mday) {
                    groupName = "Today";
                }
                else if (tmNow.tm_mday - tmCreation.tm_mday == 1) {
                    groupName = "Yesterday";
                }
                else {
                    groupName = "For this week";
                }
            }
            else {
                groupName = "This month";
            }
        }
        else {
            groupName = "This year";
        }
        groups[groupName].push_back(obj);
    }
    
    //сортировка внутри групп по времени создания
    for (auto& [groupName, groupObjects] : groups) {
        std::sort(groupObjects.begin(), groupObjects.end(), [](const Object& a, const Object& b) {
            return a.creationTime < b.creationTime;
            });
    }

    return groups;
}

