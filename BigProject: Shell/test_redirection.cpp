// #include <iostream>
// #include <unistd.h>
// #include <fcntl.h>

// int main() {
//     int inputFileDescriptor = open("test1.txt", O_RDONLY); // Open the input file

//     if (inputFileDescriptor == -1) {
//         std::cerr << "Failed to open input file." << std::endl;
//         return 1;
//     }

//     // Redirect the input file descriptor to stdin
//     if (dup2(inputFileDescriptor, STDIN_FILENO) == -1) {
//         std::cerr << "Failed to redirect input." << std::endl;
//         close(inputFileDescriptor);
//         return 1;
//     }

//     close(inputFileDescriptor); // Close the input file descriptor

//     // Read from stdin
//     std::string userInput;
//     std::getline(std::cin, userInput);

//     // Process user input
//     std::cout << "User input: " << userInput << std::endl;

//     return 0;
// }
#include <iostream>
#include <fstream>
#include <string>

void execute_command(const std::string& command) {
    // Execute the command
    std::cout << "Executing command: " << command << std::endl;
    // Add your code here to handle the execution of the command
    // ...
}

void handle_redirection(const std::string& command, const std::string& filename) {
    std::ifstream inputFile(filename); // Open the input file

    if (!inputFile) {
        std::cerr << "Failed to open input file: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string inputText;

    while (std::getline(inputFile, line)) {
        inputText += line + "\n";
    }

    inputFile.close(); // Close the input file

    // Redirect inputText to the command
    execute_command(command + " <<< " + "\"" + inputText + "\"");
}

int main() {
    std::string command = "grep 'Hello'";

    std::string filename = "hieu.txt";

    handle_redirection(command, filename);

    return 0;
}
