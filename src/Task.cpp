#include "Task.h"

Task::Task(int id, const std::string &desc, int priority, const std::string &deadline, const std::string &category)
    : id(id), description(desc), priority(priority), deadline(deadline), category(category) {}

int Task::getId() const {
    return id;
}

std::string Task::getDescription() const {
    return description;
}

void Task::setDescription(const std::string &newDesc) {
    description = newDesc;
}

int Task::getPriority() const {
    return priority;
}

void Task::setPriority(int newPriority) {
    priority = newPriority;
}

std::string Task::getDeadline() const {
    return deadline;
}

void Task::setDeadline(const std::string &newDeadline) {
    deadline = newDeadline;
}

std::string Task::getCategory() const {
    return category;
}

void Task::setCategory(const std::string &newCategory) {
    category = newCategory;
}