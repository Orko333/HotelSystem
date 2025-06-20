#include "dao/BookingDAO.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

BookingDAO::BookingDAO(SQLite::Database& db) : db(db) {}

void BookingDAO::save(const Booking& booking) {
    try {
        SQLite::Statement query(db, "INSERT INTO bookings (client_id, room_id, check_in_date, check_out_date, status) VALUES (?, ?, ?, ?, ?)");
        query.bind(1, booking.clientId);
        query.bind(2, booking.roomId);
        query.bind(3, booking.checkInDate);
        query.bind(4, booking.checkOutDate);
        query.bind(5, static_cast<int>(booking.status));
        query.exec();
    } catch (const std::exception& e) {
        std::cerr << "Помилка збереження бронювання: " << e.what() << std::endl;
        throw;
    }
}

void BookingDAO::update(const Booking& booking) {
    try {
        SQLite::Statement query(db, "UPDATE bookings SET client_id = ?, room_id = ?, check_in_date = ?, check_out_date = ?, status = ? WHERE id = ?");
        query.bind(1, booking.clientId);
        query.bind(2, booking.roomId);
        query.bind(3, booking.checkInDate);
        query.bind(4, booking.checkOutDate);
        query.bind(5, static_cast<int>(booking.status));
        query.bind(6, booking.id);
        query.exec();
    } catch (const std::exception& e) {
        std::cerr << "Помилка оновлення бронювання: " << e.what() << std::endl;
        throw;
    }
}

std::optional<Booking> BookingDAO::findById(int id) {
    try {
        SQLite::Statement query(db, "SELECT * FROM bookings WHERE id = ?");
        query.bind(1, id);
        if (query.executeStep()) {
            return std::make_optional<Booking>(
                query.getColumn("id").getInt(),
                query.getColumn("client_id").getInt(),
                query.getColumn("room_id").getInt(),
                query.getColumn("check_in_date").getString(),
                query.getColumn("check_out_date").getString(),
                static_cast<BookingStatus>(query.getColumn("status").getInt())
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку бронювання за ID: " << e.what() << std::endl;
        throw;
    }
    return std::nullopt;
}

std::vector<Booking> BookingDAO::findAll() {
    std::vector<Booking> bookings;
    try {
        SQLite::Statement query(db, "SELECT * FROM bookings ORDER BY check_in_date DESC");
        while (query.executeStep()) {
            bookings.emplace_back(
                query.getColumn("id").getInt(),
                query.getColumn("client_id").getInt(),
                query.getColumn("room_id").getInt(),
                query.getColumn("check_in_date").getString(),
                query.getColumn("check_out_date").getString(),
                static_cast<BookingStatus>(query.getColumn("status").getInt())
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка отримання всіх бронювань: " << e.what() << std::endl;
        throw;
    }
    return bookings;
}

std::vector<Booking> BookingDAO::findByClientId(int clientId) {
    std::vector<Booking> bookings;
    try {
        SQLite::Statement query(db, "SELECT * FROM bookings WHERE client_id = ? ORDER BY check_in_date DESC");
        query.bind(1, clientId);
        while (query.executeStep()) {
            bookings.emplace_back(
                query.getColumn("id").getInt(),
                query.getColumn("client_id").getInt(),
                query.getColumn("room_id").getInt(),
                query.getColumn("check_in_date").getString(),
                query.getColumn("check_out_date").getString(),
                static_cast<BookingStatus>(query.getColumn("status").getInt())
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку бронювань за ID клієнта: " << e.what() << std::endl;
        throw;
    }
    return bookings;
}

std::vector<Booking> BookingDAO::findByRoomId(int roomId) {
    std::vector<Booking> bookings;
    try {
        SQLite::Statement query(db, "SELECT * FROM bookings WHERE room_id = ? ORDER BY check_in_date DESC");
        query.bind(1, roomId);
        while (query.executeStep()) {
            bookings.emplace_back(
                query.getColumn("id").getInt(),
                query.getColumn("client_id").getInt(),
                query.getColumn("room_id").getInt(),
                query.getColumn("check_in_date").getString(),
                query.getColumn("check_out_date").getString(),
                static_cast<BookingStatus>(query.getColumn("status").getInt())
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку бронювань за ID кімнати: " << e.what() << std::endl;
        throw;
    }
    return bookings;
}

std::vector<Booking> BookingDAO::findInDateRange(const std::string& startDate, const std::string& endDate) {
    std::vector<Booking> bookings;
    try {
        SQLite::Statement query(db, "SELECT * FROM bookings WHERE check_in_date BETWEEN ? AND ? ORDER BY check_in_date DESC");
        query.bind(1, startDate);
        query.bind(2, endDate);
        while (query.executeStep()) {
            bookings.emplace_back(
                query.getColumn("id").getInt(),
                query.getColumn("client_id").getInt(),
                query.getColumn("room_id").getInt(),
                query.getColumn("check_in_date").getString(),
                query.getColumn("check_out_date").getString(),
                static_cast<BookingStatus>(query.getColumn("status").getInt())
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку бронювань за датою: " << e.what() << std::endl;
        throw;
    }
    return bookings;
}

bool BookingDAO::isRoomBookedInDateRange(int roomId, const std::string& checkInDate, const std::string& checkOutDate) {
    try {
        // Запит для перевірки перетину дат.
        // Перетин існує, якщо (StartDate1 < EndDate2) і (EndDate1 > StartDate2).
        // Ми також виключаємо бронювання зі статусом "Скасовано".
        SQLite::Statement query(db, R"(
            SELECT COUNT(*) FROM bookings 
            WHERE room_id = ? 
            AND status != ? 
            AND check_in_date < ? 
            AND check_out_date > ?
        )");
        query.bind(1, roomId);
        query.bind(2, static_cast<int>(BookingStatus::CANCELLED));
        query.bind(3, checkOutDate); // Порівнюємо з новою датою виїзду
        query.bind(4, checkInDate);  // Порівнюємо з новою датою заїзду
        
        if (query.executeStep()) {
            int count = query.getColumn(0).getInt();
            return count > 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка перевірки доступності кімнати: " << e.what() << std::endl;
        throw;
    }
    return false; // Якщо сталася помилка, вважаємо, що перевірка не вдалася
} 