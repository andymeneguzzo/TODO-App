#ifndef CSV_BACKUP_H
#define CSV_BACKUP_H

#include <string>
#include <vector>
#include "Task.h"

class CSVBackup {
public:
    CSVBackup(const std::string& backupPath);

    bool backupTasks(const std::vector<Task>& tasks);
    std::vector<Task> loadTasks(); // facoltativo per ripristinare da CSV

private:
    std::string m_backupPath;
};

#endif // CSV_BACKUP_H
