// UIManager.cpp

#include "UIManager.h"
#include "Validator.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <limits>

// Metodo per visualizzare il menu principale
void UIManager::displayMenu() {
    std::cout << "\n--- To-Do List App ---\n";
    std::cout << "1. Aggiungi Task\n";
    std::cout << "2. Modifica Task\n";
    std::cout << "3. Elimina Task\n";
    std::cout << "4. Visualizza Task\n";
    std::cout << "5. Esegui Backup CSV\n";
    std::cout << "6. Segna Task come Completato\n";
    std::cout << "7. Esci\n";
    std::cout << "Seleziona un'operazione: ";
}

// Metodo per ottenere la scelta dell'utente
int UIManager::getUserChoice() {
    int choice;
    std::cin >> choice;
    // Pulisce il buffer d'ingresso per le chiamate getline()
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

// Metodo per ottenere il titolo del task
std::string UIManager::getTaskTitle() {
    std::string title;
    std::cout << "Titolo del task: ";
    std::getline(std::cin, title);
    return title;
}

// Metodo per ottenere la descrizione del task
std::string UIManager::getTaskDescription() {
    std::string description;
    std::cout << "Descrizione del task: ";
    std::getline(std::cin, description);
    return description;
}

// Metodo per ottenere e validare la data/ora di scadenza
std::time_t UIManager::getTaskDueDate() {
    std::string dueDateStr;
    std::time_t dueDate;
    while (true) {
        std::cout << "Data e ora di scadenza (formato YYYY-MM-DD HH:MM): ";
        std::getline(std::cin, dueDateStr);
        if (Validator::validateDateTime(dueDateStr, dueDate)) {
            break;
        } else {
            std::cout << "Formato data/ora non valido. Riprova.\n";
        }
    }
    return dueDate;
}

// Metodo per ottenere e validare la priorità del task
Priority UIManager::getTaskPriority() {
    int priorityInt;
    while (true) {
        std::cout << "Priorità (1 = Alta, 2 = Media, 3 = Bassa): ";
        std::cin >> priorityInt;
        if (Validator::validatePriority(priorityInt)) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
            break;
        } else {
            std::cin.clear(); // Pulisce lo stato di errore
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
            std::cout << "Priorità non valida. Riprova.\n";
        }
    }

    switch (priorityInt) {
        case 1:
            return Priority::High;
        case 2:
            return Priority::Medium;
        case 3:
            return Priority::Low;
        default:
            return Priority::Medium; // Default fallback
    }
}

// Metodo per ottenere lo stato di completamento del task
bool UIManager::getTaskCompletionStatus() {
    int completedInt;
    while (true) {
        std::cout << "Task completato? (0 = No, 1 = Sì): ";
        std::cin >> completedInt;
        if (completedInt == 0 || completedInt == 1) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
            break;
        } else {
            std::cin.clear(); // Pulisce lo stato di errore
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
            std::cout << "Input non valido. Riprova.\n";
        }
    }
    return (completedInt == 1);
}

// Metodo per ottenere l'ID del task
int UIManager::getTaskId() {
    int id;
    std::cout << "ID del task: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pulisce il buffer
    return id;
}

// Metodo per visualizzare i task
void UIManager::displayTasks(const std::vector<Task>& tasks) {
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
            case Priority::High:
                priorityStr = "Alta";
                break;
            case Priority::Medium:
                priorityStr = "Media";
                break;
            case Priority::Low:
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

// Metodo per visualizzare un messaggio generico
void UIManager::displayMessage(const std::string& message) {
    std::cout << message << "\n";
}

// Metodo per visualizzare un messaggio di errore
void UIManager::displayError(const std::string& error) {
    std::cerr << "Errore: " << error << "\n";
}
