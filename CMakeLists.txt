cmake_minimum_required(VERSION 3.10)
project(PasswordManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем ncurses
find_package(PkgConfig REQUIRED)
pkg_check_modules(NCURSES REQUIRED ncurses)

find_package(SQLite3 REQUIRED)

# Заголовки
include_directories(include)

# Исходники
set(SOURCES
    main.cpp
    src/BigInt.cpp
    src/rsa.cpp
    src/db_manager.cpp
)


# Создаем исполняемый файл
add_executable(password_manager ${SOURCES})

# Линкуем ncurses
target_link_libraries(password_manager 
    PRIVATE 
        ${NCURSES_LIBRARIES}
        SQLite::SQLite3
)
target_compile_options(password_manager PRIVATE ${NCURSES_CFLAGS_OTHER})

# Опции компиляции
target_compile_options(password_manager PRIVATE -Wall -Wextra)