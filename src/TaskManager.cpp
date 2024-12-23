#include "TaskManager.h"
#include <iostream>
#include <fstream>

void TaskManager::loadTasksFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) return;
    int id, priority;
    std::string desc, deadline, category;
    while (file >> id >> priority >> deadline >> category && std::getline(file, desc)) {
        if(!desc.empty()) {
            tasks.emplace_back(id, desc.substr(1), priority, deadline, category);
        }
    }
}

void TaskManager::saveTasksToFile(const std::string &filename) {
    std::ofstream file(filename, std::ios::trunc);
    for (auto &t : tasks) {
        file << t.getId() << " " << t.getPriority() << " " << t.getDeadline() << " " << t.getCategory() << " " << t.getDescription() << "\n";
    }
}

void TaskManager::addTask() {
    std::string desc, deadline, category;
    int priority;
    std::cout << "Inserisci descrizione del nuovo task: ";
    std::cin.ignore();
    std::getline(std::cin, desc);
    std::cout << "Inserisci priorità del nuovo task (1,2,...): ";
    std::cin >> priority;
    std::cout << "Inserisci scadenza del nuovo task (YYYY-MM-DD): ";
    std::cin >> deadline;
    std::cout << "Inserisci categoria del nuovo task: ";
    std::cin >> category;
    int newId = tasks.empty() ? 1 : tasks.back().getId() + 1;
    tasks.emplace_back(newId, desc, priority, deadline, category);
    std::cout << "Task aggiunto!\n";
}

void TaskManager::modifyTask() {
    int id;
    std::cout << "Inserisci ID del task da modificare: ";
    std::cin >> id;
    for (auto &t : tasks) {
        if (t.getId() == id) {
            std::string newDesc, newDeadline, newCategory;
            int newPriority;
            std::cout << "Inserisci la nuova descrizione: ";
            std::cin.ignore();
            std::getline(std::cin, newDesc);
            std::cout << "Inserisci la nuova priorità: ";
            std::cin >> newPriority;
            std::cout << "Inserisci la nuova scadenza (YYYY-MM-DD): ";
            std::cin >> newDeadline;
            std::cout << "Inserisci la nuova categoria: ";
            std::cin >> newCategory;
            t.setDescription(newDesc);
            t.setPriority(newPriority);
            t.setDeadline(newDeadline);
            t.setCategory(newCategory);
            std::cout << "Task modificato!\n";
            return;
        }
    }
    std::cout << "Task non trovato.\n";
}

void TaskManager::removeTask() {
    int id;
    std::cout << "Inserisci ID del task da rimuovere: ";
    std::cin >> id;
    for (size_t i = 0; i < tasks.size(); i++) {
        if (tasks[i].getId() == id) {
            tasks.erase(tasks.begin() + i);
            std::cout << "Task rimosso!\n";
            return;
        }
    }
    std::cout << "Task non trovato.\n";
}

void TaskManager::listTasks() {
    if (tasks.empty()) {
        std::cout << "Nessun task presente.\n";
        return;
    }
    for (auto &t : tasks) {
        std::cout << "ID: " << t.getId() << " - " 
                  << "Descrizione: " << t.getDescription() << " - "
                  << "Priorità: " << t.getPriority() << " - "
                  << "Scadenza: " << t.getDeadline() << " - "
                  << "Categoria: " << t.getCategory() << "\n";
    }
}