#include <db_manager.h>

db_manager::db_manager() {    
    sqlite3* raw_db = nullptr;
    if (sqlite3_open("passwords_manager.db", &raw_db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg( raw_db) << std::endl;
        sqlite3_close(raw_db); 
        return;
    }
    db.reset(raw_db);
    std::string create_table = "CREATE TABLE IF NOT EXISTS passwords ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "title TEXT NOT NULL UNIQUE, "
                            "password TEXT NOT NULL);";
    char* errorMsg = nullptr;
    if (sqlite3_exec(db.get(), create_table.c_str(), nullptr, nullptr, &errorMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
    }

    std::string create_key_table = "CREATE TABLE IF NOT EXISTS rsa_public_key ("
                            "id INTEGER PRIMARY KEY CHECK (id = 1), "
                            "e TEXT NOT NULL, "
                            "n TEXT NOT NULL);";
    if (sqlite3_exec(db.get(), create_key_table.c_str(), nullptr, nullptr, &errorMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
    }
}

bool db_manager::save_public_key(const std::string &e, const std::string &n) {
    const char* sql = "INSERT OR REPLACE INTO rsa_public_key (id, e, n) VALUES (1, ?, ?);";
    sqlite3_stmt* raw_stmt = nullptr;
    if (sqlite3_prepare_v2(db.get(), sql, -1, &raw_stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to save public key: " << sqlite3_errmsg(db.get()) << std::endl;
        return false;
    }
    auto stmt_deleter = [](sqlite3_stmt* s) { sqlite3_finalize(s); };
    std::unique_ptr<sqlite3_stmt, decltype(stmt_deleter)> stmt(raw_stmt, stmt_deleter);

    sqlite3_bind_text(stmt.get(), 1, e.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 2, n.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt.get()) != SQLITE_DONE) {
        std::cerr << "Failed to save public key: " << sqlite3_errmsg(db.get()) << std::endl;
        return false;
    }
    return true;
}

std::optional<std::pair<std::string, std::string>> db_manager::load_public_key() const {
    const char* sql = "SELECT e, n FROM rsa_public_key WHERE id = 1;";
    sqlite3_stmt* raw_stmt = nullptr;
    if (sqlite3_prepare_v2(db.get(), sql, -1, &raw_stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to load public key: " << sqlite3_errmsg(db.get()) << std::endl;
        return std::nullopt;
    }
    auto stmt_deleter = [](sqlite3_stmt* s) { sqlite3_finalize(s); };
    std::unique_ptr<sqlite3_stmt, decltype(stmt_deleter)> stmt(raw_stmt, stmt_deleter);

    if (sqlite3_step(stmt.get()) != SQLITE_ROW) {
        return std::nullopt;
    }
    std::string e = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 0));
    std::string n = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
    return std::make_pair(e, n);
}

bool db_manager::add_password(const std::string& title, const std::string& password) {
    if(title.empty() || title.size() > 15) {
        std::cerr << "Wrong title format."; 
        return true; 
    }
    std::string add_password = "INSERT OR REPLACE INTO passwords (title, password) VALUES ('" 
                                + title + "', '" + password + "');"; 
    char* errorMsg = nullptr;
    if (sqlite3_exec(db.get(), add_password.c_str(), nullptr, nullptr, &errorMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errorMsg << std::endl;
        sqlite3_free(errorMsg);
        return true; 
    }
    return false; 
}

std::vector<std::pair<std::string, std::string>> db_manager::get_all_passwords() const {
    std::vector<std::pair<std::string, std::string>> passwords;
    const char* get_passwords = "SELECT title, password FROM passwords ORDER BY title;";
    sqlite3_stmt* raw_stmt = nullptr;
    auto stmt_deleter = [](sqlite3_stmt* s) { sqlite3_finalize(s); };
    if (sqlite3_prepare_v2(db.get(), get_passwords, -1, &raw_stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to get passwords: " << sqlite3_errmsg(db.get()) << std::endl;
        return passwords;
    }
    std::unique_ptr<sqlite3_stmt, decltype(stmt_deleter)> stmt(raw_stmt, stmt_deleter);

    while (sqlite3_step(stmt.get()) == SQLITE_ROW) {
        const std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 0));
        const std::string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), 1));
        passwords.push_back({title, password});
    }
    return passwords;
}