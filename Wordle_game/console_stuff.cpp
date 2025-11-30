#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include "console_stuff.h"
#include <clocale>

using namespace std;



void SetColor(int text, int background = BLACK) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void SetCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ClearConsole() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD written;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, { 0, 0 }, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, consoleSize, { 0, 0 }, &written);
    SetConsoleCursorPosition(hConsole, { 0, 0 });
}

int ShowMenu(const char* items[], int itemCount, int startX, int startY) {
    ShowConsoleCursor(false);
    int active = 0;
    while (true) {
        for (int i = 0; i < itemCount; ++i) {
            SetCursorPosition(startX, startY + i);
            if (i == active)
                SetColor(BLACK, WHITE);
            else
                SetColor(WHITE, BLACK);
            cout << items[i] << " ";
        }

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72)
                active = (active - 1 + itemCount) % itemCount;
            else if (key == 80)
                active = (active + 1) % itemCount;
        }
        else if (key == 13) {
            ShowConsoleCursor(true);
            SetColor(WHITE, BLACK);
            return active;
        }
        else if (key == 27) {
            ShowConsoleCursor(true);
            SetColor(WHITE, BLACK);
            return -1;
        }
    }
}