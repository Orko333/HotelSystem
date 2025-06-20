#include "logic/AuthenticationManager.h"
#include "utils.h"
#include <iostream>
#include <functional> // для std::hash

AuthenticationManager::AuthenticationManager(std::shared_ptr<UserDAO> uDAO) : userDAO(std::move(uDAO)) {}

bool AuthenticationManager::login(const std::string& username, const std::string& password) {
    auto userOpt = userDAO->findByUsername(username);
    if (userOpt) {
        // Хешуємо введений пароль для порівняння
        size_t passwordHash = std::hash<std::string>{}(password);
        if (userOpt->hashedPassword == passwordHash) {
            currentUser = userOpt;
            std::cout << "Вхід успішний. Вітаємо, " << currentUser->username << "!" << std::endl;
            return true;
        }
    }
    std::cout << "Невірний логін або пароль." << std::endl;
    return false;
}

void AuthenticationManager::logout() {
    if (currentUser) {
        std::cout << "Користувач " << currentUser->username << " вийшов з системи." << std::endl;
        currentUser.reset();
    }
}

bool AuthenticationManager::createUser(const std::string& username, const std::string& password, UserRole role) {
    // Перевірка, чи користувач вже існує
    if (userDAO->findByUsername(username)) {
        std::cerr << "Помилка: Користувач з іменем '" << username << "' вже існує." << std::endl;
        return false;
    }

    try {
        size_t passwordHash = std::hash<std::string>{}(password);
        User newUser(username, passwordHash, role);
        userDAO->save(newUser);
        std::cout << "Користувача '" << username << "' успішно створено." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Не вдалося створити користувача: " << e.what() << std::endl;
        return false;
    }
}

bool AuthenticationManager::updateUser(int userId, const std::string& newUsername, const std::string& newPassword, UserRole newRole) {
    auto userOpt = userDAO->findById(userId);
    if (!userOpt) {
        std::cerr << "Помилка: Користувача з ID " << userId << " не знайдено." << std::endl;
        return false;
    }

    User userToUpdate = *userOpt;
    userToUpdate.username = newUsername;
    userToUpdate.role = newRole;

    // Оновлюємо пароль, тільки якщо його було введено
    if (!newPassword.empty()) {
        userToUpdate.hashedPassword = std::hash<std::string>{}(newPassword);
    }

    try {
        userDAO->update(userToUpdate);
        std::cout << "Дані користувача з ID " << userId << " успішно оновлено." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Не вдалося оновити дані користувача: " << e.what() << std::endl;
        return false;
    }
}

bool AuthenticationManager::deleteUser(int userId) {
    // Заборона видалення самого себе або адміна з ID 1
    if (currentUser && currentUser->id == userId) {
        std::cerr << "Помилка: Ви не можете видалити самого себе." << std::endl;
        return false;
    }
    if (userId == 1) { // Захист головного адміна
        std::cerr << "Помилка: Ви не можете видалити головного адміністратора." << std::endl;
        return false;
    }
    
    try {
        userDAO->remove(userId);
        std::cout << "Користувача з ID " << userId << " успішно видалено." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Не вдалося видалити користувача: " << e.what() << std::endl;
        return false;
    }
}

void AuthenticationManager::listAllUsers() {
    auto users = userDAO->findAll();
    std::cout << "\n--- Список всіх користувачів ---\n";
    if (users.empty()) {
        std::cout << "Користувачі не знайдені.\n";
        return;
    }
    for (const auto& user : users) {
        std::cout << "ID: " << user.id
                  << ", Логін: " << user.username
                  << ", Роль: " << roleToString(user.role) << "\n";
    }
}

bool AuthenticationManager::userExists(const std::string& username) {
    return userDAO->findByUsername(username).has_value();
}

std::optional<User> AuthenticationManager::getCurrentUser() const {
    return currentUser;
} 