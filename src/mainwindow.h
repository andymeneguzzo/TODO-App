#pragma once

#include <QMainWindow>
#include "TaskManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addTaskButton_clicked();
    void on_modifyTaskButton_clicked();
    void on_removeTaskButton_clicked();
    void on_listTasksButton_clicked();

private:
    Ui::MainWindow *ui;
    TaskManager manager;
    void updateTaskList();
};