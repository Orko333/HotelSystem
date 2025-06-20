#pragma once

#include "models/User.h"
#include <string>
#include <vector>
#include <optional>
#include <memory>

namespace SQLite {
    class Database;
}

// DAO для операцій з користувачами
class UserDAO {
public:
    explicit UserDAO(SQLite::Database& db);

    // Створює користувача в БД
    void save(const User& user);
    
    // Оновлює дані користувача в БД
    void update(const User& user);

    // Видаляє користувача з БД
    void remove(int id);

    // Пошук користувача за іменем
    std::optional<User> findByUsername(const std::string& username);

    // Пошук користувача за ID
    std::optional<User> findById(int id);
    
    // Отримання всіх користувачів
    std::vector<User> findAll();

private:
    SQLite::Database& db;
}; 