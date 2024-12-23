#include <iostream>
#include "Task.h"
#include "TaskManager.h"

int main() {
    TaskManager manager;
    manager.loadTasksFromFile("tasks.txt");
    
    int choice = 0;
    do {
        std::cout << "\n--- TO-DO-LIST MENU ---\n"
                  << "1. Aggiungi Task\n"
                  << "2. Modifica Task\n"
                  << "3. Rimuovi Task\n"
                  << "4. Visualizza Tasks\n"
                  << "5. Esci\n"
                  << "Scelta: ";
        std::cin >> choice;
        switch (choice) {
            case 1: manager.addTask(); break;
            case 2: manager.modifyTask(); break;
            case 3: manager.removeTask(); break;
            case 4: manager.listTasks(); break;
            case 5: 
                manager.saveTasksToFile("tasks.txt");
                std::cout << "Uscita...\n";
                break;
            default: 
                std::cout << "Scelta non valida.\n";
                break;
        }
    } while (choice != 5);

    return 0;
}