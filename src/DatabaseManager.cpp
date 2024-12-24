#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : m_dbPath(dbPath), m_db(nullptr)
{
}

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::open() {
    if (sqlite3_open(m_dbPath.c_str(), &m_db) != SQLITE_OK) {
        std::cerr << "Errore nell'aprire il database: " 
                  << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }
    return true;
}

void DatabaseManager::close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool DatabaseManager::createTable() {
    const char* createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(m_db, createTableQuery, nullptr, nullptr, &errMsg) 
        != SQLITE_OK) 
    {
        std::cerr << "Errore nella creazione della tabella: " 
                  << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool DatabaseManager::addTask(const Task& task) {
    const char* insertQuery = 
        "INSERT INTO tasks (title, description) VALUES (?, ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, insertQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di addTask: " 
                  << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    // Bind dei parametri
    sqlite3_bind_text(stmt, 1, task.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, task.getDescription().c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Errore nell'esecuzione di addTask: " 
                  << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseManager::updateTask(const Task& task) {
    const char* updateQuery =
        "UPDATE tasks SET title = ?, description = ? WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, updateQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di updateTask: "
                  << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    // Bind dei parametri
    sqlite3_bind_text(stmt, 1, task.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, task.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, task.getId());

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Errore nell'esecuzione di updateTask: "
                  << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseManager::removeTask(int id) {
    const char* deleteQuery = "DELETE FROM tasks WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, deleteQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di removeTask: "
                  << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Errore nell'esecuzione di removeTask: "
                  << sqlite3_errmsg(m_db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

std::vector<Task> DatabaseManager::getAllTasks() {
    std::vector<Task> tasks;
    const char* selectQuery = "SELECT id, title, description FROM tasks;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db, selectQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di getAllTasks: "
                  << sqlite3_errmsg(m_db) << std::endl;
        return tasks;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* title = sqlite3_column_text(stmt, 1);
        const unsigned char* description = sqlite3_column_text(stmt, 2);

        tasks.emplace_back(
            id,
            title ? reinterpret_cast<const char*>(title) : "",
            description ? reinterpret_cast<const char*>(description) : ""
        );
    }

    sqlite3_finalize(stmt);
    return tasks;
}
