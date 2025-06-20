#pragma once

#include "models/Booking.h"
#include <vector>
#include <optional>
#include <string>

namespace SQLite {
    class Database;
}

// DAO для операцій з бронюваннями
class BookingDAO {
public:
    explicit BookingDAO(SQLite::Database& db);

    void save(const Booking& booking);
    void update(const Booking& booking);
    std::optional<Booking> findById(int id);
    std::vector<Booking> findAll();
    std::vector<Booking> findByClientId(int clientId);
    std::vector<Booking> findByRoomId(int roomId);
    std::vector<Booking> findInDateRange(const std::string& startDate, const std::string& endDate);
    bool isRoomBookedInDateRange(int roomId, const std::string& checkInDate, const std::string& checkOutDate);

private:
    SQLite::Database& db;
}; 