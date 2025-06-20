#pragma once

#include <string>
#include <memory>
#include <SQLiteCpp/SQLiteCpp.h>

// Клас-одинак (Singleton) для управління з'єднанням з базою даних
class DatabaseManager {
public:
    // Видаляємо конструктори копіювання та оператори присвоєння
    DatabaseManager(const DatabaseManager&) = delete;
    void operator=(const DatabaseManager&) = delete;

    // Метод для отримання єдиного екземпляра класу
    static DatabaseManager& getInstance();

    // Метод для підключення до БД та ініціалізації схеми
    void connect(const std::string& dbPath);

    // Метод для отримання доступу до об'єкта БД
    SQLite::Database& getDb();

private:
    // Приватний конструктор та деструктор
    DatabaseManager() = default;
    ~DatabaseManager() = default;

    // Метод для створення таблиць у БД
    void initSchema();

    // Єдиний екземпляр з'єднання з БД
    std::unique_ptr<SQLite::Database> db;
}; 