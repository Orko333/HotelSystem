#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <sstream>

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseConsole() {
    std::cout << "\nНатисніть Enter, щоб продовжити...";
    // Очищуємо буфер перед очікуванням вводу
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string roleToString(UserRole role) {
    switch (role) {
        case UserRole::ADMINISTRATOR: return "Адміністратор";
        case UserRole::MANAGER:       return "Менеджер";
        case UserRole::RECEPTIONIST:  return "Реєстратор";
        case UserRole::HOUSEKEEPING:  return "Обслуговування";
        case UserRole::GUEST:         return "Гість";
        default:                      return "Невідома роль";
    }
}

std::string getConfigValue(const std::string& key) {
    std::ifstream configFile("config.ini");
    std::string line;
    std::string foundValue = "";

    if (configFile.is_open()) {
        while (getline(configFile, line)) {
            std::stringstream ss(line);
            std::string currentKey;
            std::string currentValue;

            if (getline(ss, currentKey, '=') && getline(ss, currentValue)) {
                if (currentKey == key) {
                    foundValue = currentValue;
                    break;
                }
            }
        }
        configFile.close();
    } else {
        std::cerr << "Не вдалося відкрити файл конфігурації 'config.ini'" << std::endl;
    }

    return foundValue;
} 