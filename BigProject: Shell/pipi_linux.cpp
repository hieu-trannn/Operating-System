#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// Hàm xử lý lệnh với redirection >
void execute_redirection(const std::vector<std::string>& command, const std::string& output_file) {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
        exit(1);
    } else if (pid == 0) {
        // Tiến trình con

        // Mở tập tin đầu ra và chuyển đầu ra của tiến trình con vào tập tin
        int output_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) {
            std::cerr << "Lỗi khi mở tập tin đầu ra." << std::endl;
            exit(1);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);

        // Thực thi lệnh command
        std::vector<char*> args;
        for (const auto& arg : command) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }
        args.push_back(nullptr);
        execvp(args[0], args.data());

        std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
        exit(1);
    }

    // Tiến trình cha
    waitpid(pid, nullptr, 0);
}

// Hàm xử lý lệnh với redirection >>
void execute_append_redirection(const std::vector<std::string>& command, const std::string& output_file) {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
        exit(1);
    } else if (pid == 0) {
        // Tiến trình con

        // Mở tập tin đầu ra và chuyển đầu ra của tiến trình con vào tập tin (nối thêm)
        int output_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (output_fd < 0) {
            std::cerr << "Lỗi khi mở tập tin đầu ra." << std::endl;
            exit(1);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);

        // Thực thi lệnh command
        std::vector<char*> args;
        for (const auto& arg : command) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }
        args.push_back(nullptr);
        execvp(args[0], args.data());

        std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
        exit(1);
    }

    // Tiến trình cha
    waitpid(pid, nullptr, 0);
}

// Hàm xử lý lệnh với redirection 2>
void execute_error_redirection(const std::vector<std::string>& command, const std::string& error_file) {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
        exit(1);
    } else if (pid == 0) {
        // Tiến trình con

        // Mở tập tin lỗi đầu ra và chuyển đầu ra lỗi của tiến trình con vào tập tin
        int error_fd = open(error_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (error_fd < 0) {
            std::cerr << "Lỗi khi mở tập tin lỗi đầu ra." << std::endl;
            exit(1);
        }
        dup2(error_fd, STDERR_FILENO);
        close(error_fd);

        // Thực thi lệnh command
        std::vector<char*> args;
        for (const auto& arg : command) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }
        args.push_back(nullptr);
        execvp(args[0], args.data());

        std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
        exit(1);
    }

    // Tiến trình cha
    waitpid(pid, nullptr, 0);
}

// Hàm xử lý lệnh với redirection <
void execute_input_redirection(const std::vector<std::string>& command, const std::string& input_file) {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
        exit(1);
    } else if (pid == 0) {
        // Tiến trình con

        // Mở tập tin đầu vào và chuyển đầu vào của tiến trình con từ tập tin
        int input_fd = open(input_file.c_str(), O_RDONLY);
        if (input_fd < 0) {
            std::cerr << "Lỗi khi mở tập tin đầu vào." << std::endl;
            exit(1);
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);

        // Thực thi lệnh command
        std::vector<char*> args;
        for (const auto& arg : command) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }
        args.push_back(nullptr);
        execvp(args[0], args.data());

        std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
        exit(1);
    }

    // Tiến trình cha
    waitpid(pid, nullptr, 0);
}

// Hàm tách lệnh thành các command riêng biệt
std::vector<std::vector<std::string>> split_commands(const std::vector<std::string>& input) {
    std::vector<std::vector<std::string>> commands;
    std::vector<std::string> command;

    for (const std::string& token : input) {
        if (token == "," || token == ">" || token == ">>" || token == "2>" || token == "<") {
            // Nếu gặp ký tự phân tách lệnh, lưu command hiện tại vào danh sách commands và chuẩn bị cho command tiếp theo
            if (!command.empty()) {
                commands.push_back(command);
                command.clear();
            }
        } else {
            // Thêm từng token vào command hiện tại
            command.push_back(token);
        }
    }

    if (!command.empty()) {
        // Lưu command cuối cùng vào danh sách commands
        commands.push_back(command);
    }

    return commands;
}

int main() {
    std::vector<std::string> input = {"ls", ">", "output.txt", ",", "grep", "Xin chao", ">>", "log.txt"};

    std::vector<std::vector<std::string>> commands = split_commands(input);

    for (const auto& command : commands) {
        if (command.size() >= 3) {
            if (command[1] == ">") {
                execute_redirection(std::vector<std::string>(command.begin(), command.begin() + 1),
                                    command[2]);
            } else if (command[1] == ">>") {
                execute_append_redirection(std::vector<std::string>(command.begin(), command.begin() + 1),
                                            command[2]);
            } else if (command[1] == "2>") {
                execute_error_redirection(std::vector<std::string>(command.begin(), command.begin() + 1),
                                            command[2]);
            } else if (command[1] == "<") {
                execute_input_redirection(std::vector<std::string>(command.begin(), command.begin() + 1),
                                            command[2]);
            }
        }
    }

    return 0;
}