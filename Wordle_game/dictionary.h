#pragma once
#include <string>
#include <map>

struct Node {
    std::map<char, Node*> children;
    bool isWord;
    Node() : isWord(false) {}
};

class DictionaryTree {
    Node* root;
public:
    DictionaryTree();
    void insert(const std::string& word);
    bool contains(const std::string& word) const;
};