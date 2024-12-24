#include <iostream>
#include <limits>
#include "TaskManager.h"
#include "DatabaseManager.h"
#include "CSVBackup.h"

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
    TaskManager taskManager(dbManager, csvBackup);

    while (true) {
        std::cout << "\n--- To-Do List App ---\n";
        std::cout << "1. Aggiungi Task\n";
        std::cout << "2. Modifica Task\n";
        std::cout << "3. Elimina Task\n";
        std::cout << "4. Visualizza Task\n";
        std::cout << "5. Esegui Backup CSV\n";
        std::cout << "6. Esci\n";
        std::cout << "Seleziona un'operazione: ";

        int choice;
        std::cin >> choice;

        // Pulisce il buffer d'ingresso per le chiamate getline()
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
                std::cout << "Uscita...\n";
                dbManager.close();
                return 0;
            default:
                std::cout << "Scelta non valida, riprovare.\n";
        }
    }

    // Chiudiamo il database prima di terminare
    dbManager.close();
    return 0;
}
