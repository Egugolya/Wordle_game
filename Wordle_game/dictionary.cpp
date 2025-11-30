#include "dictionary.h"
#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

std::vector<std::wstring> loadDictionary(const std::wstring& filename) {
    std::vector<std::wstring> words;
    std::wifstream file(filename);
    file.imbue(std::locale("uk_UA.UTF-8"));

    std::wstring word;
    while (file >> word) {
        if (word.size() == 5) {
            words.push_back(word);
        }
    }
    return words;
}
DictionaryTree::DictionaryTree() {
    root = new Node();
}

void DictionaryTree::insert(const std::string& word) {
    Node* current = root;
    for (char c : word) {
        if (current->children[c] == nullptr) {
            current->children[c] = new Node();
        }
        current = current->children[c];
    }
    current->isWord = true;
}

bool DictionaryTree::contains(const std::string& word) const {
    const Node* current = root;
    for (char c : word) {
        auto it = current->children.find(c);
        if (it == current->children.end()) return false;
        current = it->second;
    }
    return current->isWord;
}