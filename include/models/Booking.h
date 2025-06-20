#pragma once
#include <string>
#include <utility>
#include "models/Enums.h"

// Клас, що представляє бронювання
class Booking {
public:
    int id;
    int clientId;
    int roomId;
    std::string checkInDate;
    std::string checkOutDate;
    BookingStatus status;

    Booking(int id, int clientId, int roomId, std::string checkInDate, std::string checkOutDate, BookingStatus status)
        : id(id), clientId(clientId), roomId(roomId), checkInDate(std::move(checkInDate)), checkOutDate(std::move(checkOutDate)), status(status) {}

    // Конструктор для нових бронювань
    Booking(int clientId, int roomId, std::string checkInDate, std::string checkOutDate, BookingStatus status)
        : id(-1), clientId(clientId), roomId(roomId), checkInDate(std::move(checkInDate)), checkOutDate(std::move(checkOutDate)), status(status) {}
}; 