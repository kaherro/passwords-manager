#include <iostream> 
#include <string>
#include <functional>
#include <vector>
#include <ncurses.h>
#include <rsa.h>
#include <db_manager.h>
using namespace std;

BigInt e, d, n; 
db_manager db; 

struct menu_item {
    std::string title;
    std::function<void()> action = nullptr;  
};

void show_menu(const std::string menu_title, const vector<menu_item> &items) {
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

void generate_key() {
    clear(); 
    printw("Generating public key..."); 
    refresh(); 
    std::string msg = "Your new public key is: " + rsa::generate_keys(e, d, n).to_string(); 
    clear(); 
    printw("%s\n", msg.c_str());
    attron(COLOR_PAIR(2)); 
    printw("Warning: Make sure you saved the key, otherwise you won't be able to get the correct passwords.\n\n"); 
    attroff(COLOR_PAIR(2)); 
    printw("Press any key to return to menu...");
    refresh();
    getch(); 
}

BigInt read_public_key() {
    clear();
    std::string key;
    while(true) {
        printw("Enter your public key (make sure to type it correctly, otherwise you won't be able to get the correct passwords)\n");
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
                printw("Public key format is incorrect. Only numbers are able. Try again.\n"); 
                refresh(); 
                break; 
            }
            key += c; 
        }
        if(key.length() > 50) {
            key.clear(); 
            clear();
            printw("Public key format is incorrect. Only numbers are able. Try again.\n"); 
            tr = false; 
        }
        if(tr) break;
    }
    return BigInt(key); 
}

void add_password() {
    BigInt public_key = read_public_key(); 
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
    rsa::use_existing_public_key(public_key, e, d, n); 
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

void show_passwords() {
    BigInt public_key = read_public_key(); 
    rsa::use_existing_public_key(public_key, e, d, n); 
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
    cout << characters.size();  
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
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0); 
    
    start_color(); 
    init_pair(1, COLOR_CYAN, COLOR_WHITE); 
    init_pair(2, COLOR_RED, COLOR_BLACK); 

    std::vector<menu_item> main_menu_options = {
        {"Check my passwords", show_passwords}, 
        {"Generate a new key", generate_key}, 
        {"Generate a new password", generate_password}, 
        {"Add your own password", add_password}, 
        {"Exit password manager", [](){ endwin(); exit(0); }}
    };

    show_menu("PASSWORD MANAGER", main_menu_options); 

    endwin();
    return 0;
}
