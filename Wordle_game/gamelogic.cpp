#include "gamelogic.h"
#include "console_stuff.h"
#include "invalidWordException.h"
#include <iostream>
#include <map>

#include <windows.h>
#include <iostream>
using namespace std;

void printInstructions() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdOut, &csbi);

    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int startX = consoleWidth - 70;
    int startY = consoleHeight - 10;

    SetCursorPosition(startX, startY);
    cout << "Game rules:";
    SetCursorPosition(startX, startY + 1);
    cout << "- Goal: guess a 5-letter English word.";
    SetCursorPosition(startX, startY + 2);
    cout << "- Number of attempts: 6 words.";
    SetCursorPosition(startX, startY + 3);
    cout << "- Input: each attempt must be a real English word.";
    SetCursorPosition(startX, startY + 4);
    cout << "- Colour hints:";

    SetCursorPosition(startX + 4, startY + 5);
    cout << "> ";
    SetColor(BLACK, GREEN);
    cout << "green";
    SetColor(WHITE, BLACK);
    cout << " — the letter is in the correct position.";

    SetCursorPosition(startX + 4, startY + 6);
    cout << "> ";
    SetColor(BLACK, YELLOW);
    cout << "yellow";
    SetColor(WHITE, BLACK);
    cout << " — the letter is in the word but in a different position.";

    SetCursorPosition(startX + 4, startY + 7);
    cout << "> ";
    SetColor(BLACK, WHITE);
    cout << "gray";
    SetColor(WHITE, BLACK);
    cout << " — the letter is not in the word.";
}

const char* youwon = R"(
__   __                                
\ \ / /                                
 \ V /___  _   _  __      _____  _ __  
  \ // _ \| | | | \ \ /\ / / _ \| '_ \ 
  | | (_) | |_| |  \ V  V / (_) | | | |
  \_/\___/ \__,_|   \_/\_/ \___/|_| |_|
                                       
                                             
)";

const char* gameover = R"(
 _____                                            
|  __ \                                           
| |  \/ __ _ _ __ ___   ___    _____   _____ _ __ 
| | __ / _` | '_ ` _ \ / _ \  / _ \ \ / / _ \ '__|
| |_\ \ (_| | | | | | |  __/ | (_) \ V /  __/ |   
 \____/\__,_|_| |_| |_|\___|  \___/ \_/ \___|_|   
                                                  
                                                                
)";
const char* playagain = R"(
______ _                                  _       
| ___ \ |                                (_)      
| |_/ / | __ _ _   _     __ _  __ _  __ _ _ _ __  
|  __/| |/ _` | | | |   / _` |/ _` |/ _` | | '_ \ 
| |   | | (_| | |_| |  | (_| | (_| | (_| | | | | |
\_|   |_|\__,_|\__, |   \__,_|\__, |\__,_|_|_| |_|
                __/ |          __/ |              
               |___/          |___/               
)";

const char* returntomenu = R"(
______     _                      _                                     
| ___ \   | |                    | |                                    
| |_/ /___| |_ _   _ _ __ _ __   | |_ ___    _ __ ___   ___ _ __  _   _ 
|    // _ \ __| | | | '__| '_ \  | __/ _ \  | '_ ` _ \ / _ \ '_ \| | | |
| |\ \  __/ |_| |_| | |  | | | | | || (_) | | | | | | |  __/ | | | |_| |
\_| \_\___|\__|\__,_|_|  |_| |_|  \__\___/  |_| |_| |_|\___|_| |_|\__,_|
                                                                        
                                                                                      
)";
using namespace std;

WordleGame::WordleGame(const string& secretWord, const vector<string>& dictionary)
    : secret(secretWord), attempts(0) {
    for (const auto& w : dictionary) {
        dict.insert(w);
    }
    attemptsHistory.clear();
}

void WordleGame::printColored(const string& guess, int rowY) {
    map<char, int> freq;
    for (char c : secret) freq[c]++;

    Attempt attempt;
    attempt.word = guess;
    attempt.colors.assign(guess.size(), WHITE);

    for (size_t i = 0; i < guess.size(); ++i) {
        if (guess[i] == secret[i]) {
            attempt.colors[i] = GREEN;
            freq[guess[i]]--;
        }
    }

    for (size_t i = 0; i < guess.size(); ++i) {
        if (attempt.colors[i] == WHITE && freq[guess[i]] > 0) {
            attempt.colors[i] = YELLOW;
            freq[guess[i]]--;
        }
    }

    attemptsHistory.push_back(attempt);

    for (size_t row = 0; row < attemptsHistory.size(); ++row) {
        const Attempt& a = attemptsHistory[row];
        for (size_t i = 0; i < a.word.size(); ++i) {
            SetCursorPosition(5 + int(i) * 3, 4 + int(row));
            SetColor(BLACK, a.colors[i]);
            cout << a.word[i];
            SetColor(WHITE, BLACK);
        }
    }
}

void WordleGame::play() {
    ClearConsole();
    SetCursorPosition(50, 50);
    printInstructions();
    attempts = 0;
    while (attempts < 6) {
        try {
            SetCursorPosition(50, 2);
            cout << "Attempts left: " << (6 - attempts) << "   ";

            string guess;
            SetCursorPosition(0, 2 + attempts);
            cout << "Attempt #" << (attempts + 1) << ": ";
            cin >> guess;

            if (guess.size() != 5)
                throw InvalidWordException("Word must be 5 letters!");

            if (!dict.contains(guess))
                throw InvalidWordException("Word not in dictionary!");

            map<char, int> freq;
            for (char c : secret) freq[c]++;

            vector<int> colors(5, WHITE);
            for (int i = 0; i < 5; ++i) {
                if (guess[i] == secret[i]) {
                    colors[i] = GREEN;
                    freq[guess[i]]--;
                }
            }
            for (int i = 0; i < 5; ++i) {
                if (colors[i] == WHITE && freq[guess[i]] > 0) {
                    colors[i] = YELLOW;
                    freq[guess[i]]--;
                }
            }

            for (int i = 0; i < 5; ++i) {
                SetCursorPosition(12 + i, 2 + attempts);
                SetColor(BLACK, colors[i]);
                cout << guess[i];
                SetColor(WHITE, BLACK);
            }

            Attempt attempt;
            attempt.word = guess;
            attempt.colors = colors;
            attemptsHistory.push_back(attempt);
            attempts++;

            if (guess == secret) {
                ClearConsole();
                cout << youwon << endl;
                SetCursorPosition(5, 9);
                cout << "You guessed the word!\n";
                const char* menuItems[] = { playagain , returntomenu };
                int choice = ShowMenu(menuItems, 2, 10, 10);
                if (choice == 0) {
                    ClearConsole();
                    play();
                }
                else if (choice == 1) {
                    ClearConsole();
                    return;
                }
            }
        }
        catch (const InvalidWordException& e) {
            SetCursorPosition(0, 12);
            cout << "Error: " << e.what() << " Try again.\n";
        }
    }
    ClearConsole();
    cout << gameover << endl;
    SetCursorPosition(5, 9);
    cout << "Out of attempts! The word was: " << secret << "\n";
    const char* menuItems[] = { playagain , returntomenu };
    int choice = ShowMenu(menuItems, 2, 10, 10);
    if (choice == 0) {
        ClearConsole();
        play();
    }
    else if (choice == 1) {
        ClearConsole();
        return;
    }
}