#include "TaskManager.h"
#include <iostream>
#include <algorithm>

TaskManager::TaskManager(DatabaseManager& dbManager, CSVBackup& csvBackup, UIManager& uiManager)
    : m_dbManager(dbManager), m_csvBackup(csvBackup), m_uiManager(uiManager)
{
}

// bool TaskManager::validateDateTime(const std::string& dateTimeStr, std::time_t& outTime) {
//     // Formato atteso: YYYY-MM-DD HH:MM
//     std::tm tm = {};
//     std::istringstream ss(dateTimeStr);
//     ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
//     if (ss.fail()) {
//         return false;
//     }
//     outTime = std::mktime(&tm);
//     return true;
// }

void TaskManager::addTask() {
    std::string title = m_uiManager.getTaskTitle();
    std::string description = m_uiManager.getTaskDescription();
    std::time_t dueDate = m_uiManager.getTaskDueDate();
    Priority priority = m_uiManager.getTaskPriority();

    Task newTask(-1, title, description, dueDate, priority, false);
    if (m_dbManager.addTask(newTask)) {
        m_uiManager.displayMessage("Task aggiunto correttamente.");
    } else {
        m_uiManager.displayError("Errore durante l'aggiunta del task.");
    }
}

void TaskManager::editTask() {
    int id = m_uiManager.getTaskId();

    // Recuperiamo il task esistente
    auto tasks = m_dbManager.getAllTasks();
    auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& task) {
        return task.getId() == id;
    });

    if (it == tasks.end()) {
        m_uiManager.displayError("Task con ID " + std::to_string(id) + " non trovato.");
        return;
    }

    std::string newTitle = m_uiManager.getTaskTitle();
    std::string newDescription = m_uiManager.getTaskDescription();
    std::time_t newDueDate = m_uiManager.getTaskDueDate();
    Priority newPriority = m_uiManager.getTaskPriority();
    bool newCompleted = m_uiManager.getTaskCompletionStatus();

    Task updatedTask(id, newTitle, newDescription, newDueDate, newPriority, newCompleted);
    if (m_dbManager.updateTask(updatedTask)) {
        m_uiManager.displayMessage("Task modificato correttamente.");
    } else {
        m_uiManager.displayError("Errore durante la modifica del task.");
    }
}

void TaskManager::deleteTask() {
    int id = m_uiManager.getTaskId();

    if (m_dbManager.removeTask(id)) {
        m_uiManager.displayMessage("Task eliminato correttamente.");
    } else {
        m_uiManager.displayError("Errore durante l'eliminazione del task.");
    }
}

void TaskManager::listTasks() {
    auto tasks = m_dbManager.getAllTasks();
    m_uiManager.displayTasks(tasks);
}

void TaskManager::backupTasks() {
    auto tasks = m_dbManager.getAllTasks();
    if (m_csvBackup.backupTasks(tasks)) {
        m_uiManager.displayMessage("Backup su file CSV completato correttamente.");
    } else {
        m_uiManager.displayError("Errore durante il backup.");
    }
}

void TaskManager::markTaskCompleted() {
    int id = m_uiManager.getTaskId();

    // Recuperiamo il task esistente
    auto tasks = m_dbManager.getAllTasks();
    auto it = std::find_if(tasks.begin(), tasks.end(), [id](const Task& task) {
        return task.getId() == id;
    });

    if (it != tasks.end()) {
        if (it->isCompleted()) {
            m_uiManager.displayMessage("Il task è già segnato come completato.");
            return;
        }

        it->setCompleted(true);
        if (m_dbManager.updateTask(*it)) {
            m_uiManager.displayMessage("Task segnato come completato.");
        } else {
            m_uiManager.displayError("Errore durante l'aggiornamento del task.");
        }
    } else {
        m_uiManager.displayError("Task con ID " + std::to_string(id) + " non trovato.");
    }
}