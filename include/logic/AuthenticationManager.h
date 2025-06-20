#pragma once

#include "dao/UserDAO.h"
#include "models/User.h"
#include <memory>
#include <string>
#include <optional>

// Клас для управління автентифікацією та сесіями користувачів
class AuthenticationManager {
public:
    // Конструктор приймає DAO через dependency injection
    explicit AuthenticationManager(std::shared_ptr<UserDAO> userDAO);

    // Спроба входу в систему
    bool login(const std::string& username, const std::string& password);

    // Вихід з системи
    void logout();

    // Створення нового користувача
    bool createUser(const std::string& username, const std::string& password, UserRole role);

    // Оновлення даних користувача
    bool updateUser(int userId, const std::string& newUsername, const std::string& newPassword, UserRole newRole);

    // Видалення користувача
    bool deleteUser(int userId);

    // Отримання списку всіх користувачів
    void listAllUsers();

    // Перевірка чи користувач існує
    bool userExists(const std::string& username);

    // Отримання поточного активного користувача
    std::optional<User> getCurrentUser() const;

private:
    std::shared_ptr<UserDAO> userDAO;
    std::optional<User> currentUser; // Зберігає дані про поточного користувача
}; 