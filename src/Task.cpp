#include "Task.h"

Task::Task()
    : m_id(-1), m_title(""), m_description("")
{
}

Task::Task(int id, const std::string& title, const std::string& description)
    : m_id(id), m_title(title), m_description(description)
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
