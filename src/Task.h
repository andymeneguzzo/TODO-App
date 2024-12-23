#pragma once
#include <string>

class Task {
private:
    int id;
    std::string description;
    int priority;
    std::string deadline;
    std::string category;

public:
    Task(int id, const std::string &desc, int priority, const std::string &deadline, const std::string &category);
    int getId() const;
    std::string getDescription() const;
    void setDescription(const std::string &newDesc);
    int getPriority() const;
    void setPriority(int newPriority);
    std::string getDeadline() const;
    void setDeadline(const std::string &newDeadline);
    std::string getCategory() const;
    void setCategory(const std::string &newCategory);
};