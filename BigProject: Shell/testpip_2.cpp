#include <iostream>
#include <string>
#include <cstdio>

std::string execute_command(const std::string& command) {
    std::string result;
    FILE* pipe = popen((command + " 2>/dev/null").c_str(), "r");
    if (pipe) {
        constexpr int buffer_size = 128;
        char buffer[buffer_size];

        while (fgets(buffer, buffer_size, pipe) != nullptr) {
            result += buffer;
        }

        pclose(pipe);
    }

    return result;
}

int main() {
    std::string command = "ls | grep a ";

    std::string output = execute_command(command);
    // output = output.substr(0, output.find_last_not_of("\n") + 1); // Remove trailing newline

    std::cout << "Output: " << output << std::endl;

    return 0;
}
