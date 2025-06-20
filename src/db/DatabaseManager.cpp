#include "db/DatabaseManager.h"
#include <iostream>
#include <stdexcept>
#include <filesystem>

// Статичний метод для отримання екземпляра
DatabaseManager& DatabaseManager::getInstance() {
    static DatabaseManager instance;
    return instance;
}

// Підключення до БД
void DatabaseManager::connect(const std::string& dbPath) {
    try {
        // Перевіряємо, чи існує директорія
        std::filesystem::path path(dbPath);
        std::filesystem::path dir = path.parent_path();
        
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            std::cout << "Створюємо директорію: " << dir.string() << std::endl;
            std::filesystem::create_directories(dir);
        }

        // Перевіряємо права доступу
        if (std::filesystem::exists(dbPath)) {
            std::cout << "Файл бази даних існує: " << dbPath << std::endl;
            std::cout << "Розмір файлу: " << std::filesystem::file_size(dbPath) << " байт" << std::endl;
        } else {
            std::cout << "Створюємо нову базу даних: " << dbPath << std::endl;
        }

        // Створюємо об'єкт БД. 
        // SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE створює файл, якщо він не існує.
        db = std::make_unique<SQLite::Database>(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        std::cout << "Успішно підключено до бази даних: " << dbPath << std::endl;
        
        // Ініціалізуємо схему (створюємо таблиці)
        initSchema();
    } catch (const std::exception& e) {
        std::cerr << "Помилка підключення до БД: " << e.what() << std::endl;
        std::cerr << "Шлях до БД: " << dbPath << std::endl;
        std::cerr << "Перевірте права доступу та наявність SQLite." << std::endl;
        throw;
    }
}

// Отримання об'єкта БД
SQLite::Database& DatabaseManager::getDb() {
    if (!db) {
        throw std::runtime_error("База даних не підключена. Викличте connect() спочатку.");
    }
    return *db;
}

// Ініціалізація схеми БД
void DatabaseManager::initSchema() {
    try {
        db->exec("PRAGMA foreign_keys = ON;");

        // Таблиця користувачів
        db->exec(R"(
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT UNIQUE NOT NULL,
                hashed_password INTEGER NOT NULL,
                role INTEGER NOT NULL
            );
        )");

        // Таблиця кімнат
        db->exec(R"(
            CREATE TABLE IF NOT EXISTS rooms (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                room_number INTEGER UNIQUE NOT NULL,
                type INTEGER NOT NULL,
                status INTEGER NOT NULL,
                price_per_night REAL NOT NULL,
                description TEXT NOT NULL DEFAULT '',
                has_air_conditioner INTEGER NOT NULL DEFAULT 0
            );
        )");

        // Таблиця клієнтів
        db->exec(R"(
            CREATE TABLE IF NOT EXISTS clients (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                full_name TEXT NOT NULL,
                contact_info TEXT
            );
        )");

        // Таблиця бронювань
        db->exec(R"(
            CREATE TABLE IF NOT EXISTS bookings (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                client_id INTEGER NOT NULL,
                room_id INTEGER NOT NULL,
                check_in_date TEXT NOT NULL,
                check_out_date TEXT,
                status INTEGER NOT NULL,
                FOREIGN KEY(client_id) REFERENCES clients(id) ON DELETE CASCADE,
                FOREIGN KEY(room_id) REFERENCES rooms(id) ON DELETE RESTRICT
            );
        )");

        std::cout << "Схема бази даних успішно ініціалізована." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Помилка ініціалізації схеми БД: " << e.what() << std::endl;
        throw;
    }
} 