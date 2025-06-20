#include "dao/UserDAO.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

UserDAO::UserDAO(SQLite::Database& db) : db(db) {}

void UserDAO::save(const User& user) {
    try {
        SQLite::Statement query(db, "INSERT INTO users (username, hashed_password, role) VALUES (?, ?, ?)");
        query.bind(1, user.username);
        query.bind(2, static_cast<long long>(user.hashedPassword));
        query.bind(3, static_cast<int>(user.role));
        query.exec();
    } catch (const std::exception& e) {
        std::cerr << "Помилка збереження користувача: " << e.what() << std::endl;
        throw;
    }
}

void UserDAO::update(const User& user) {
    try {
        SQLite::Statement query(db, "UPDATE users SET username = ?, hashed_password = ?, role = ? WHERE id = ?");
        query.bind(1, user.username);
        query.bind(2, static_cast<long long>(user.hashedPassword));
        query.bind(3, static_cast<int>(user.role));
        query.bind(4, user.id);
        query.exec();
    } catch (const std::exception& e) {
        std::cerr << "Помилка оновлення користувача: " << e.what() << std::endl;
        throw;
    }
}

void UserDAO::remove(int id) {
    try {
        SQLite::Statement query(db, "DELETE FROM users WHERE id = ?");
        query.bind(1, id);
        query.exec();
    } catch (const std::exception& e) {
        std::cerr << "Помилка видалення користувача: " << e.what() << std::endl;
        throw;
    }
}

std::optional<User> UserDAO::findByUsername(const std::string& username) {
    try {
        SQLite::Statement query(db, "SELECT id, username, hashed_password, role FROM users WHERE username = ?");
        query.bind(1, username);

        if (query.executeStep()) {
            return std::make_optional<User>(
                query.getColumn(0).getInt(),
                query.getColumn(1).getString(),
                static_cast<size_t>(query.getColumn(2).getInt64()),
                static_cast<UserRole>(query.getColumn(3).getInt())
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку користувача за іменем: " << e.what() << std::endl;
        throw;
    }
    return std::nullopt;
}

std::optional<User> UserDAO::findById(int id) {
    try {
        SQLite::Statement query(db, "SELECT id, username, hashed_password, role FROM users WHERE id = ?");
        query.bind(1, id);

        if (query.executeStep()) {
            return std::make_optional<User>(
                query.getColumn(0).getInt(),
                query.getColumn(1).getString(),
                static_cast<size_t>(query.getColumn(2).getInt64()),
                static_cast<UserRole>(query.getColumn(3).getInt())
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку користувача за ID: " << e.what() << std::endl;
        throw;
    }
    return std::nullopt;
}

std::vector<User> UserDAO::findAll() {
    std::vector<User> users;
    try {
        SQLite::Statement query(db, "SELECT id, username, hashed_password, role FROM users");
        while (query.executeStep()) {
            users.emplace_back(
                query.getColumn(0).getInt(),
                query.getColumn(1).getString(),
                static_cast<size_t>(query.getColumn(2).getInt64()),
                static_cast<UserRole>(query.getColumn(3).getInt())
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка отримання всіх користувачів: " << e.what() << std::endl;
        throw;
    }
    return users;
} 