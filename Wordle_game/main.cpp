#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <clocale>
#include "invalidWordException.h"
#include "gamelogic.h"
#include "console_stuff.h"

using namespace std;

string gamelogo = R"(
 .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------. 
| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |
| | _____  _____ | || |     ____     | || |  _______     | || |  ________    | || |   _____      | || |  _________   | |
| ||_   _||_   _|| || |   .'    `.   | || | |_   __ \    | || | |_   ___ `.  | || |  |_   _|     | || | |_   ___  |  | |
| |  | | /\ | |  | || |  /  .--.  \  | || |   | |__) |   | || |   | |   `. \ | || |    | |       | || |   | |_  \_|  | |
| |  | |/  \| |  | || |  | |    | |  | || |   |  __ /    | || |   | |    | | | || |    | |   _   | || |   |  _|  _   | |
| |  |   /\   |  | || |  \  `--'  /  | || |  _| |  \ \_  | || |  _| |___.' / | || |   _| |__/ |  | || |  _| |___/ |  | |
| |  |__/  \__|  | || |   `.____.'   | || | |____| |___| | || | |________.'  | || |  |________|  | || | |_________|  | |
| |              | || |              | || |              | || |              | || |              | || |              | |
| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |
 '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------' 
)";

const char* endlogo = R"(
 _____     _ _   
|  ___|   (_) |  
| |____  ___| |_ 
|  __\ \/ / | __|
| |___>  <| | |_ 
\____/_/\_\_|\__|
                 
                 
)";

const char* startlogo = R"(
 _____ _             _                                
/  ___| |           | |                               
\ `--.| |_ __ _ _ __| |_    __ _  __ _ _ __ ___   ___ 
 `--. \ __/ _` | '__| __|  / _` |/ _` | '_ ` _ \ / _ \
/\__/ / || (_| | |  | |_  | (_| | (_| | | | | | |  __/
\____/ \__\__,_|_|   \__|  \__, |\__,_|_| |_| |_|\___|
                            __/ |                     
                           |___/                      
)";


vector<string> loadDictionary(const string& filename) {
    vector<string> words;
    ifstream file(filename);
    string word;
    while (file >> word) {
        if (word.size() == 5) {
            words.push_back(word);
        }
    }
    return words;
}

int main() {
    setlocale(LC_ALL, "ukr");
    try {
        while (true) {
            cout << gamelogo << endl;
            const char* menuItems[] = { startlogo, endlogo };
            int choice = ShowMenu(menuItems, 2, 20, 12);

            if (choice == 0) {
                vector<string> dict = loadDictionary("dictionary.txt");
                if (dict.empty()) {
                    cout << "Dictionary file is empty or missing!\n";
                    return 1;
                }
                srand((unsigned)time(nullptr));
                string secret = dict[rand() % dict.size()];

                WordleGame game(secret, dict);
                game.play();
                ClearConsole();
            }
            else if (choice == 1) {
                cout << "Goodbye!\n";
                break;
            }
        }
    }
    catch (const InvalidWordException& e) {
        cout << "Error: " << e.what() << "\n";
    }
    return 0;
}
