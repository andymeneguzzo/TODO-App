// TaskManager.h

#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <string>
#include <vector>
#include "Task.h"
#include "DatabaseManager.h"
#include "CSVBackup.h"

class TaskManager {
public:
    TaskManager(DatabaseManager& dbManager, CSVBackup& csvBackup);

    void addTask();
    void editTask();
    void deleteTask();
    void listTasks();
    void backupTasks();
    void markTaskCompleted(); // Nuovo metodo per segnare un task come completato

private:
    DatabaseManager& m_dbManager;
    CSVBackup& m_csvBackup;

    bool validateDateTime(const std::string& dateTimeStr, std::time_t& outTime); // Nuovo metodo di validazione
};

#endif // TASK_MANAGER_H
