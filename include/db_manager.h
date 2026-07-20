
#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <vector>

class db_manager {
private:
    struct sqlite3_deleter {
        void operator()(sqlite3* db) const {
            sqlite3_close(db);
        }
    };
    std::unique_ptr<sqlite3, sqlite3_deleter> db;

public:
    db_manager();
    bool add_password(const std::string& title, const std::string& password);
    std::vector<std::pair<std::string, std::string>> get_all_passwords() const;
};