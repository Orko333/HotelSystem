#pragma once

#include "models/Client.h"
#include <vector>
#include <optional>
#include <string>

namespace SQLite {
    class Database;
}

// DAO для операцій з клієнтами
class ClientDAO {
public:
    explicit ClientDAO(SQLite::Database& db);

    void save(const Client& client);
    void update(const Client& client);
    std::optional<Client> findById(int id);
    std::vector<Client> findByFullName(const std::string& name);
    std::vector<Client> findAll();

private:
    SQLite::Database& db;
}; 