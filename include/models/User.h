#pragma once

#include <string>
#include <utility>
#include <cstddef>
#include "models/Enums.h"

// Клас, що представляє користувача системи
class User {
public:
    int id;
    std::string username;
    size_t hashedPassword; // Використовуємо size_t для хешу
    UserRole role;

    // Конструктор для існуючих користувачів з БД
    User(int id, std::string username, size_t hashedPassword, UserRole role)
        : id(id), username(std::move(username)), hashedPassword(hashedPassword), role(role) {}

    // Конструктор для нових користувачів, яких ще немає в БД
    User(std::string username, size_t hashedPassword, UserRole role)
        : id(-1), username(std::move(username)), hashedPassword(hashedPassword), role(role) {}
}; 