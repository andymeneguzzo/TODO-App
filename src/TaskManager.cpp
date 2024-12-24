#include "TaskManager.h"
#include <iostream>

TaskManager::TaskManager(DatabaseManager& dbManager, CSVBackup& csvBackup)
    : m_dbManager(dbManager), m_csvBackup(csvBackup)
{
}

void TaskManager::addTask() {
    std::string title, description;
    std::cout << "Titolo del task: ";
    std::getline(std::cin, title);
    std::cout << "Descrizione del task: ";
    std::getline(std::cin, description);

    Task newTask(-1, title, description);
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
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // pulisce il buffer

    std::string newTitle, newDescription;
    std::cout << "Nuovo titolo: ";
    std::getline(std::cin, newTitle);
    std::cout << "Nuova descrizione: ";
    std::getline(std::cin, newDescription);

    Task updatedTask(id, newTitle, newDescription);
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
    for (const auto& task : tasks) {
        std::cout << "ID: " << task.getId()
                  << " | Titolo: " << task.getTitle()
                  << " | Descrizione: " << task.getDescription() << "\n";
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
