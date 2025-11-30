#pragma once
#include <string>
#include <vector>
#include <map>
#include "dictionary.h"
#include "console_stuff.h"


struct Attempt {
    std::string word;
    std::vector<int> colors;
};

class WordleGame {
private:
    std::string secret;
    int attempts;                   
    DictionaryTree dict;
    std::vector<Attempt> attemptsHistory; 

public:
    WordleGame(const std::string& secretWord, const std::vector<std::string>& dictionary);

    void play();

    void printColored(const std::string& guess, int rowY);
};