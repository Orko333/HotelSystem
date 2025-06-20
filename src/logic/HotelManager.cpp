#include "logic/HotelManager.h"
#include "models/Enums.h"
#include "models/Room.h"
#include "models/Client.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <optional>
#include <sstream>
#include <chrono>
#include <iomanip>

// Оголошення функцій, визначених в іншому файлі
void clearConsole();
void pauseConsole();
std::string roleToString(UserRole role);

// --- Конвертери Enum в рядок для виводу ---

// Функція для перетворення RoomType в рядок
std::string roomTypeToString(RoomType type) {
    switch (type) {
        case RoomType::SINGLE:  return "Одномісний";
        case RoomType::DOUBLE:  return "Двомісний";
        case RoomType::SUITE:   return "Люкс";
        case RoomType::DELUXE:  return "Делюкс";
        default:                return "Невідомий тип";
    }
}

// Функція для перетворення RoomStatus в рядок
std::string roomStatusToString(RoomStatus status) {
    switch (status) {
        case RoomStatus::AVAILABLE:   return "Вільний";
        case RoomStatus::OCCUPIED:    return "Зайнятий";
        case RoomStatus::MAINTENANCE: return "Обслуговування";
        default:                      return "Невідомий статус";
    }
}

// Функція для перетворення BookingStatus в рядок
std::string bookingStatusToString(BookingStatus status) {
    switch (status) {
        case BookingStatus::CONFIRMED:    return "Підтверджено";
        case BookingStatus::CHECKED_IN:   return "Заселено";
        case BookingStatus::CHECKED_OUT:  return "Виселено";
        case BookingStatus::CANCELLED:    return "Скасовано";
        default:                          return "Невідомий статус";
    }
}

// --- Конструктор ---

HotelManager::HotelManager(
    std::shared_ptr<RoomDAO> rDAO,
    std::shared_ptr<ClientDAO> cDAO,
    std::shared_ptr<BookingDAO> bDAO)
    : roomDAO(std::move(rDAO)), clientDAO(std::move(cDAO)), bookingDAO(std::move(bDAO)) {}

// --- Допоміжні функції для вводу ---

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int HotelManager::getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.good()) {
            clearInputBuffer();
            return value;
        }
        std::cout << "Помилка: введіть ціле число." << std::endl;
        std::cin.clear();
        clearInputBuffer();
    }
}

double HotelManager::getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.good()) {
            clearInputBuffer();
            return value;
        }
        std::cout << "Помилка: введіть число." << std::endl;
        std::cin.clear();
        clearInputBuffer();
    }
}

std::string HotelManager::getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

// Допоміжна функція для валідації формату дати YYYY-MM-DD
bool isValidDate(const std::string& s) {
    std::tm tm = {};
    std::stringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    return !ss.fail() && ss.eof();
}

std::string HotelManager::getDateInput(const std::string& prompt) {
    std::string dateStr;
    while (true) {
        dateStr = getStringInput(prompt);
        if (isValidDate(dateStr)) {
            return dateStr;
        }
        std::cout << "Невірний формат дати. Будь ласка, використовуйте YYYY-MM-DD.\n";
    }
}

// --- Методи для роботи з кімнатами ---

void HotelManager::listAllRooms() {
    clearConsole();
    auto rooms = roomDAO->findAll();
    std::cout << "\n--- Список всіх кімнат ---\n";
    if (rooms.empty()) {
        std::cout << "Кімнати не знайдені.\n";
    } else {
        for (const auto& room : rooms) {
            std::cout << "ID: " << room.id
                      << ", Номер: " << room.roomNumber
                      << ", Тип: " << roomTypeToString(room.type)
                      << ", Статус: " << roomStatusToString(room.status)
                      << ", Ціна: " << room.pricePerNight << " UAH"
                      << ", Опис: " << room.description
                      << ", Кондиціонер: " << (room.hasAirConditioner ? "Так" : "Ні") << "\n";
        }
    }
    pauseConsole();
}

void HotelManager::listAvailableRooms() {
    clearConsole();
    auto rooms = roomDAO->findByStatus(RoomStatus::AVAILABLE);
    std::cout << "\n--- Список вільних кімнат ---\n";
    if (rooms.empty()) {
        std::cout << "Вільні кімнати не знайдені.\n";
    } else {
        for (const auto& room : rooms) {
            std::cout << "ID: " << room.id
                << ", Номер: " << room.roomNumber
                << ", Тип: " << roomTypeToString(room.type)
                << ", Ціна: " << room.pricePerNight << " UAH"
                << ", Опис: " << room.description
                << ", Кондиціонер: " << (room.hasAirConditioner ? "Так" : "Ні") << "\n";
        }
    }
    pauseConsole();
}

void HotelManager::addRoom() {
    clearConsole();
    std::cout << "\n--- Додавання нової кімнати ---\n";
    int roomNumber = getIntInput("Введіть номер кімнати: ");
    if(roomDAO->findByNumber(roomNumber)){
        std::cout << "Кімната з таким номером вже існує.\n";
        pauseConsole();
        return;
    }
    int typeInt = getIntInput("Введіть тип (0: Single, 1: Double, 2: Suite, 3: Deluxe): ");
    double price = getDoubleInput("Введіть ціну за ніч: ");
    std::string description = getStringInput("Введіть опис кімнати: ");
    int acInput = getIntInput("Чи є кондиціонер? (1 - так, 0 - ні): ");
    bool hasAirConditioner = (acInput == 1);

    Room newRoom(roomNumber, static_cast<RoomType>(typeInt), RoomStatus::AVAILABLE, price, description, hasAirConditioner);
    roomDAO->save(newRoom);
    std::cout << "Кімнату успішно додано!\n";
    pauseConsole();
}

void HotelManager::updateRoomStatus() {
    clearConsole();
    std::cout << "\n--- Оновлення статусу кімнати ---\n";
    int roomId = getIntInput("Введіть ID кімнати для оновлення: ");
    auto roomOpt = roomDAO->findById(roomId);
    if (!roomOpt) {
        std::cout << "Кімнату з ID " << roomId << " не знайдено.\n";
        pauseConsole();
        return;
    }
    
    int statusInt = getIntInput("Введіть новий статус (0: Available, 1: Occupied, 2: Maintenance): ");
    Room roomToUpdate = *roomOpt;
    roomToUpdate.status = static_cast<RoomStatus>(statusInt);
    roomDAO->update(roomToUpdate);
    std::cout << "Статус кімнати успішно оновлено!\n";
    pauseConsole();
}

void HotelManager::filterRooms() {
    clearConsole();
    std::cout << "\n--- Фільтрація кімнат ---\n";
    std::cout << "Залиште поле пустим, щоб не використовувати фільтр.\n";

    std::string typeInputStr = getStringInput("Введіть тип (0: Single, 1: Double, 2: Suite, 3: Deluxe): ");
    std::string priceInputStr = getStringInput("Введіть максимальну ціну: ");

    std::optional<RoomType> typeFilter;
    if (!typeInputStr.empty()) {
        try {
            int typeInt = std::stoi(typeInputStr);
            if (typeInt >= 0 && typeInt <= 3) {
                 typeFilter = static_cast<RoomType>(typeInt);
            } else {
                std::cout << "Невірний тип кімнати.\n";
            }
        } catch (const std::invalid_argument& e) {
            std::cout << "Помилка: введіть число для типу.\n";
        }
    }

    std::optional<double> priceFilter;
    if (!priceInputStr.empty()) {
        try {
            priceFilter = std::stod(priceInputStr);
        } catch (const std::invalid_argument& e) {
            std::cout << "Помилка: введіть число для ціни.\n";
        }
    }

    auto rooms = roomDAO->findWithFilters(typeFilter, priceFilter);

    std::cout << "\n--- Результати пошуку ---\n";
    if (rooms.empty()) {
        std::cout << "Кімнати за вашими критеріями не знайдені.\n";
    } else {
        for (const auto& room : rooms) {
            std::cout << "ID: " << room.id
                      << ", Номер: " << room.roomNumber
                      << ", Тип: " << roomTypeToString(room.type)
                      << ", Статус: " << roomStatusToString(room.status)
                      << ", Ціна: " << room.pricePerNight << " UAH"
                      << ", Опис: " << room.description
                      << ", Кондиціонер: " << (room.hasAirConditioner ? "Так" : "Ні") << "\n";
        }
    }
    pauseConsole();
}

// --- Методи для роботи з клієнтами ---

void HotelManager::listAllClients() {
    clearConsole();
    auto clients = clientDAO->findAll();
    std::cout << "\n--- Список всіх клієнтів ---\n";
    if (clients.empty()) {
        std::cout << "Клієнти не знайдені.\n";
    } else {
        for (const auto& client : clients) {
            std::cout << "ID: " << client.id
                      << ", ПІБ: " << client.fullName
                      << ", Контакти: " << client.contactInfo << "\n";
        }
    }
    pauseConsole();
}

void HotelManager::findClientsByName() {
    clearConsole();
    std::cout << "\n--- Пошук клієнтів за іменем ---\n";
    std::string name = getStringInput("Введіть ім'я або частину імені для пошуку: ");
    
    auto clients = clientDAO->findByFullName(name);

    std::cout << "\n--- Результати пошуку ---\n";
    if (clients.empty()) {
        std::cout << "Клієнти з таким іменем не знайдені.\n";
    } else {
        for (const auto& client : clients) {
            std::cout << "ID: " << client.id
                      << ", ПІБ: " << client.fullName
                      << ", Контакти: " << client.contactInfo << "\n";
        }
    }
    pauseConsole();
}

void HotelManager::addNewClient() {
    clearConsole();
    std::cout << "\n--- Додавання нового клієнта ---\n";
    std::string name = getStringInput("Введіть ПІБ клієнта: ");
    std::string contacts = getStringInput("Введіть контактну інформацію: ");
    
    Client newClient(name, contacts);
    clientDAO->save(newClient);
    std::cout << "Клієнта успішно додано!\n";
    pauseConsole();
}

// --- Методи для програмного додавання даних (для заповнення) ---

void HotelManager::addRoom(int number, RoomType type, double price, const std::string& description, bool hasAirConditioner) {
    if(roomDAO->findByNumber(number)){
        return; // Кімната вже існує
    }
    Room newRoom(number, type, RoomStatus::AVAILABLE, price, description, hasAirConditioner);
    roomDAO->save(newRoom);
}

void HotelManager::addNewClient(const std::string& name, const std::string& contactInfo) {
    // Тут можна додати перевірку на існування клієнта, якщо потрібно
    Client newClient(name, contactInfo);
    clientDAO->save(newClient);
}

// --- Методи для роботи з бронюваннями ---

// Допоміжна функція для розрахунку днів між датами
long long daysBetween(const std::string& start, const std::string& end) {
    std::tm start_tm = {};
    std::tm end_tm = {};
    std::stringstream ss_start(start);
    std::stringstream ss_end(end);

    ss_start >> std::get_time(&start_tm, "%Y-%m-%d");
    ss_end >> std::get_time(&end_tm, "%Y-%m-%d");

    if (ss_start.fail() || ss_end.fail()) {
        return 0; // Помилка парсингу
    }

    auto start_time_t = std::mktime(&start_tm);
    auto end_time_t = std::mktime(&end_tm);

    if (start_time_t == -1 || end_time_t == -1) {
        return 0; // Некоректна дата
    }
    
    auto duration = std::chrono::duration_cast<std::chrono::hours>(
        std::chrono::system_clock::from_time_t(end_time_t) - 
        std::chrono::system_clock::from_time_t(start_time_t)
    );

    return duration.count() / 24;
}

void HotelManager::createNewBooking() {
    clearConsole();
    std::cout << "\n--- Створення нового бронювання ---\n";
    listAvailableRooms();
    int roomId = getIntInput("Введіть ID кімнати для бронювання: ");
    auto roomOpt = roomDAO->findById(roomId);
    if (!roomOpt || roomOpt->status != RoomStatus::AVAILABLE) {
        std::cout << "Помилка: Кімната не доступна для бронювання.\n";
        pauseConsole();
        return;
    }

    listAllClients();
    int clientId = getIntInput("Введіть ID клієнта: ");
    if (!clientDAO->findById(clientId)) {
        std::cout << "Помилка: Клієнта з таким ID не знайдено.\n";
        pauseConsole();
        return;
    }
    
    std::string checkIn = getDateInput("Введіть дату заїзду (YYYY-MM-DD): ");
    std::string checkOut = getDateInput("Введіть дату виїзду (YYYY-MM-DD): ");

    // Перевірка, чи кімната вже заброньована на ці дати
    if (bookingDAO->isRoomBookedInDateRange(roomId, checkIn, checkOut)) {
        std::cout << "Помилка: Ця кімната вже заброньована на вибрані дати.\n";
        pauseConsole();
        return;
    }

    Booking newBooking(clientId, roomId, checkIn, checkOut, BookingStatus::CONFIRMED);
    bookingDAO->save(newBooking);

    // Оновлюємо статус кімнати на "Зайнятий"
    Room roomToUpdate = *roomOpt;
    roomToUpdate.status = RoomStatus::OCCUPIED;
    roomDAO->update(roomToUpdate);

    std::cout << "Бронювання успішно створено!\n";
    pauseConsole();
}

void HotelManager::listAllBookings() {
    clearConsole();
    auto bookings = bookingDAO->findAll();
    std::cout << "\n--- Список всіх бронювань ---\n";
    if (bookings.empty()) {
        std::cout << "Бронювання не знайдені.\n";
    } else {
        for (const auto& booking : bookings) {
            auto clientName = clientDAO->findById(booking.clientId)->fullName;
            auto roomNumber = roomDAO->findById(booking.roomId)->roomNumber;

            std::cout << "ID: " << booking.id
                      << ", Клієнт: " << (clientName.empty() ? "N/A" : clientName)
                      << ", Кімната: " << (roomNumber ? roomNumber : 0)
                      << ", Заїзд: " << booking.checkInDate
                      << ", Виїзд: " << booking.checkOutDate
                      << ", Статус: " << bookingStatusToString(booking.status) << "\n";
        }
    }
    pauseConsole();
}

void HotelManager::generateFinancialReport() {
    clearConsole();
    std::cout << "\n--- Генерація фінансового звіту ---\n";
    std::string startDate = getDateInput("Введіть початкову дату (YYYY-MM-DD): ");
    std::string endDate = getDateInput("Введіть кінцеву дату (YYYY-MM-DD): ");

    auto bookings = bookingDAO->findInDateRange(startDate, endDate);
    if (bookings.empty()) {
        std::cout << "За вказаний період бронювання не знайдені.\n";
        pauseConsole();
        return;
    }

    double totalRevenue = 0;
    std::cout << "\n--- Звіт про доходи з " << startDate << " по " << endDate << " ---\n";

    for (const auto& booking : bookings) {
        auto roomOpt = roomDAO->findById(booking.roomId);
        if (roomOpt && (booking.status == BookingStatus::CHECKED_IN || booking.status == BookingStatus::CHECKED_OUT || booking.status == BookingStatus::CONFIRMED)) {
            long long nights = daysBetween(booking.checkInDate, booking.checkOutDate);
            if (nights <= 0) nights = 1; // Мінімально 1 ніч

            double revenue = nights * roomOpt->pricePerNight;
            totalRevenue += revenue;
            std::cout << "Бронювання #" << booking.id
                      << " | Кімната #" << roomOpt->roomNumber
                      << " | " << nights << " ночей * " << roomOpt->pricePerNight << " UAH = " << revenue << " UAH\n";
        }
    }

    std::cout << "--------------------------------------------------\n";
    std::cout << "Загальний дохід за період: " << totalRevenue << " UAH\n";
    pauseConsole();
} 