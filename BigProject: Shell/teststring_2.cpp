#include <iostream>
#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

int main() {
    std::string input = "Hello  World How  Are You?  ";
    char delimiter = ' ';

    std::vector<std::string> tokens = splitString(input, delimiter);

    // Print the tokens
    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }

    return 0;
}
