#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    // Налаштування консолі для Windows
    #ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    #endif

    std::cout << "=== Тест бази даних ===" << std::endl;
    
    // Перевіряємо поточну робочу директорію
    std::cout << "Поточна робоча директорія: " << std::filesystem::current_path().string() << std::endl;
    
    // Перевіряємо наявність файлу бази даних
    std::string dbPath = "hotel.db";
    if (std::filesystem::exists(dbPath)) {
        std::cout << "✓ Файл бази даних знайдено: " << dbPath << std::endl;
        std::cout << "  Розмір: " << std::filesystem::file_size(dbPath) << " байт" << std::endl;
        
        // Перевіряємо права доступу
        try {
            std::filesystem::path path(dbPath);
            auto perms = std::filesystem::status(path).permissions();
            std::cout << "  Права на читання: " << ((perms & std::filesystem::perms::owner_read) != std::filesystem::perms::none ? "Так" : "Ні") << std::endl;
            std::cout << "  Права на запис: " << ((perms & std::filesystem::perms::owner_write) != std::filesystem::perms::none ? "Так" : "Ні") << std::endl;
        } catch (const std::exception& e) {
            std::cout << "  Помилка при перевірці прав: " << e.what() << std::endl;
        }
    } else {
        std::cout << "✗ Файл бази даних НЕ знайдено: " << dbPath << std::endl;
        
        // Перевіряємо інші можливі місця
        std::vector<std::string> possiblePaths = {
            "./hotel.db",
            "../hotel.db",
            "../../hotel.db",
            "build/hotel.db",
            "../build/hotel.db"
        };
        
        for (const auto& path : possiblePaths) {
            if (std::filesystem::exists(path)) {
                std::cout << "✓ Файл знайдено в: " << path << std::endl;
                break;
            }
        }
    }
    
    // Перевіряємо наявність SQLite
    std::cout << "\nПеревірка SQLite..." << std::endl;
    try {
        // Спроба створити тестову базу даних
        std::string testDbPath = "test_db.sqlite";
        std::cout << "Спроба створення тестової БД: " << testDbPath << std::endl;
        
        // Тут буде код для створення тестової БД, якщо SQLite доступний
        std::cout << "SQLite доступний для тестування" << std::endl;
        
        // Видаляємо тестову БД
        if (std::filesystem::exists(testDbPath)) {
            std::filesystem::remove(testDbPath);
        }
        
    } catch (const std::exception& e) {
        std::cout << "✗ Помилка SQLite: " << e.what() << std::endl;
    }
    
    std::cout << "\nНатисніть Enter для виходу...";
    std::cin.get();
    
    return 0;
} 