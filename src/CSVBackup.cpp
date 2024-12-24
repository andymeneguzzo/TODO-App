#include "CSVBackup.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

CSVBackup::CSVBackup(const std::string& backupPath)
    : m_backupPath(backupPath)
{
}

bool CSVBackup::backupTasks(const std::vector<Task>& tasks) {
    std::ofstream file(m_backupPath);
    if (!file.is_open()) {
        std::cerr << "Impossibile aprire il file CSV di backup: " 
                  << m_backupPath << std::endl;
        return false;
    }

    // Intestazione CSV
    file << "id,title,description,due_date,priority,completed\n";

    for (const auto& task : tasks) {
        // Sostituzione di eventuali virgole nelle stringhe
        std::string safeTitle = task.getTitle();
        std::replace(safeTitle.begin(), safeTitle.end(), ',', ' ');

        std::string safeDesc = task.getDescription();
        std::replace(safeDesc.begin(), safeDesc.end(), ',', ' ');

        // Convertiamo la priorità in intero
        int priorityInt = static_cast<int>(task.getPriority());

        // Convertiamo la data/ora in formato timestamp
        std::time_t dueDate = task.getDueDate();

        file << task.getId() << ","
             << safeTitle << ","
             << safeDesc << ","
             << dueDate << ","
             << priorityInt << ","
             << (task.isCompleted() ? "1" : "0") << "\n";
    }

    file.close();
    return true;
}



std::vector<Task> CSVBackup::loadTasks() {
    std::vector<Task> tasks;
    std::ifstream file(m_backupPath);
    if (!file.is_open()) {
        std::cerr << "Impossibile aprire il file CSV di backup per il ripristino: "
                  << m_backupPath << std::endl;
        return tasks;
    }

    std::string line;
    // Saltiamo l'intestazione
    if (std::getline(file, line)) {
        if (line.find("id") == std::string::npos) {
            // Se non è un'intestazione, reinseriamo la riga nel buffer
            file.seekg(0);
        }
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, title, description, dueDateStr, priorityStr, completedStr;

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, title, ',') &&
            std::getline(ss, description, ',') &&
            std::getline(ss, dueDateStr, ',') &&
            std::getline(ss, priorityStr, ',') &&
            std::getline(ss, completedStr)) 
        {
            int id = std::stoi(idStr);
            std::time_t dueDate = static_cast<std::time_t>(std::stoll(dueDateStr));
            int priorityInt = std::stoi(priorityStr);
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

            bool completed = (completedStr == "1");

            tasks.emplace_back(id, title, description, dueDate, priority, completed);
        }
    }

    return tasks;
}
