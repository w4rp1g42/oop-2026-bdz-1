#include "utils.hpp"

#include <iostream>

size_t input_size_t() {
    std::string line;
    while (std::getline(std::cin, line)) {
        try {
            size_t pos = 0;
            const long long value = std::stoll(line, &pos);
            if (pos == line.size() && value >= 0) return static_cast<size_t>(value);
        } catch (...) {
        }
        std::cout << "Invalid number input\n";
    }
    throw QuitSignal{};
}

std::string input_line(const std::string &prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) throw QuitSignal{};
    return line;
}
