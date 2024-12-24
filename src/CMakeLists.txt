cmake_minimum_required(VERSION 3.10)
project(ToDoListApp)

set(CMAKE_CXX_STANDARD 17)

find_package(SQLite3 REQUIRED)

include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(ToDoListApp
    main.cpp
    Task.cpp
    TaskManager.cpp
    DatabaseManager.cpp
    CSVBackup.cpp
)

target_link_libraries(ToDoListApp PRIVATE ${SQLite3_LIBRARIES})
