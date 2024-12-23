#pragma once
#include <string>

class Task {
private:
    int id;
    std::string description;
public:
    Task(int id, const std::string &desc);
    int getId() const;
    std::string getDescription() const;
    void setDescription(const std::string &newDesc);
};