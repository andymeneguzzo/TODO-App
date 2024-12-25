#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "Task.h"
#include <sqlite3.h>

// Deleter personalizzato per sqlite3*
struct SQLiteDeleter {
    void operator()(sqlite3* db) const {
        if (db) {
            sqlite3_close(db);
        }
    }
};

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    bool isOpen() const;
    bool migrateSchema();
    bool open();
    bool createTable();
    bool addTask(const Task& task);
    bool updateTask(const Task& task);
    bool removeTask(int id);
    std::vector<Task> getAllTasks();

private:
    std::string m_dbPath;
    std::unique_ptr<sqlite3, SQLiteDeleter> m_db;
};

#endif // DATABASE_MANAGER_H
