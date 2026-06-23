#include <db_manager.h>

db_manager::db_manager() {    
    if (sqlite3_open("passwords_manager.db", &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    std::string create_table = "CREATE TABLE IF NOT EXISTS passwords ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "title TEXT NOT NULL UNIQUE, "
                            "password TEXT NOT NULL);";
    char* errorMsg = nullptr;
    if (sqlite3_exec(db, create_table.c_str(), nullptr, nullptr, &errorMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
    }
}

db_manager::~db_manager() {
    sqlite3_close(db);
}

bool db_manager::add_password(const std::string title, const std::string password) {
    if(title.empty() || title.size() > 15) {
        std::cerr << "Wrong title format."; 
    }
    if(title.empty() || title.size() > 50) {
        std::cerr << "Wrong password format."; 
    }
    std::string add_password = "INSERT OR REPLACE INTO passwords (title, password) VALUES ('" 
                                + title + "', '" + password + "');"; 
    char* errorMsg = nullptr;
    if (sqlite3_exec(db, add_password.c_str(), nullptr, nullptr, &errorMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
        return true; 
    }
    return false; 
}

std::vector<std::pair<std::string, std::string>> db_manager::get_all_passwords() {
    std::vector<std::pair<std::string, std::string>> passwords;
    const char* get_passwords = "SELECT title, password FROM passwords ORDER BY title;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, get_passwords, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to get passwords: " << sqlite3_errmsg(db) << std::endl;
        return passwords;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const std::string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        passwords.push_back({title, password});
    }
    sqlite3_finalize(stmt);
    return passwords;
}
