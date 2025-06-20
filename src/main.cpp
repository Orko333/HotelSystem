#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cstdlib>
#include <limits>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

#include "db/DatabaseManager.h"
#include "dao/UserDAO.h"
#include "dao/RoomDAO.h"
#include "dao/ClientDAO.h"
#include "dao/BookingDAO.h"
#include "logic/AuthenticationManager.h"
#include "logic/HotelManager.h"
#include "ui/Menu.h"
#include "utils.h"

// Forward declarations
void setupConsole();
void ensureAdminExists(AuthenticationManager& authManager);
void seedData(HotelManager& hotelManager, AuthenticationManager& authManager);
void manageUsers(std::shared_ptr<AuthenticationManager> authManager);
std::string findDatabaseFile();

void setupConsole() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
}

std::string findDatabaseFile() {
    std::vector<std::string> possiblePaths = {
        "hotel.db",                    // Поточна папка
        "./hotel.db",                  // Поточна папка (явно)
        "../hotel.db",                 // Батьківська папка
        "../../hotel.db",              // Дві папки вгору
        "build/hotel.db",              // Папка build
        "../build/hotel.db",           // Build в батьківській папці
        "data/hotel.db",               // Папка data
        "../data/hotel.db"             // Data в батьківській папці
    };

    for (const auto& path : possiblePaths) {
        if (std::filesystem::exists(path)) {
            std::cout << "Знайдено базу даних: " << path << std::endl;
            return path;
        }
    }

    // Якщо файл не знайдено, повертаємо шлях для створення в поточній папці
    std::cout << "База даних не знайдена. Буде створена в поточній папці: hotel.db" << std::endl;
    return "hotel.db";
}

void ensureAdminExists(AuthenticationManager& authManager) {
    if (!authManager.userExists("admin")) {
        if (authManager.createUser("admin", "admin", UserRole::ADMINISTRATOR)) {
            std::cout << "Створено обліковий запис адміністратора за замовчуванням (admin/admin)." << std::endl;
        }
    }
}

void seedData(HotelManager& hotelManager, AuthenticationManager& authManager) {
    if (!authManager.userExists("reception")) {
        std::cout << "Виконується початкове заповнення даних..." << std::endl;
        
        hotelManager.addRoom(101, RoomType::SINGLE, 1200.0, "Стандартний одномісний номер", true);
        hotelManager.addRoom(102, RoomType::DOUBLE, 1800.0, "Просторий двомісний номер з видом на місто", true);
        hotelManager.addRoom(201, RoomType::SUITE, 2500.0, "Розкішний люкс з окремою вітальнею", true);
        hotelManager.addRoom(202, RoomType::DELUXE, 3200.0, "Делюкс з панорамними вікнами і балконом", false);

        hotelManager.addNewClient("Іван Петров", "+380501112233");
        hotelManager.addNewClient("Марія Іваненко", "+380674445566");

        authManager.createUser("reception", "reception", UserRole::RECEPTIONIST);
        authManager.createUser("guest", "guest", UserRole::GUEST);

        std::cout << "Початкове заповнення даних завершено." << std::endl;
    }
}

void manageUsers(std::shared_ptr<AuthenticationManager> authManager) {
    clearConsole();
    std::cout << "--- Керування користувачами ---" << std::endl;
    std::cout << "1. Створити нового користувача" << std::endl;
    std::cout << "2. Переглянути всіх користувачів" << std::endl;
    std::cout << "Будь-яка інша клавіша для повернення." << std::endl;
    std::cout << "\nВаш вибір: ";

    std::string choice_str;
    std::getline(std::cin, choice_str);

    if (choice_str == "1") {
        std::string username, password, role_str;
        UserRole role;

        std::cout << "Введіть новий логін: ";
        std::getline(std::cin, username);
        
        std::cout << "Введіть новий пароль: ";
        std::getline(std::cin, password);
        
        std::cout << "Введіть роль (Guest, Receptionist, Manager, Administrator): ";
        std::getline(std::cin, role_str);

        if (role_str == "Receptionist") role = UserRole::RECEPTIONIST;
        else if (role_str == "Manager") role = UserRole::MANAGER;
        else if (role_str == "Administrator") role = UserRole::ADMINISTRATOR;
        else role = UserRole::GUEST;

        if (authManager->createUser(username, password, role)) {
            std::cout << "Користувача успішно створено!" << std::endl;
        } else {
            std::cout << "Не вдалося створити користувача (можливо, такий логін вже існує)." << std::endl;
        }

    } else if (choice_str == "2") {
        authManager->listAllUsers();
    }
    
    pauseConsole();
}

int main() {
    setupConsole();

    std::string dbPath;
    try {
        dbPath = findDatabaseFile();
    } catch (const std::exception& e) {
        std::cerr << "Помилка при пошуку бази даних: " << e.what() << std::endl;
        pauseConsole();
        return 1;
    }

    if (dbPath.empty()) {
        std::cerr << "Помилка: не вдалося знайти або створити базу даних. Програма буде закрита." << std::endl;
        pauseConsole();
        return 1;
    }

    std::cout << "Використовується база даних: " << dbPath << std::endl;

    std::shared_ptr<UserDAO> userDAO;
    std::shared_ptr<RoomDAO> roomDAO;
    std::shared_ptr<ClientDAO> clientDAO;
    std::shared_ptr<BookingDAO> bookingDAO;

    try {
        DatabaseManager::getInstance().connect(dbPath);
        auto& db = DatabaseManager::getInstance().getDb();
        
        userDAO = std::make_shared<UserDAO>(db);
        roomDAO = std::make_shared<RoomDAO>(db);
        clientDAO = std::make_shared<ClientDAO>(db);
        bookingDAO = std::make_shared<BookingDAO>(db);

        std::cout << "База даних успішно ініціалізована." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Критична помилка при ініціалізації БД: " << e.what() << std::endl;
        std::cerr << "Перевірте права доступу до файлу та наявність SQLite." << std::endl;
        pauseConsole();
        return 1;
    }

    auto authManager = std::make_shared<AuthenticationManager>(userDAO);
    auto hotelManager = std::make_shared<HotelManager>(roomDAO, clientDAO, bookingDAO);

    ensureAdminExists(*authManager);
    seedData(*hotelManager, *authManager);

    bool appRunning = true;
    while(appRunning) {
        while (!authManager->getCurrentUser().has_value()) {
            clearConsole();
            std::cout << "\n--- Вхід в систему ---" << std::endl;
            std::cout << "(введіть 'exit' в поле для логіну, щоб вийти з програми)" << std::endl;
            std::string username, password;
            std::cout << "Логін: ";
            std::getline(std::cin, username);

            if (username == "exit") {
                appRunning = false;
                break;
            }

            std::cout << "Пароль: ";
            std::getline(std::cin, password);

            if (!authManager->login(username, password)) {
                std::cout << "Невірний логін або пароль. Спробуйте ще раз." << std::endl;
                pauseConsole();
            }
        }

        if (!appRunning) {
            break; 
        }

        if (authManager->getCurrentUser().has_value()) {
            User currentUser = authManager->getCurrentUser().value();
            Menu mainMenu;
            
            mainMenu.addItem("Переглянути список всіх кімнат", [&]() { hotelManager->listAllRooms(); }, UserRole::GUEST);
            mainMenu.addItem("Переглянути список вільних кімнат", [&]() { hotelManager->listAvailableRooms(); }, UserRole::GUEST);
            mainMenu.addItem("Пошук та фільтрація кімнат", [&]() { hotelManager->filterRooms(); }, UserRole::GUEST);
            mainMenu.addItem("Переглянути всі бронювання", [&]() { hotelManager->listAllBookings(); }, UserRole::GUEST);
            mainMenu.addItem("Переглянути всіх клієнтів", [&]() { hotelManager->listAllClients(); }, UserRole::GUEST);
            mainMenu.addItem("Пошук клієнта за іменем", [&]() { hotelManager->findClientsByName(); }, UserRole::GUEST);
            mainMenu.addItem("Створити нове бронювання", [&]() { hotelManager->createNewBooking(); }, UserRole::RECEPTIONIST);
            mainMenu.addItem("Додати нового клієнта", [&]() { hotelManager->addNewClient(); }, UserRole::RECEPTIONIST);
            mainMenu.addItem("Оновити статус кімнати", [&]() { hotelManager->updateRoomStatus(); }, UserRole::MANAGER);
            mainMenu.addItem("Генерувати фінансовий звіт", [&]() { hotelManager->generateFinancialReport(); }, UserRole::MANAGER);
            mainMenu.addItem("Додати нову кімнату", [&]() { hotelManager->addRoom(); }, UserRole::ADMINISTRATOR);
            mainMenu.addItem("Керування користувачами", [&]() { manageUsers(authManager); }, UserRole::ADMINISTRATOR);

            mainMenu.run(currentUser.role);
            authManager->logout();
        }
    }
    
    std::cout << "Роботу програми завершено." << std::endl;
    pauseConsole();
    return 0;
} 