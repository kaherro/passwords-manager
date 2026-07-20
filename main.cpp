#include <iostream> 
#include <string>
#include <functional>
#include <vector>
#include <ncurses.h>
#include <rsa.h>
#include <db_manager.h>
using namespace std;

struct menu_item {
    std::string title;
    std::function<void()> action = nullptr;  
};

void show_menu(const std::string &menu_title, const vector<menu_item> &items) {
    int cur_option = 0, menu_size = items.size(); 
    while(true) {
        clear();
        for(int i = 0; i < items.size(); i++) {
            if(cur_option == i) {
                attron(COLOR_PAIR(1)); 
                printw("> "); 
            }
            else {
                printw("  ");
            }
            printw("%s\n", items[i].title.c_str()); 
            if(cur_option == i) {
                attroff(COLOR_PAIR(1)); 
            }
        }
        refresh(); 
        int button = getch(); 
        if(button == KEY_DOWN) {
            cur_option = (cur_option + 1) % menu_size; 
        }
        if(button == KEY_UP) {
            cur_option = (cur_option + menu_size - 1) % menu_size;  
        }
        if(button == KEY_RIGHT || button == KEY_ENTER || button == 10) {
            if(items[cur_option].action) {
                items[cur_option].action(); 
            }
        }
    }
}

void generate_key(BigInt &e, BigInt &d, BigInt &n, db_manager &db) {
    clear(); 
    printw("Generating a new RSA key pair..."); 
    refresh(); 
    rsa::generate_keys(e, d, n);
    db.save_public_key(e.to_string(), n.to_string());
    clear(); 
    std::string msg = "Your PRIVATE key is: " + d.to_string();
    printw("%s\n", msg.c_str());
    attron(COLOR_PAIR(2)); 
    printw("Warning: this is your PRIVATE key. Save it somewhere safe (e.g. a physical notebook).\n");
    printw("Anyone who has it can read all your saved passwords. It is never stored on disk.\n");
    printw("If you lose it, your saved passwords cannot be recovered.\n\n");
    attroff(COLOR_PAIR(2)); 
    printw("Press any key to return to menu...");
    refresh();
    getch(); 
}

BigInt read_private_key() {
    clear();
    std::string key;
    while(true) {
        printw("Enter your private key to continue\n");
        refresh();
        char buf[256];
        echo(); 
        getstr(buf); 
        noecho();
        bool tr = true; 
        for(auto c : buf) {
            if(c == 0) break; 
            if(!(c - '0' >= 0 && c - '0' <= 9)) {
                tr = false; 
                clear();
                printw("Key format is incorrect. Only numbers are allowed. Try again.\n"); 
                refresh(); 
                break; 
            }
            key += c; 
        }
        if(key.length() > 100) {
            key.clear(); 
            clear();
            printw("Key format is incorrect. Only numbers are allowed. Try again.\n");
            tr = false; 
        }
        if(tr) break;
    }
    return BigInt(key); 
}

void add_password(db_manager &db) {
    auto stored_key = db.load_public_key();
    if (!stored_key) {
        clear();
        printw("No key found yet. Please generate a key first.\n");
        printw("Press any key to return to menu...");
        getch();
        return;
    }
    BigInt e(stored_key->first);
    BigInt n(stored_key->second);
    clear();
    std::string title; 
    while(true) {
        printw("Enter your title (length must be <= 15, latin letters, numbers and special symbols only available)\n");
        refresh();
        char buf[256];
        echo(); 
        getstr(buf); 
        noecho();
        bool tr = true; 
        for(auto c : buf) {
            if(c == 0) break; 
            if(!(c > 0 && c <= 127)) {
                tr = false; 
                clear();
                printw("Title format is incorrect. Try again.\n"); 
                refresh(); 
                break; 
            }
            title += c; 
        }
        if(title.length() > 15) {
            title.clear(); 
            clear();
            printw("Title format is incorrect. Try again.\n"); 
            tr = false; 
        }
        if(tr) break;
    }
    std::string password;
    while(true) {
        printw("Enter your password (length must be <= 20, latin letters, numbers and special symbols only available)\n");
        refresh();
        char buf[256];
        echo(); 
        getstr(buf); 
        noecho();
        bool tr = true; 
        for(auto c : buf) {
            if(c == 0) break; 
            if(!(c > 0 && c <= 127)) {
                tr = false; 
                clear();
                printw("Password format is incorrect. Try again.\n"); 
                refresh(); 
                break; 
            }
            password += c; 
        }
        if(password.length() > 20) {
            password.clear(); 
            clear();
            printw("Password format is incorrect. Try again.\n"); 
            tr = false; 
        }
        if(tr) break;
    }
    refresh();
    BigInt enc = rsa::encrypt(password, e, n); 
    bool result = db.add_password(title, enc.to_string()); 
    if(result) {
        clear(); 
        printw("Error occured while adding your password in table. Try again.\n"); 
    }
    else {
        clear();
        std::string answer = "Password for '" + title + "' added successfully."; 
        printw("%s\n", answer.c_str()); 
    } 
    getch();
}

void show_passwords(db_manager &db) {
    auto stored_key = db.load_public_key();
    if (!stored_key) {
        clear();
        printw("No key found yet. Please generate a key first.\n");
        printw("Press any key to return to menu...");
        getch();
        return;
    }
    BigInt d = read_private_key();
    BigInt n(stored_key->second);
    std::vector<std::pair<std::string, std::string>> passwords = db.get_all_passwords(); 
    clear(); 
    printw("================================\n");
    printw("         Passwords list: \n");
    printw("================================\n");
    for(auto [title, enc] : passwords) {
        std::string password = rsa::decrypt(enc, d, n); 
        while(title.length() < 15) {
            title += " "; 
        }
        std::string s = title + " : " + password; 
        printw("%s\n", s.c_str()); 
    }
    printw("\n"); 
    printw("Press any key to return to menu..."); 
    getch(); 
}

void generate_password() {
    clear();
    printw("Generating your password..."); 
    std::string characters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"
                            "1234567890-=`~!@#$%^&*()_+[]{};:'/?.>,<" + '"'; 
    std::string password = "";
    srand(time(0));
    for(int i = 0; i < 20; i++) {
        password += characters[rand() % 58]; 
    }
    std::string answer = "Your generated password: " + password;
    clear();
    printw("%s\n", answer.c_str());
    printw("\n"); 
    printw("Press any key to return to menu..."); 
    getch(); 
}

int main() {
    BigInt e, d, n; 
    db_manager db; 

    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0); 
    
    start_color(); 
    init_pair(1, COLOR_CYAN, COLOR_WHITE); 
    init_pair(2, COLOR_RED, COLOR_BLACK); 

    std::vector<menu_item> main_menu_options = {
        {"Check my passwords", [&](){ show_passwords(db); }}, 
        {"Generate a new key", [&](){ generate_key(e, d, n, db); }}, 
        {"Generate a new password", [&](){ generate_password(); }}, 
        {"Add your own password", [&](){ add_password(db); }}, 
        {"Exit password manager", [](){ endwin(); exit(0); }}
    };

    show_menu("PASSWORD MANAGER", main_menu_options); 

    endwin();
    return 0;
}