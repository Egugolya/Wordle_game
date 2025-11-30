#pragma once
#include <string>

class InvalidWordException {
    std::string message;
public:
    InvalidWordException(const std::string& msg) : message(msg) {}
    std::string what() const { return message; }
};