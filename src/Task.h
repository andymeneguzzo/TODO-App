#ifndef TASK_H
#define TASK_H

#include <string>
#include <ctime>

class Task {
public:
    Task();
    Task(int id, const std::string& title, const std::string& description,
         const std::time_t dueDate, int priority, bool completed);

    int getId() const;
    void setId(int id);

    std::string getTitle() const;
    void setTitle(const std::string& title);

    std::string getDescription() const;
    void setDescription(const std::string& description);

    std::time_t getDueDate() const;
    void setDueDate(const std::time_t dueDate);

    int getPriority() const;
    void setPriority(int priority);

    bool isCompleted() const;
    void setCompleted(bool completed);


private:
    int m_id;
    std::string m_title;
    std::string m_description;
    std::time_t m_dueDate;
    int m_priority;
    bool m_completed;
};

#endif // TASK_H
