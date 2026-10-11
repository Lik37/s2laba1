#pragma once

#include "database.h"

#include <string>
#include <vector>

void load(const std::string& filePath, Database& db);
void save(const std::string& filePath, const Database& db);
std::vector<std::string> parse(const std::string& query);
Command toCommand(const std::string& token);