#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>
#include "Task.h"

// Per la gestione diretta di SQLite
#include <sqlite3.h>

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    bool open();
    void close();

    bool createTable();
    bool addTask(const Task& task);
    bool updateTask(const Task& task);
    bool removeTask(int id);
    std::vector<Task> getAllTasks();

private:
    std::string m_dbPath;
    sqlite3* m_db;
};

#endif // DATABASE_MANAGER_H
