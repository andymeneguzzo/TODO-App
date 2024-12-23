#include "TaskManager.h"
#include <iostream>
#include <fstream>

void TaskManager::loadTasksFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) return;
    int id;
    std::string desc;
    while (file >> id && std::getline(file, desc)) {
        if(!desc.empty()) {
            tasks.emplace_back(id, desc.substr(1));
        }
    }
}

void TaskManager::saveTasksToFile(const std::string &filename) {
    std::ofstream file(filename, std::ios::trunc);
    for (auto &t : tasks) {
        file << t.getId() << " " << t.getDescription() << "\n";
    }
}

void TaskManager::addTask() {
    std::string desc;
    std::cout << "Inserisci descrizione del nuovo task: ";
    std::cin.ignore();
    std::getline(std::cin, desc);
    int newId = tasks.empty() ? 1 : tasks.back().getId() + 1;
    tasks.emplace_back(newId, desc);
    std::cout << "Task aggiunto!\n";
}

void TaskManager::modifyTask() {
    int id;
    std::cout << "Inserisci ID del task da modificare: ";
    std::cin >> id;
    for (auto &t : tasks) {
        if (t.getId() == id) {
            std::string newDesc;
            std::cout << "Inserisci la nuova descrizione: ";
            std::cin.ignore();
            std::getline(std::cin, newDesc);
            t.setDescription(newDesc);
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
                  << t.getDescription() << "\n";
    }
}