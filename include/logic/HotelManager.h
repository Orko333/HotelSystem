#pragma once

#include "dao/RoomDAO.h"
#include "dao/ClientDAO.h"
#include "dao/BookingDAO.h"
#include <memory>

// Головний клас бізнес-логіки для управління готелем
class HotelManager {
public:
    // Конструктор приймає всі необхідні DAO
    HotelManager(
        std::shared_ptr<RoomDAO> roomDAO,
        std::shared_ptr<ClientDAO> clientDAO,
        std::shared_ptr<BookingDAO> bookingDAO
    );

    // --- Методи для роботи з кімнатами ---
    void listAllRooms();
    void listAvailableRooms();
    void addRoom();
    void updateRoomStatus();
    void filterRooms();

    // --- Методи для роботи з клієнтами ---
    void listAllClients();
    void addNewClient();
    void findClientsByName();

    // --- Методи для роботи з бронюваннями ---
    void createNewBooking();
    void listAllBookings();
    void generateFinancialReport();

    // --- Методи для програмного додавання даних (для заповнення) ---
    void addRoom(int number, RoomType type, double price, const std::string& description, bool hasAirConditioner);
    void addNewClient(const std::string& name, const std::string& contactInfo);

private:
    std::shared_ptr<RoomDAO> roomDAO;
    std::shared_ptr<ClientDAO> clientDAO;
    std::shared_ptr<BookingDAO> bookingDAO;
    
    // Допоміжні функції для вводу даних
    int getIntInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    std::string getStringInput(const std::string& prompt);
    std::string getDateInput(const std::string& prompt);
}; 