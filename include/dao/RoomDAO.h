#pragma once

#include "models/Room.h"
#include <vector>
#include <optional>
#include <string>

namespace SQLite {
    class Database;
}

// DAO для операцій з кімнатами
class RoomDAO {
public:
    explicit RoomDAO(SQLite::Database& db);

    void save(const Room& room);
    void update(const Room& room);
    std::optional<Room> findById(int id);
    std::optional<Room> findByNumber(int roomNumber);
    std::vector<Room> findAll();
    std::vector<Room> findByStatus(RoomStatus status);
    std::vector<Room> findWithFilters(std::optional<RoomType> type, std::optional<double> maxPrice);
    
    // Знаходить вільні кімнати у заданому діапазоні дат (поки що не реалізовано)
    // std::vector<Room> findAvailable(const std::string& startDate, const std::string& endDate);

private:
    SQLite::Database& db;
}; 