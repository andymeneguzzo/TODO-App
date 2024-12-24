#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : m_dbPath(dbPath), m_db(nullptr)
{
}

DatabaseManager::~DatabaseManager() {
}

bool DatabaseManager::open() {
    sqlite3* dbRaw = nullptr;
    if (sqlite3_open(m_dbPath.c_str(), &dbRaw) != SQLITE_OK) {
        std::cerr << "Errore nell'aprire il database: " 
                  << sqlite3_errmsg(dbRaw) << std::endl;
        if (dbRaw) {
            sqlite3_close(dbRaw);
        }
        return false;
    }
    m_db.reset(dbRaw);
    return true;
}

bool DatabaseManager::createTable() {
    const char* createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT,
            due_date INTEGER,
            priority INTEGER,
            completed INTEGER DEFAULT 0
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(m_db.get(), createTableQuery, nullptr, nullptr, &errMsg) 
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
        "INSERT INTO tasks (title, description, due_date, priority, completed) VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db.get(), insertQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di addTask: " 
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Bind dei parametri
    sqlite3_bind_text(stmt, 1, task.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, task.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 3, static_cast<sqlite3_int64>(task.getDueDate()));
    sqlite3_bind_int(stmt, 4, static_cast<int>(task.getPriority()));
    sqlite3_bind_int(stmt, 5, task.isCompleted() ? 1 : 0);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Errore nell'esecuzione di addTask: " 
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseManager::updateTask(const Task& task) {
    const char* updateQuery =
        "UPDATE tasks SET title = ?, description = ?, due_date = ?, priority = ?, completed = ? WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db.get(), updateQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di updateTask: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Bind dei parametri
    sqlite3_bind_text(stmt, 1, task.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, task.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 3, static_cast<sqlite3_int64>(task.getDueDate()));
    sqlite3_bind_int(stmt, 4, static_cast<int>(task.getPriority()));
    sqlite3_bind_int(stmt, 5, task.isCompleted() ? 1 : 0);
    sqlite3_bind_int(stmt, 6, task.getId());

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Errore nell'esecuzione di updateTask: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool DatabaseManager::removeTask(int id) {
    const char* deleteQuery = "DELETE FROM tasks WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db.get(), deleteQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di removeTask: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Errore nell'esecuzione di removeTask: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

std::vector<Task> DatabaseManager::getAllTasks() {
    std::vector<Task> tasks;
    const char* selectQuery = "SELECT id, title, description, due_date, priority, completed FROM tasks;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(m_db.get(), selectQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di getAllTasks: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return tasks;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* title = sqlite3_column_text(stmt, 1);
        const unsigned char* description = sqlite3_column_text(stmt, 2);
        sqlite3_int64 dueDateInt = sqlite3_column_int64(stmt, 3);
        int priorityInt = sqlite3_column_int(stmt, 4);
        int completedInt = sqlite3_column_int(stmt, 5);

        Priority priority = Priority::Medium; // Default
        switch (priorityInt) {
            case 1:
                priority = Priority::High;
                break;
            case 2:
                priority = Priority::Medium;
                break;
            case 3:
                priority = Priority::Low;
                break;
            default:
                priority = Priority::Medium;
        }

        tasks.emplace_back(
            id,
            title ? reinterpret_cast<const char*>(title) : "",
            description ? reinterpret_cast<const char*>(description) : "",
            static_cast<std::time_t>(dueDateInt),
            priority,
            completedInt != 0
        );
    }

    sqlite3_finalize(stmt);
    return tasks;
}
