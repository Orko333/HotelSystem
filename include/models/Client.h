#pragma once
#include <string>
#include <utility>

// Клас, що представляє клієнта готелю
class Client {
public:
    int id;
    std::string fullName;
    std::string contactInfo;

    // Конструктор для існуючих клієнтів
    Client(int id, std::string fullName, std::string contactInfo)
        : id(id), fullName(std::move(fullName)), contactInfo(std::move(contactInfo)) {}

    // Конструктор для нових клієнтів
    Client(std::string fullName, std::string contactInfo)
        : id(-1), fullName(std::move(fullName)), contactInfo(std::move(contactInfo)) {}
}; 