cmake_minimum_required(VERSION 3.10)
project(PasswordManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NCURSES REQUIRED ncurses)

find_package(SQLite3 REQUIRED)

include_directories(include)

set(SOURCES
    main.cpp
    src/BigInt.cpp
    src/rsa.cpp
    src/db_manager.cpp
)


add_executable(password_manager ${SOURCES})

target_link_libraries(password_manager 
    PRIVATE 
        ${NCURSES_LIBRARIES}
        SQLite::SQLite3
)
target_compile_options(password_manager PRIVATE ${NCURSES_CFLAGS_OTHER})
