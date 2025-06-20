#include "dao/ClientDAO.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>

ClientDAO::ClientDAO(SQLite::Database& db) : db(db) {}

void ClientDAO::save(const Client& client) {
    try {
        SQLite::Statement query(db, "INSERT INTO clients (full_name, contact_info) VALUES (?, ?)");
        query.bind(1, client.fullName);
        query.bind(2, client.contactInfo);
        query.exec();
    } catch (const std::exception& e) {
        std::cerr << "Помилка збереження клієнта: " << e.what() << std::endl;
        throw;
    }
}

void ClientDAO::update(const Client& client) {
    try {
        SQLite::Statement query(db, "UPDATE clients SET full_name = ?, contact_info = ? WHERE id = ?");
        query.bind(1, client.fullName);
        query.bind(2, client.contactInfo);
        query.bind(3, client.id);
        query.exec();
    } catch (const std::exception& e) {
        std::cerr << "Помилка оновлення клієнта: " << e.what() << std::endl;
        throw;
    }
}

std::optional<Client> ClientDAO::findById(int id) {
    try {
        SQLite::Statement query(db, "SELECT * FROM clients WHERE id = ?");
        query.bind(1, id);
        if (query.executeStep()) {
            return std::make_optional<Client>(
                query.getColumn("id").getInt(),
                query.getColumn("full_name").getString(),
                query.getColumn("contact_info").getString()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку клієнта за ID: " << e.what() << std::endl;
        throw;
    }
    return std::nullopt;
}

std::vector<Client> ClientDAO::findByFullName(const std::string& name) {
    std::vector<Client> clients;
    try {
        // Використовуємо LIKE для пошуку за частиною імені
        SQLite::Statement query(db, "SELECT * FROM clients WHERE full_name LIKE ?");
        query.bind(1, "%" + name + "%");
        while (query.executeStep()) {
            clients.emplace_back(
                query.getColumn("id").getInt(),
                query.getColumn("full_name").getString(),
                query.getColumn("contact_info").getString()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка пошуку клієнта за іменем: " << e.what() << std::endl;
        throw;
    }
    return clients;
}

std::vector<Client> ClientDAO::findAll() {
    std::vector<Client> clients;
    try {
        SQLite::Statement query(db, "SELECT * FROM clients ORDER BY full_name ASC");
        while (query.executeStep()) {
            clients.emplace_back(
                query.getColumn("id").getInt(),
                query.getColumn("full_name").getString(),
                query.getColumn("contact_info").getString()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Помилка отримання всіх клієнтів: " << e.what() << std::endl;
        throw;
    }
    return clients;
} 