// UIManager.h

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "Task.h"
#include <string>
#include <vector>

class UIManager {
public:
    void displayMenu();
    int getUserChoice();
    
    // Metodi per ottenere input dall'utente
    std::string getTaskTitle();
    std::string getTaskDescription();
    std::time_t getTaskDueDate();
    Priority getTaskPriority();
    bool getTaskCompletionStatus();

    int getTaskId();

    // Metodi per visualizzare i task
    void displayTasks(const std::vector<Task>& tasks);
    void displayMessage(const std::string& message);
    void displayError(const std::string& error);
};

#endif // UIMANAGER_H
