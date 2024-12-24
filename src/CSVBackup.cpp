#include "CSVBackup.h"
#include <fstream>
#include <sstream>
#include <iostream>

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

    // Intestazione CSV (facoltativa)
    file << "id,title,description\n";

    for (const auto& task : tasks) {
        // Sostituiamo eventuali virgole nelle stringhe
        std::string safeTitle = task.getTitle();
        std::replace(safeTitle.begin(), safeTitle.end(), ',', ' ');
        
        std::string safeDesc = task.getDescription();
        std::replace(safeDesc.begin(), safeDesc.end(), ',', ' ');

        file << task.getId() << ","
             << safeTitle << ","
             << safeDesc << "\n";
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
    // Se c'è un'intestazione, saltiamo la prima riga
    if (std::getline(file, line)) {
        // Verifica se la riga contiene la parola "id"
        if (line.find("id") == std::string::npos) {
            // Se non è veramente un header, allora la reinseriamo nel buffer
            // in modo che possa essere letta di nuovo
            file.seekg(0);
        }
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, title, description;

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, title, ',') &&
            std::getline(ss, description)) 
        {
            int id = std::stoi(idStr);
            tasks.emplace_back(id, title, description);
        }
    }

    return tasks;
}
