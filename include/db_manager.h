
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>
#include <vector>

class db_manager {
private: 
    sqlite3 *db;
public:
    db_manager();
    bool add_password(const std::string title, const std::string password);
    std::vector<std::pair<std::string, std::string>> get_all_passwords();
    ~db_manager(); 
};