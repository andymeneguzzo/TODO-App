#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Task.h"
#include "TaskManager.h"

static std::vector<Task> tasks;

// static void loadTasksFromFile(const std::string &filename) {
//     std::ifstream file(filename);
//     if (!file) return;
//     int id;
//     std::string desc;
//     while (file >> id && std::getline(file, desc)) {
//         if(!desc.empty()) {
//             tasks.emplace_back(id, desc.substr(1)); // remove leading space
//         }
//     }
// }

// static void saveTasksToFile(const std::string &filename) {
//     std::ofstream file(filename, std::ios::trunc);
//     for (auto &t : tasks) {
//         file << t.getId() << " " << t.getDescription() << "\n";
//     }
// }

// static void addTask() {
//     std::string desc;
//     std::cout << "Inserisci descrizione del nuovo task: ";
//     std::cin.ignore();
//     std::getline(std::cin, desc);
//     int newId = tasks.empty() ? 1 : tasks.back().getId() + 1;
//     tasks.emplace_back(newId, desc);
//     std::cout << "Task aggiunto!\n";
// }

// static void modifyTask() {
//     int id;
//     std::cout << "Inserisci ID del task da modificare: ";
//     std::cin >> id;
//     for (auto &t : tasks) {
//         if (t.getId() == id) {
//             std::string newDesc;
//             std::cout << "Inserisci la nuova descrizione: ";
//             std::cin.ignore();
//             std::getline(std::cin, newDesc);
//             t.setDescription(newDesc);
//             std::cout << "Task modificato!\n";
//             return;
//         }
//     }
//     std::cout << "Task non trovato.\n";
// }

// static void removeTask() {
//     int id;
//     std::cout << "Inserisci ID del task da rimuovere: ";
//     std::cin >> id;
//     for (size_t i = 0; i < tasks.size(); i++) {
//         if (tasks[i].getId() == id) {
//             tasks.erase(tasks.begin() + i);
//             std::cout << "Task rimosso!\n";
//             return;
//         }
//     }
//     std::cout << "Task non trovato.\n";
// }

// static void listTasks() {
//     if (tasks.empty()) {
//         std::cout << "Nessun task presente.\n";
//         return;
//     }
//     for (auto &t : tasks) {
//         std::cout << "ID: " << t.getId() << " - " 
//                   << t.getDescription() << "\n";
//     }
// }

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