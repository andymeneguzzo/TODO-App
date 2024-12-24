#include "Task.h"

Task::Task()
    : m_id(-1), m_title(""), m_description(""),
      m_dueDate(0), m_priority(1), m_completed(false)
{
}

Task::Task(int id, const std::string& title, const std::string& description,
           const std::time_t dueDate, int priority, bool completed)
    : m_id(id), m_title(title), m_description(description),
      m_dueDate(dueDate), m_priority(priority), m_completed(completed) 
{
}

int Task::getId() const {
    return m_id;
}

void Task::setId(int id) {
    m_id = id;
}

std::string Task::getTitle() const {
    return m_title;
}

void Task::setTitle(const std::string& title) {
    m_title = title;
}

std::string Task::getDescription() const {
    return m_description;
}

void Task::setDescription(const std::string& description) {
    m_description = description;
}

std::time_t Task::getDueDate() const {
    return m_dueDate;
}

void Task::setDueDate(const std::time_t dueDate) {
    m_dueDate = dueDate;
}

int Task::getPriority() const {
    return m_priority;
}

void Task::setPriority(int priority) {
    m_priority = priority;
}

bool Task::isCompleted() const {
    return m_completed;
}

void Task::setCompleted(bool completed) {
    m_completed = completed;
}
