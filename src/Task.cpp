#include "Task.h"

Task::Task(int id, const std::string &desc) : id(id), description(desc) {}

int Task::getId() const {
    return id;
}

std::string Task::getDescription() const {
    return description;
}

void Task::setDescription(const std::string &newDesc) {
    description = newDesc;
}