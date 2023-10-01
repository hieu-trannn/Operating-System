// #include <iostream>
// #include <vector>
// #include <string>
// #include <sstream>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <fcntl.h>

// // Hàm xử lý pipe
// void execute_pipe(const std::vector<std::string>& command1, const std::vector<std::string>& command2) {
//     int pipefd[2];

//     if (pipe(pipefd) < 0) {
//         std::cerr << "Lỗi khi tạo pipe." << std::endl;
//         exit(1);
//     }

//     pid_t pid1 = fork();

//     if (pid1 < 0) {
//         std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
//         exit(1);
//     } else if (pid1 == 0) {
//         // Tiến trình con 1
//         close(pipefd[0]);  // Đóng đầu đọc của pipe

//         // Chuyển đầu ra của tiến trình con 1 vào đầu ghi của pipe
//         dup2(pipefd[1], STDOUT_FILENO);

//         // Thực thi lệnh command1
//         std::vector<char*> args;
//         for (const auto& arg : command1) {
//             args.push_back(const_cast<char*>(arg.c_str()));
//         }
//         args.push_back(nullptr);
//         execvp(args[0], args.data());

//         std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
//         exit(1);
//     }

//     pid_t pid2 = fork();

//     if (pid2 < 0) {
//         std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
//         exit(1);
//     } else if (pid2 == 0) {
//         // Tiến trình con 2
//         close(pipefd[1]);  // Đóng đầu ghi của pipe

//         // Chuyển đầu vào của tiến trình con 2 từ đầu đọc của pipe
//         dup2(pipefd[0], STDIN_FILENO);

//         // Thực thi lệnh command2
//         std::vector<char*> args;
//         for (const auto& arg : command2) {
//             args.push_back(const_cast<char*>(arg.c_str()));
//         }
//         args.push_back(nullptr);
//         execvp(args[0], args.data());

//         std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
//         exit(1);
//     }

//     close(pipefd[0]);  // Đóng cả hai đầu đọc và ghi của pipe

//     // Đợi cho cả hai tiến trình con kết thúc
//     waitpid(pid1, nullptr, 0);
//     waitpid(pid2, nullptr, 0);
// }

// // Hàm xử lý redirection >
// void execute_redirection(const std::vector<std::string>& command, const std::string& output_file) {
//     pid_t pid = fork();

//     if (pid < 0) {
//         std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
//         exit(1);
//     } else if (pid == 0) {
//         // Tiến trình con

//         // Mở tập tin đầu ra và chuyển đầu ra của tiến trình con vào tập tin
//         int output_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (output_fd < 0) {
//             std::cerr << "Lỗi khi mở tập tin đầu ra." << std::endl;
//             exit(1);
//         }
//         dup2(output_fd, STDOUT_FILENO);
//         close(output_fd);

//         // Thực thi lệnh command
//         std::vector<char*> args;
//         for (const auto& arg : command) {
//             args.push_back(const_cast<char*>(arg.c_str()));
//         }
//         args.push_back(nullptr);
//         execvp(args[0], args.data());

//         std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
//         exit(1);
//     }

//     // Tiến trình cha
//     waitpid(pid, nullptr, 0);
// }

// // Hàm xử lý redirection >>
// void execute_append_redirection(const std::vector<std::string>& command, const std::string& output_file) {
//     pid_t pid = fork();

//     if (pid < 0) {
//         std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
//         exit(1);
//     } else if (pid == 0) {
//         // Tiến trình con

//         // Mở tập tin đầu ra và chuyển đầu ra của tiến trình con vào tập tin (nối thêm)
//         int output_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
//         if (output_fd < 0) {
//             std::cerr << "Lỗi khi mở tập tin đầu ra." << std::endl;
//             exit(1);
//         }
//         dup2(output_fd, STDOUT_FILENO);
//         close(output_fd);

//         // Thực thi lệnh command
//         std::vector<char*> args;
//         for (const auto& arg : command) {
//             args.push_back(const_cast<char*>(arg.c_str()));
//         }
//         args.push_back(nullptr);
//         execvp(args[0], args.data());

//         std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
//         exit(1);
//     }

//     // Tiến trình cha
//     waitpid(pid, nullptr, 0);
// }

// // Hàm xử lý redirection 2>
// void execute_error_redirection(const std::vector<std::string>& command, const std::string& error_file) {
//     pid_t pid = fork();

//     if (pid < 0) {
//         std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
//         exit(1);
//     } else if (pid == 0) {
//         // Tiến trình con

//         // Mở tập tin lỗi đầu ra và chuyển đầu ra lỗi của tiến trình con vào tập tin
//         int error_fd = open(error_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (error_fd < 0) {
//             std::cerr << "Lỗi khi mở tập tin lỗi đầu ra." << std::endl;
//             exit(1);
//         }
//         dup2(error_fd, STDERR_FILENO);
//         close(error_fd);

//         // Thực thi lệnh command
//         std::vector<char*> args;
//         for (const auto& arg : command) {
//             args.push_back(const_cast<char*>(arg.c_str()));
//         }
//         args.push_back(nullptr);
//         execvp(args[0], args.data());

//         std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
//         exit(1);
//     }

//     // Tiến trình cha
//     waitpid(pid, nullptr, 0);
// }

// // Hàm xử lý redirection <
// void execute_input_redirection(const std::vector<std::string>& command, const std::string& input_file) {
//     pid_t pid = fork();

//     if (pid < 0) {
//         std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
//         exit(1);
//     } else if (pid == 0) {
//         // Tiến trình con

//         // Mở tập tin đầu vào và chuyển đầu vào của tiến trình con từ tập tin
//         int input_fd = open(input_file.c_str(), O_RDONLY);
//         if (input_fd < 0) {
//             std::cerr << "Lỗi khi mở tập tin đầu vào." << std::endl;
//             exit(1);
//         }
//         dup2(input_fd, STDIN_FILENO);
//         close(input_fd);

//         // Thực thi lệnh command
//         std::vector<char*> args;
//         for (const auto& arg : command) {
//             args.push_back(const_cast<char*>(arg.c_str()));
//         }
//         args.push_back(nullptr);
//         execvp(args[0], args.data());

//         std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
//         exit(1);
//     }

//     // Tiến trình cha
//     waitpid(pid, nullptr, 0);
// }

// // Hàm tách lệnh thành các command riêng biệt
// std::vector<std::vector<std::string>> split_commands(const std::vector<std::string>& input) {
//     std::vector<std::vector<std::string>> commands;
//     std::vector<std::string> command;

//     for (const std::string& token : input) {
//         if (token == "|" || token == ">" || token == ">>" || token == "2>" || token == "<") {
//             // Nếu gặp ký tự đặc biệt, lưu command hiện tại vào danh sách commands và chuẩn bị cho command tiếp theo
//             if (!command.empty()) {
//                 commands.push_back(command);
//                 command.clear();
//             }
//             command.push_back(token);
//         } else {
//             // Thêm từng token vào command hiện tại
//             command.push_back(token);
//         }
//     }

//     if (!command.empty()) {
//         // Lưu command cuối cùng vào danh sách commands
//         commands.push_back(command);
//     }

//     return commands;
// }

// int main() {
//     std::string input_string;
//     std::cout << "Nhập lệnh: ";
//     std::getline(std::cin, input_string);

//     std::vector<std::string> input;
//     std::string token;
//     std::stringstream ss(input_string);

//     while (ss >> token) {
//         input.push_back(token);
//     }

//     std::vector<std::vector<std::string>> commands = split_commands(input);

//     // Thực thi các command
//     for (size_t i = 0; i < commands.size(); i++) {
//         const std::vector<std::string>& command = commands[i];
//         if (command.size() == 1) {
//             std::string token = command[0];

//             if (token == "|") {
//                 // Xử lý phép pipe
//                 if (i > 0 && i < commands.size() - 1) {
//                     std::vector<std::string> command1 = commands[i - 1];
//                     std::vector<std::string> command2 = commands[i + 1];
//                     execute_pipe(command1, command2);
//                 } else {
//                     std::cerr << "Lỗi cú pháp: Pipe không hợp lệ." << std::endl;
//                 }
//             } else if (token == ">") {
//                 // Xử lý redirection >
//                 if (i > 0 && i < commands.size() - 1) {
//                     std::vector<std::string> command = commands[i - 1];
//                     std::string output_file = commands[i + 1][0];
//                     execute_redirection(command, output_file);
//                 } else {
//                     std::cerr << "Lỗi cú pháp: Redirection không hợp lệ." << std::endl;
//                 }
//             } else if (token == ">>") {
//                 // Xử lý redirection >>
//                 if (i > 0 && i < commands.size() - 1) {
//                     std::vector<std::string> command = commands[i - 1];
//                     std::string output_file = commands[i + 1][0];
//                     execute_append_redirection(command, output_file);
//                 } else {
//                     std::cerr << "Lỗi cú pháp: Redirection không hợp lệ." << std::endl;
//                 }
//             } else if (token == "2>") {
//                 // Xử lý redirection 2>
//                 if (i > 0 && i < commands.size() - 1) {
//                     std::vector<std::string> command = commands[i - 1];
//                     std::string error_file = commands[i + 1][0];
//                     execute_error_redirection(command, error_file);
//                 } else {
//                     std::cerr << "Lỗi cú pháp: Redirection không hợp lệ." << std::endl;
//                 }
//             } else if (token == "<") {
//                 // Xử lý redirection <
//                 if (i > 0 && i < commands.size() - 1) {
//                     std::vector<std::string> command = commands[i - 1];
//                     std::string input_file = commands[i + 1][0];
//                     execute_input_redirection(command, input_file);
//                 } else {
//                     std::cerr << "Lỗi cú pháp: Redirection không hợp lệ." << std::endl;
//                 }
//             }
//         }
//     }

//     return 0;
// }
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void execute_command(const std::vector<std::string>& args) {
    // Chuyển đổi vector<std::string> thành mảng null-terminated char*
    std::vector<char*> argv;
    for (const auto& arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    // Tạo pipe để kết nối giữa các lệnh
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        // Child process

        // Redirect input from previous command (if applicable)
        if (pipefd[0] != STDIN_FILENO) {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
        }

        // Redirect output to next command (if applicable)
        if (pipefd[1] != STDOUT_FILENO) {
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
        }

        // Execute the command
        execvp(argv[0], argv.data());
        perror("execvp");  // This line is reached only if execvp fails

        exit(EXIT_FAILURE);
    } else {
        // Parent process

        close(pipefd[0]);
        close(pipefd[1]);

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                std::cerr << "Child process exited with status: " << exit_status << std::endl;
            }
        } else if (WIFSIGNALED(status)) {
            int signal_number = WTERMSIG(status);
            std::cerr << "Child process terminated with signal: " << signal_number << std::endl;
        }
    }
}

int main() {
    std::string command = "ls -l | grep .cpp > output.txt";

    std::vector<std::string> commands;
    std::string delimiter = "|";

    // Phân tách các lệnh dựa trên delimiter
    size_t pos = 0;
    while ((pos = command.find(delimiter)) != std::string::npos) {
        std::string cmd = command.substr(0, pos);
        commands.push_back(cmd);
        command.erase(0, pos + delimiter.length());
    }
    commands.push_back(command);

    for (const auto& cmd : commands) {
        std::vector<std::string> args;

        // Phân tách các đối số dựa trên khoảng trắng
        char* token = strtok(const_cast<char*>(cmd.c_str()), " ");
        while (token != nullptr) {
            args.push_back(token);
            token = strtok(nullptr, " ");
        }

        execute_command(args);
    }

    return 0;
}
