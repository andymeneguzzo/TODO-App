#include "TaskManager.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <iomanip>
#include <ctime>

TaskManager::TaskManager(DatabaseManager& dbManager, CSVBackup& csvBackup)
    : m_dbManager(dbManager), m_csvBackup(csvBackup)
{
}

bool TaskManager::validateDateTime(const std::string& dateTimeStr, std::time_t& outTime) {
    // Formato atteso: YYYY-MM-DD HH:MM
    std::tm tm = {};
    std::istringstream ss(dateTimeStr);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
    if (ss.fail()) {
        return false;
    }
    outTime = std::mktime(&tm);
    return true;
}

void TaskManager::addTask() {
    std::string title, description, dueDateStr;
    int priority;

    std::cout << "Titolo del task: ";
    std::getline(std::cin, title);
    std::cout << "Descrizione del task: ";
    std::getline(std::cin, description);

    // Input e validazione della data/ora di scadenza
    std::time_t dueDate;
    while (true) {
        std::cout << "Data e ora di scadenza (formato YYYY-MM-DD HH:MM): ";
        std::getline(std::cin, dueDateStr);
        if (validateDateTime(dueDateStr, dueDate)) {
            break;
        } else {
            std::cout << "Formato data/ora non valido. Riprova.\n";
        }
    }

    // Input della priorità
    while (true) {
        std::cout << "Priorità (1 = Alta, 2 = Media, 3 = Bassa): ";
        std::cin >> priority;
        if (std::cin.fail() || priority < 1 || priority > 3) {
            std::cin.clear(); // Pulisce lo stato di errore
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
            std::cout << "Priorità non valida. Riprova.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
            break;
        }
    }

    Task newTask(-1, title, description, dueDate, priority, false); // Completato = false
    if (m_dbManager.addTask(newTask)) {
        std::cout << "Task aggiunto correttamente.\n";
    } else {
        std::cout << "Errore durante l'aggiunta del task.\n";
    }
}

void TaskManager::editTask() {
    int id;
    std::cout << "ID del task da modificare: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer

    // Presumiamo che l'utente voglia modificare tutti i campi
    std::string newTitle, newDescription, dueDateStr;
    int newPriority;
    bool newCompleted;

    std::cout << "Nuovo titolo: ";
    std::getline(std::cin, newTitle);
    std::cout << "Nuova descrizione: ";
    std::getline(std::cin, newDescription);

    // Input e validazione della nuova data/ora di scadenza
    std::time_t newDueDate;
    while (true) {
        std::cout << "Nuova data e ora di scadenza (formato YYYY-MM-DD HH:MM): ";
        std::getline(std::cin, dueDateStr);
        if (validateDateTime(dueDateStr, newDueDate)) {
            break;
        } else {
            std::cout << "Formato data/ora non valido. Riprova.\n";
        }
    }

    // Input della nuova priorità
    while (true) {
        std::cout << "Nuova priorità (1 = Alta, 2 = Media, 3 = Bassa): ";
        std::cin >> newPriority;
        if (std::cin.fail() || newPriority < 1 || newPriority > 3) {
            std::cin.clear(); // Pulisce lo stato di errore
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
            std::cout << "Priorità non valida. Riprova.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
            break;
        }
    }

    // Input dello stato completato
    while (true) {
        std::cout << "Task completato? (0 = No, 1 = Sì): ";
        int completedInt;
        std::cin >> completedInt;
        if (std::cin.fail() || (completedInt != 0 && completedInt != 1)) {
            std::cin.clear(); // Pulisce lo stato di errore
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
            std::cout << "Input non valido. Riprova.\n";
        } else {
            newCompleted = (completedInt == 1);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
            break;
        }
    }

    Task updatedTask(id, newTitle, newDescription, newDueDate, newPriority, newCompleted);
    if (m_dbManager.updateTask(updatedTask)) {
        std::cout << "Task modificato correttamente.\n";
    } else {
        std::cout << "Errore durante la modifica del task.\n";
    }
}

void TaskManager::deleteTask() {
    int id;
    std::cout << "ID del task da eliminare: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (m_dbManager.removeTask(id)) {
        std::cout << "Task eliminato correttamente.\n";
    } else {
        std::cout << "Errore durante l'eliminazione del task.\n";
    }
}

void TaskManager::listTasks() {
    auto tasks = m_dbManager.getAllTasks();
    if (tasks.empty()) {
        std::cout << "Non ci sono task.\n";
        return;
    }

    std::cout << "Elenco dei Task:\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(20) << "Titolo"
              << std::setw(25) << "Scadenza"
              << std::setw(10) << "Priorità"
              << std::setw(15) << "Stato"
              << "Descrizione\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    std::time_t currentTime = std::time(nullptr);

    for (const auto& task : tasks) {
        // Formattiamo la data/ora di scadenza
        char dueDateStr[20];
        if (task.getDueDate() != 0) {
            std::time_t dueDate = task.getDueDate();
            std::tm* tm_info = std::localtime(&dueDate);
            std::strftime(dueDateStr, sizeof(dueDateStr), "%Y-%m-%d %H:%M", tm_info);
        } else {
            std::strcpy(dueDateStr, "N/A");
        }

        // Determiniamo lo stato del task
        std::string status;
        if (task.isCompleted()) {
            status = "Completato";
        } else if (task.getDueDate() != 0 && task.getDueDate() < currentTime) {
            status = "In Ritardo";
        } else {
            status = "Da Completare";
        }

        // Convertiamo la priorità in stringa
        std::string priorityStr;
        switch (task.getPriority()) {
            case 1:
                priorityStr = "Alta";
                break;
            case 2:
                priorityStr = "Media";
                break;
            case 3:
                priorityStr = "Bassa";
                break;
            default:
                priorityStr = "Sconosciuta";
        }

        std::cout << std::left << std::setw(5) << task.getId()
                  << std::setw(20) << task.getTitle()
                  << std::setw(25) << dueDateStr
                  << std::setw(10) << priorityStr
                  << std::setw(15) << status
                  << task.getDescription() << "\n";
    }
}

void TaskManager::backupTasks() {
    auto tasks = m_dbManager.getAllTasks();
    if (m_csvBackup.backupTasks(tasks)) {
        std::cout << "Backup su file CSV completato correttamente.\n";
    } else {
        std::cout << "Errore durante il backup.\n";
    }
}

void TaskManager::markTaskCompleted() {
    int id;
    std::cout << "ID del task da segnare come completato: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Recuperiamo il task esistente
    auto tasks = m_dbManager.getAllTasks();
    auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& task) {
        return task.getId() == id;
    });

    if (it != tasks.end()) {
        it->setCompleted(true);
        if (m_dbManager.updateTask(*it)) {
            std::cout << "Task segnato come completato.\n";
        } else {
            std::cout << "Errore durante l'aggiornamento del task.\n";
        }
    } else {
        std::cout << "Task con ID " << id << " non trovato.\n";
    }
}