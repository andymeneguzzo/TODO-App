// DatabaseManager.cpp

#include "DatabaseManager.h"
#include <iostream>
#include <cassert>

// Funzione di utilità per verificare che il database sia aperto
bool DatabaseManager::isOpen() const {
    return m_db && sqlite3_db_filename(m_db.get(), "main") != nullptr;
}

DatabaseManager::DatabaseManager(const std::string& dbPath)
    : m_dbPath(dbPath), m_db(nullptr)
{
}

DatabaseManager::~DatabaseManager() {
    // La chiusura del database è gestita dallo smart pointer
}

bool DatabaseManager::open() {
    sqlite3* dbRaw = nullptr;
    int rc = sqlite3_open(m_dbPath.c_str(), &dbRaw);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nell'aprire il database: " 
                  << sqlite3_errmsg(dbRaw) << std::endl;
        if (dbRaw) {
            sqlite3_close(dbRaw);
        }
        return false;
    }
    m_db.reset(dbRaw);
    return true;
}

bool DatabaseManager::createTable() {
    if (!isOpen()) {
        std::cerr << "Database non aperto. Impossibile creare la tabella." << std::endl;
        return false;
    }

    const char* createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT,
            due_date INTEGER,
            priority INTEGER,
            completed INTEGER DEFAULT 0
        );
    )";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db.get(), createTableQuery, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nella creazione della tabella: " 
                  << (errMsg ? errMsg : "Unknown error") << std::endl;
        if (errMsg) {
            sqlite3_free(errMsg);
        }
        return false;
    }
    return true;
}

bool DatabaseManager::migrateSchema() {
    if (!isOpen()) {
        std::cerr << "Database non aperto. Impossibile migrare lo schema." << std::endl;
        return false;
    }

    // Controlla la versione corrente dello schema
    const char* getVersionQuery = "PRAGMA user_version;";
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db.get(), getVersionQuery, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di PRAGMA user_version: " 
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    int currentVersion = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        currentVersion = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    // Supponiamo che la versione corrente dovrebbe essere 1
    if (currentVersion < 1) {
        // Verifica se la colonna due_date esiste già
        const char* checkColumnQuery = "PRAGMA table_info(tasks);";
        rc = sqlite3_prepare_v2(m_db.get(), checkColumnQuery, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Errore nella preparazione di PRAGMA table_info: " 
                      << sqlite3_errmsg(m_db.get()) << std::endl;
            return false;
        }

        bool columnExists = false;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* columnName = sqlite3_column_text(stmt, 1);
            if (columnName && std::string(reinterpret_cast<const char*>(columnName)) == "due_date") {
                columnExists = true;
                break;
            }
        }
        sqlite3_finalize(stmt);

        if (!columnExists) {
            // Aggiungi la colonna due_date se non esiste
            const char* addDueDateQuery = "ALTER TABLE tasks ADD COLUMN due_date INTEGER;";
            char* errMsg = nullptr;
            rc = sqlite3_exec(m_db.get(), addDueDateQuery, nullptr, nullptr, &errMsg);
            if (rc != SQLITE_OK) {
                std::cerr << "Errore nell'aggiungere la colonna due_date: " 
                          << (errMsg ? errMsg : "Unknown error") << std::endl;
                if (errMsg) {
                    sqlite3_free(errMsg);
                }
                return false;
            }
        }

        // Aggiorna la versione dello schema
        const char* setVersionQuery = "PRAGMA user_version = 1;";
        char* errMsg = nullptr;
        rc = sqlite3_exec(m_db.get(), setVersionQuery, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "Errore nell'impostare la versione dello schema: " 
                      << (errMsg ? errMsg : "Unknown error") << std::endl;
            if (errMsg) {
                sqlite3_free(errMsg);
            }
            return false;
        }

        std::cout << "Migrazione dello schema completata: Colonna due_date aggiunta." << std::endl;
    }

    return true;
}

bool DatabaseManager::addTask(const Task& task) {
    if (!isOpen()) {
        std::cerr << "Database non aperto. Impossibile aggiungere il task." << std::endl;
        return false;
    }

    // Controllo preventivo per il titolo vuoto
    if (task.getTitle().empty()) {
        std::cerr << "Errore: Il titolo del task non può essere vuoto." << std::endl;
        return false;
    }

    const char* insertQuery = 
        "INSERT INTO tasks (title, description, due_date, priority, completed) VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db.get(), insertQuery, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di addTask: " 
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Assicurati che lo statement venga finalizzato
    struct StmtFinalizer {
        sqlite3_stmt* stmt;
        ~StmtFinalizer() {
            if (stmt) {
                sqlite3_finalize(stmt);
            }
        }
    } finalizer{ stmt };

    // Bind dei parametri
    rc = sqlite3_bind_text(stmt, 1, task.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding del titolo: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    rc = sqlite3_bind_text(stmt, 2, task.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding della descrizione: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    rc = sqlite3_bind_int64(stmt, 3, static_cast<sqlite3_int64>(task.getDueDate()));
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding della data di scadenza: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    rc = sqlite3_bind_int(stmt, 4, static_cast<int>(task.getPriority()));
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding della priorità: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    rc = sqlite3_bind_int(stmt, 5, task.isCompleted() ? 1 : 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding dello stato di completamento: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Esecuzione dello statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Errore nell'esecuzione di addTask: " 
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Il destructor di StmtFinalizer si occuperà di finalizzare lo statement
    return true;
}

bool DatabaseManager::updateTask(const Task& task) {
    if (!isOpen()) {
        std::cerr << "Database non aperto. Impossibile aggiornare il task." << std::endl;
        return false;
    }

    // Controllo preventivo per il titolo vuoto
    if (task.getTitle().empty()) {
        std::cerr << "Errore: Il titolo del task non può essere vuoto." << std::endl;
        return false;
    }

    const char* updateQuery =
        "UPDATE tasks SET title = ?, description = ?, due_date = ?, priority = ?, completed = ? WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db.get(), updateQuery, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di updateTask: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Assicurati che lo statement venga finalizzato
    struct StmtFinalizer {
        sqlite3_stmt* stmt;
        ~StmtFinalizer() {
            if (stmt) {
                sqlite3_finalize(stmt);
            }
        }
    } finalizer{ stmt };

    // Bind dei parametri
    rc = sqlite3_bind_text(stmt, 1, task.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding del titolo: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    rc = sqlite3_bind_text(stmt, 2, task.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding della descrizione: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    rc = sqlite3_bind_int64(stmt, 3, static_cast<sqlite3_int64>(task.getDueDate()));
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding della data di scadenza: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    rc = sqlite3_bind_int(stmt, 4, static_cast<int>(task.getPriority()));
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding della priorità: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    rc = sqlite3_bind_int(stmt, 5, task.isCompleted() ? 1 : 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding dello stato di completamento: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    rc = sqlite3_bind_int(stmt, 6, task.getId());
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding dell'ID: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Esecuzione dello statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Errore nell'esecuzione di updateTask: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Il destructor di StmtFinalizer si occuperà di finalizzare lo statement
    return true;
}

bool DatabaseManager::removeTask(int id) {
    if (!isOpen()) {
        std::cerr << "Database non aperto. Impossibile rimuovere il task." << std::endl;
        return false;
    }

    const char* deleteQuery = "DELETE FROM tasks WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db.get(), deleteQuery, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di removeTask: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Assicurati che lo statement venga finalizzato
    struct StmtFinalizer {
        sqlite3_stmt* stmt;
        ~StmtFinalizer() {
            if (stmt) {
                sqlite3_finalize(stmt);
            }
        }
    } finalizer{ stmt };

    // Bind del parametro ID
    rc = sqlite3_bind_int(stmt, 1, id);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nel binding dell'ID: " << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Esecuzione dello statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Errore nell'esecuzione di removeTask: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return false;
    }

    // Il destructor di StmtFinalizer si occuperà di finalizzare lo statement
    return true;
}

std::vector<Task> DatabaseManager::getAllTasks() {
    std::vector<Task> tasks;

    if (!isOpen()) {
        std::cerr << "Database non aperto. Impossibile recuperare i task." << std::endl;
        return tasks;
    }

    const char* selectQuery = "SELECT id, title, description, due_date, priority, completed FROM tasks;";

    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(m_db.get(), selectQuery, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Errore nella preparazione di getAllTasks: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
        return tasks;
    }

    // Assicurati che lo statement venga finalizzato
    struct StmtFinalizer {
        sqlite3_stmt* stmt;
        ~StmtFinalizer() {
            if (stmt) {
                sqlite3_finalize(stmt);
            }
        }
    } finalizer{ stmt };

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* title = sqlite3_column_text(stmt, 1);
        const unsigned char* description = sqlite3_column_text(stmt, 2);
        sqlite3_int64 dueDateInt = sqlite3_column_int64(stmt, 3);
        int priorityInt = sqlite3_column_int(stmt, 4);
        int completedInt = sqlite3_column_int(stmt, 5);

        Priority priority = Priority::Medium; // Default
        switch (priorityInt) {
            case 1:
                priority = Priority::High;
                break;
            case 2:
                priority = Priority::Medium;
                break;
            case 3:
                priority = Priority::Low;
                break;
            default:
                priority = Priority::Medium;
        }

        tasks.emplace_back(
            id,
            title ? reinterpret_cast<const char*>(title) : "",
            description ? reinterpret_cast<const char*>(description) : "",
            static_cast<std::time_t>(dueDateInt),
            priority,
            completedInt != 0
        );
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Errore durante l'esecuzione di getAllTasks: "
                  << sqlite3_errmsg(m_db.get()) << std::endl;
    }

    return tasks;
}