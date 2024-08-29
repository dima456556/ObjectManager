#pragma once
#include <vector>
#include <map>
#include <string>

class Object;

std::map<std::string, std::vector<Object>> groupByDistance(const std::vector<Object>& objects);

std::map<std::string, std::vector<Object>> groupByName(const std::vector<Object>& objects);

std::map<std::string, std::vector<Object>> groupByType(const std::vector<Object>& objects, int threshold);

std::map<std::string, std::vector<Object>> groupByCreationTime(const std::vector<Object>& objects);