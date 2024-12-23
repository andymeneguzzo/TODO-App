#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    manager.loadTasksFromFile("tasks.txt");
    updateTaskList();
}

MainWindow::~MainWindow() {
    delete ui;
    manager.saveTasksToFile("tasks.txt");
}

void MainWindow::on_addTaskButton_clicked() {
    bool ok;
    QString desc = QInputDialog::getText(this, "Aggiungi Task", "Descrizione:", QLineEdit::Normal, "", &ok);
    if (!ok || desc.isEmpty()) return;

    int priority = QInputDialog::getInt(this, "Aggiungi Task", "Priorità:", 1, 1, 10, 1, &ok);
    if (!ok) return;

    QString deadline = QInputDialog::getText(this, "Aggiungi Task", "Scadenza (YYYY-MM-DD):", QLineEdit::Normal, "", &ok);
    if (!ok || deadline.isEmpty()) return;

    QString category = QInputDialog::getText(this, "Aggiungi Task", "Categoria:", QLineEdit::Normal, "", &ok);
    if (!ok || category.isEmpty()) return;

    int newId = manager.getTasks().empty() ? 1 : manager.getTasks().back().getId() + 1;
    manager.addTask(newId, desc.toStdString(), priority, deadline.toStdString(), category.toStdString());
    updateTaskList();
}

void MainWindow::on_modifyTaskButton_clicked() {
    bool ok;
    int id = QInputDialog::getInt(this, "Modifica Task", "ID del Task:", 1, 1, 1000, 1, &ok);
    if (!ok) return;

    Task *task = manager.getTaskById(id);
    if (!task) {
        QMessageBox::warning(this, "Errore", "Task non trovato.");
        return;
    }

    QString desc = QInputDialog::getText(this, "Modifica Task", "Nuova Descrizione:", QLineEdit::Normal, QString::fromStdString(task->getDescription()), &ok);
    if (!ok || desc.isEmpty()) return;

    int priority = QInputDialog::getInt(this, "Modifica Task", "Nuova Priorità:", task->getPriority(), 1, 10, 1, &ok);
    if (!ok) return;

    QString deadline = QInputDialog::getText(this, "Modifica Task", "Nuova Scadenza (YYYY-MM-DD):", QLineEdit::Normal, QString::fromStdString(task->getDeadline()), &ok);
    if (!ok || deadline.isEmpty()) return;

    QString category = QInputDialog::getText(this, "Modifica Task", "Nuova Categoria:", QLineEdit::Normal, QString::fromStdString(task->getCategory()), &ok);
    if (!ok || category.isEmpty()) return;

    task->setDescription(desc.toStdString());
    task->setPriority(priority);
    task->setDeadline(deadline.toStdString());
    task->setCategory(category.toStdString());
    updateTaskList();
}

void MainWindow::on_removeTaskButton_clicked() {
    bool ok;
    int id = QInputDialog::getInt(this, "Rimuovi Task", "ID del Task:", 1, 1, 1000, 1, &ok);
    if (!ok) return;

    if (!manager.removeTaskById(id)) {
        QMessageBox::warning(this, "Errore", "Task non trovato.");
    } else {
        updateTaskList();
    }
}

void MainWindow::on_listTasksButton_clicked() {
    updateTaskList();
}

void MainWindow::updateTaskList() {
    ui->taskListWidget->clear();
    for (const auto &task : manager.getTasks()) {
        ui->taskListWidget->addItem(QString::fromStdString(task.toString()));
    }
}