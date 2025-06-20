#pragma once

#include "models/Enums.h"
#include <iostream>
#include <vector>
#include <string>
#include <functional>

// Представляє один пункт меню
class MenuItem {
public:
    std::string name;
    std::function<void()> action;
    UserRole requiredRole;

    MenuItem(std::string name, std::function<void()> action, UserRole requiredRole)
        : name(std::move(name)), action(std::move(action)), requiredRole(requiredRole) {}

    // Перевіряє, чи має користувач достатньо прав для цього пункту
    bool canExecute(UserRole userRole) const {
        return static_cast<int>(userRole) <= static_cast<int>(requiredRole);
    }
};

// Клас, що керує меню
class Menu {
public:
    Menu() = default;

    // Додає новий пункт в меню
    void addItem(const std::string& name, const std::function<void()>& action, UserRole requiredRole) {
        items.emplace_back(name, action, requiredRole);
    }

    // Запускає головний цикл меню
    void run(UserRole currentUserRole);

private:
    std::vector<MenuItem> items;
}; 