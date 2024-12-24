#include <iostream>
#include "TaskManager.h"
#include "DatabaseManager.h"
#include "CSVBackup.h"
#include "UIManager.h"

int main() {
    // Percorso del database SQLite
    std::string dbPath = "tasks.db";
    // Percorso del file CSV di backup
    std::string csvPath = "backup.csv";

    DatabaseManager dbManager(dbPath);
    if (!dbManager.open()) {
        std::cerr << "Impossibile aprire il database. Uscita...\n";
        return 1;
    }

    // Creiamo la tabella se non esiste
    if (!dbManager.createTable()) {
        std::cerr << "Impossibile creare o verificare la tabella. Uscita...\n";
        return 1;
    }

    CSVBackup csvBackup(csvPath);
    UIManager uiManager;
    TaskManager taskManager(dbManager, csvBackup, uiManager);

    // Visualizza i task all'avvio
    std::cout << "\n--- Stato Attuale dei Task ---\n";
    taskManager.listTasks();

    while (true) {
        uiManager.displayMenu();
        int choice = uiManager.getUserChoice();

        switch (choice) {
            case 1:
                taskManager.addTask();
                break;
            case 2:
                taskManager.editTask();
                break;
            case 3:
                taskManager.deleteTask();
                break;
            case 4:
                taskManager.listTasks();
                break;
            case 5:
                taskManager.backupTasks();
                break;
            case 6:
                taskManager.markTaskCompleted();
                break;
            case 7:
                std::cout << "Uscita...\n";
                return 0;
            default:
                uiManager.displayError("Scelta non valida, riprovare.");
        }
    }

    return 0;
}