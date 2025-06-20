#pragma once

// Ролі користувачів в системі
enum class UserRole {
    ADMINISTRATOR,
    MANAGER,
    RECEPTIONIST,
    HOUSEKEEPING,
    GUEST
};

// Типи готельних номерів
enum class RoomType {
    SINGLE,
    DOUBLE,
    SUITE,
    DELUXE
};

// Статус готельного номера
enum class RoomStatus {
    AVAILABLE,
    OCCUPIED,
    MAINTENANCE // Потребує прибирання/обслуговування
};

// Статус бронювання
enum class BookingStatus {
    CONFIRMED,
    CHECKED_IN,
    CHECKED_OUT,
    CANCELLED
}; 