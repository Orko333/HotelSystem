#pragma once

#include <string>
#include "models/Enums.h"

// Оголошення спільних функцій

void clearConsole();
void pauseConsole();
std::string roleToString(UserRole role);
std::string getConfigValue(const std::string& key); 