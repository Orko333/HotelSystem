#pragma once
#include <string>
#include <utility>
#include "models/Enums.h"

// Клас, що представляє готельний номер
class Room {
public:
    int id;
    int roomNumber;
    RoomType type;
    RoomStatus status;
    double pricePerNight;
    std::string description;
    bool hasAirConditioner;

    Room(int id, int roomNumber, RoomType type, RoomStatus status, double pricePerNight, std::string description, bool hasAirConditioner)
        : id(id), roomNumber(roomNumber), type(type), status(status), pricePerNight(pricePerNight), 
          description(std::move(description)), hasAirConditioner(hasAirConditioner) {}

    // Конструктор для нових номерів
    Room(int roomNumber, RoomType type, RoomStatus status, double pricePerNight, std::string description, bool hasAirConditioner)
        : id(-1), roomNumber(roomNumber), type(type), status(status), pricePerNight(pricePerNight),
          description(std::move(description)), hasAirConditioner(hasAirConditioner) {}
}; 