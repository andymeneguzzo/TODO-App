#pragma once
#include <vector>
#include <string>
#include "Task.h"

class TaskManager {
private:
    std::vector<Task> tasks;

public:
    void loadTasksFromFile(const std::string &filename);
    void saveTasksToFile(const std::string &filename);
    void addTask();
    void modifyTask();
    void removeTask();
    void listTasks();

    const std::vector<Task>& getTasks() const;
    Task* getTaskById(int id);
    bool removeTaskById(int id);
};