#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// Hàm xử lý pipe
void execute_pipe(const std::vector<std::string>& command1, const std::vector<std::string>& command2) {
    int pipefd[2];

    if (pipe(pipefd) < 0) {
        std::cerr << "Lỗi khi tạo pipe." << std::endl;
        exit(1);
    }

    pid_t pid1 = fork();

    if (pid1 < 0) {
        std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
        exit(1);
    } else if (pid1 == 0) {
        // Tiến trình con 1
        close(pipefd[0]);  // Đóng đầu đọc của pipe

        // Chuyển đầu ra của tiến trình con 1 vào đầu ghi của pipe
        dup2(pipefd[1], STDOUT_FILENO);

        // Thực thi lệnh command1
        std::vector<char*> args;
        for (const auto& arg : command1) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }
        args.push_back(nullptr);
        execvp(args[0], args.data());

        std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
        exit(1);
    }

    pid_t pid2 = fork();

    if (pid2 < 0) {
        std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
        exit(1);
    } else if (pid2 == 0) {
        // Tiến trình con 2
        close(pipefd[1]);  // Đóng đầu ghi của pipe

        // Chuyển đầu vào của tiến trình con 2 từ đầu đọc của pipe
        dup2(pipefd[0], STDIN_FILENO);

        // Thực thi lệnh command2
        std::vector<char*> args;
        for (const auto& arg : command2) {
            args.push_back(const_cast<char*>(arg.c_str()));
        }
        args.push_back(nullptr);
        execvp(args[0], args.data());

        std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
        exit(1);
    }

    close(pipefd[0]);  // Đóng cả hai đầu đọc và ghi của pipe

    // Đợi cho cả hai tiến trình con kết thúc
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
}

// Hàm xử lý redirection >
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

// Hàm xử lý redirection >>
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

// Hàm xử lý redirection 2>
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

// Hàm xử lý redirection <
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
        if (token == "|" || token == ">" || token == ">>" || token == "2>" || token == "<") {
            // Nếu gặp ký tự đặc biệt, lưu command hiện tại vào danh sách commands và chuẩn bị cho command tiếp theo
            if (!command.empty()) {
                commands.push_back(command);
                command.clear();
            }
            command.push_back(token);
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
    std::vector<std::string> input = {"cat", "log.txt", "|", "grep", "Xin chao", ">", "README.txt"};

    std::vector<std::vector<std::string>> commands = split_commands(input);

    // Thực thi các command
    for (const auto& command : commands) {
        if (command.size() == 1) {
            std::string token = command[0];

            if (token == "|") {
                // Xử lý phép pipe
                std::vector<std::string> command1 = commands[0];
                std::vector<std::string> command2 = commands[1];
                execute_pipe(command1, command2);
            } else if (token == ">") {
                // Xử lý redirection >
                std::vector<std::string> command = commands[0];
                std::string output_file = commands[1][0];
                execute_redirection(command, output_file);
            } else if (token == ">>") {
                // Xử lý redirection >>
                std::vector<std::string> command = commands[0];
                std::string output_file = commands[1][0];
                execute_append_redirection(command, output_file);
            } else if (token == "2>") {
                // Xử lý redirection 2>
                std::vector<std::string> command = commands[0];
                std::string error_file = commands[1][0];
                execute_error_redirection(command, error_file);
            } else if (token == "<") {
                // Xử lý redirection <
                std::vector<std::string> command = commands[0];
                std::string input_file = commands[1][0];
                execute_input_redirection(command, input_file);
            }
        }
    }

    return 0;
}