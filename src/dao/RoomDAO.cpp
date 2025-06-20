#include "dao/RoomDAO.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <string>
#include <vector>
#include <variant>

RoomDAO::RoomDAO(SQLite::Database& db) : db(db) {}

void RoomDAO::save(const Room& room) {
    try {
        SQLite::Statement query(db, "INSERT INTO rooms (room_number, type, status, price_per_night, description, has_air_conditioner) VALUES (?, ?, ?, ?, ?, ?)");
        query.bind(1, room.roomNumber);
        query.bind(2, static_cast<int>(room.type));
        query.bind(3, static_cast<int>(room.status));
        query.bind(4, room.pricePerNight);
        query.bind(5, room.description);
        query.bind(6, room.hasAirConditioner);
        query.exec();
    } catch (const std::exception& e) {
        std::cerr << "Помилка збереження кімнати: " << e.what() << std::endl;
        throw;
    }
}

void RoomDAO::update(const Room& room) {
    try {
        SQLite::Statement query(db, "UPDATE rooms SET room_number = ?, type = ?, status = ?, price_per_night = ?, description = ?, has_air_conditioner = ? WHERE id = ?");
        query.bind(1, room.roomNumber);
        query.bind(2, static_cast<int>(room.type));
        query.bind(3, static_cast<int>(room.status));
        query.bind(4, room.pricePerNight);
        query.bind(5, room.description);
        query.bind(6, room.hasAirConditioner);
        query.bind(7, room.id);
        query.exec();
    } catch (const std::exception& e) {
        std::cerr << "Помилка оновлення кімнати: " << e.what() << std::endl;
        throw;
    }
}

std::optional<Room> RoomDAO::findById(int id) {
    try {
        SQLite::Statement query(db, "SELECT * FROM rooms WHERE id = ?");
        query.bind(1, id);
        if (query.executeStep()) {
            return std::make_optional<Room>(
                query.getColumn("id").getInt(),
                query.getColumn("room_number").getInt(),
                static_cast<RoomType>(query.getColumn("type").getInt()),
                static_cast<RoomStatus>(query.getColumn("status").getInt()),
                query.getColumn("price_per_night").getDouble(),
                query.getColumn("description").getString(),
                query.getColumn("has_air_conditioner").getInt()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку кімнати за ID: " << e.what() << std::endl;
        throw;
    }
    return std::nullopt;
}

std::optional<Room> RoomDAO::findByNumber(int roomNumber) {
    try {
        SQLite::Statement query(db, "SELECT * FROM rooms WHERE room_number = ?");
        query.bind(1, roomNumber);
        if (query.executeStep()) {
            return std::make_optional<Room>(
                query.getColumn("id").getInt(),
                query.getColumn("room_number").getInt(),
                static_cast<RoomType>(query.getColumn("type").getInt()),
                static_cast<RoomStatus>(query.getColumn("status").getInt()),
                query.getColumn("price_per_night").getDouble(),
                query.getColumn("description").getString(),
                query.getColumn("has_air_conditioner").getInt()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку кімнати за номером: " << e.what() << std::endl;
        throw;
    }
    return std::nullopt;
}

std::vector<Room> RoomDAO::findAll() {
    std::vector<Room> rooms;
    try {
        SQLite::Statement query(db, "SELECT * FROM rooms ORDER BY room_number ASC");
        while (query.executeStep()) {
            rooms.emplace_back(
                query.getColumn("id").getInt(),
                query.getColumn("room_number").getInt(),
                static_cast<RoomType>(query.getColumn("type").getInt()),
                static_cast<RoomStatus>(query.getColumn("status").getInt()),
                query.getColumn("price_per_night").getDouble(),
                query.getColumn("description").getString(),
                query.getColumn("has_air_conditioner").getInt()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка отримання всіх кімнат: " << e.what() << std::endl;
        throw;
    }
    return rooms;
}

std::vector<Room> RoomDAO::findByStatus(RoomStatus status) {
    std::vector<Room> rooms;
    try {
        SQLite::Statement query(db, "SELECT * FROM rooms WHERE status = ? ORDER BY room_number ASC");
        query.bind(1, static_cast<int>(status));
        while (query.executeStep()) {
            rooms.emplace_back(
                query.getColumn("id").getInt(),
                query.getColumn("room_number").getInt(),
                static_cast<RoomType>(query.getColumn("type").getInt()),
                static_cast<RoomStatus>(query.getColumn("status").getInt()),
                query.getColumn("price_per_night").getDouble(),
                query.getColumn("description").getString(),
                query.getColumn("has_air_conditioner").getInt()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку кімнат за статусом: " << e.what() << std::endl;
        throw;
    }
    return rooms;
}

std::vector<Room> RoomDAO::findWithFilters(std::optional<RoomType> type, std::optional<double> maxPrice) {
    std::vector<Room> rooms;
    std::string queryString = "SELECT * FROM rooms WHERE 1=1";
    std::vector<std::variant<int, double, std::string>> params;

    if (type.has_value()) {
        queryString += " AND type = ?";
        params.push_back(static_cast<int>(type.value()));
    }
    if (maxPrice.has_value()) {
        queryString += " AND price_per_night <= ?";
        params.push_back(maxPrice.value());
    }
    queryString += " ORDER BY room_number ASC";

    try {
        SQLite::Statement query(db, queryString);
        for (int i = 0; i < params.size(); ++i) {
            std::visit([&](auto&& arg) {
                query.bind(i + 1, arg);
            }, params[i]);
        }

        while (query.executeStep()) {
            rooms.emplace_back(
                query.getColumn("id").getInt(),
                query.getColumn("room_number").getInt(),
                static_cast<RoomType>(query.getColumn("type").getInt()),
                static_cast<RoomStatus>(query.getColumn("status").getInt()),
                query.getColumn("price_per_night").getDouble(),
                query.getColumn("description").getString(),
                query.getColumn("has_air_conditioner").getInt()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку кімнат за фільтрами: " << e.what() << std::endl;
        throw;
    }
    return rooms;
} 