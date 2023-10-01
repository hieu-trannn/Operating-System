#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h> //for using the function sleep
#include <glob.h>

using namespace std;
// int pipling = 0;

void execute_external_command(const std::vector<std::string> &args)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        std::vector<char *> c_args;
        c_args.reserve(args.size() + 1);

        for (const auto &arg : args)
        {
            c_args.push_back(const_cast<char *>(arg.c_str()));
        }
        c_args.push_back(NULL);
        execvp(c_args[0], c_args.data());
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL);
    }
}

int execute_internal_command(const std::vector<std::string> &args)
{
    // cout<<"args0: "<< args[0]<<endl;
    // cout<<"args1: "<< args[1]<<endl;
    if (args[0] == "cd")
    {
        if (args.size() > 1)
        {
            cout << "a7" << endl;
            // sleep(2);
            if (chdir(args[1].c_str()) == -1)
            {
                perror("chdir");
            }
        }
        else
        {
            std::cout << "cd: missing argument" << std::endl;
        }
        return 1;
    }
    else if (args[0] == "pwd")
    {
        char current_dir[1024];
        if (getcwd(current_dir, sizeof(current_dir)) != NULL)
        {
            std::cout << current_dir << std::endl;
        }
        else
        {
            perror("getcwd");
        }
        return 1;
    }
    else if (args[0] == "echo")
    {
        for (size_t i = 1; i < args.size(); ++i)
        {
            std::cout << args[i] << " ";
        }
        std::cout << std::endl;
        return 1;
    }
    else
    {
        return 0;
    }
}

// find & character
bool AndCharacter(std::string &command)
{
    int found = command.find('&');
    if (found != std::string::npos)
    {
        return 1;
    }
    // k co ki tu dac biet
    return 0;
}

// ' cd hieudz  ' => 'cd hieudz'
void trimString(std::string &input)
{

    // eliminate " " at the beginning
    input.erase(input.begin(), std::find_if(input.begin(), input.end(), [](int ch)
                                            { return !std::isspace(ch); }));

    // // eliminate " " at the end
    input.erase(std::find_if(input.rbegin(), input.rend(), [](int ch)
                             { return !std::isspace(ch); })
                    .base(),
                input.end());
}

// commands -> tach dau & -> nhieu command nho
// tach 1 command (don)-> nhieu cum nho, dang vector
std::vector<std::string> split(std::string &command, const std::string &delimiter)
{
    trimString(command);
    std::vector<std::string> tokens;
    int start = 0;
    int end = command.find(delimiter);

    while (end != std::string::npos)
    {
        tokens.push_back(command.substr(start, end - start));
        start = end + delimiter.length();
        end = command.find(delimiter, start);
    }

    tokens.push_back(command.substr(start, end));
    return tokens;
}

// neu co ki tu ' => deal
// neu k co ki tu ' => split voi " " (1 command)
std::vector<std::string> ProcessCommandLine(std::string &command)
{
    vector<string> tokens;
    std::size_t firstQuotePos = command.find('\'');
    std::size_t secondQuotePos = command.find('\'', firstQuotePos + 1);

    if (firstQuotePos != std::string::npos && secondQuotePos != std::string::npos)
    {
        std::string string1 = command.substr(0, firstQuotePos - 1);
        std::string string2 = command.substr(firstQuotePos + 1, secondQuotePos - firstQuotePos - 1);
        tokens = split(string1, " ");
        tokens.push_back(string2);
        return tokens;
    }
    else
    {
        return split(command, " ");
    }
}

// void handle_pipling (std::vector<std::string> &args)
// {
//     std::string output_file;
//     for (auto it = args.begin(); it != args.end(); ++it)
//     {
//         if (*it == "|")
//         {
//             output_file = *(it + 1);
//             args.erase(it, it + 2);
//             break;
//         }
//     }
// }

void handle_redirection(std::vector<std::string> &args)
{
    std::string output_file;
    std::string input_file;
    std::string error_file;
    bool append = false;

    for (auto it = args.begin(); it != args.end(); ++it)
    {
        if (*it == ">")
        {
            // get name of output_file
            output_file = *(it + 1);
            args.erase(it, it + 2);
            break;
        }
        else if (*it == ">>")
        {
            output_file = *(it + 1);
            args.erase(it, it + 2);
            append = true;
            break;
        }
        else if (*it == "<")
        {
            input_file = *(it + 1);
            args.erase(it, it + 2);
            break;
        }
        else if (*it == "2>")
        {
            error_file = *(it + 1);
            args.erase(it, it + 2);
            break;
        }
    }

    if (!output_file.empty())
    {
        int fd;
        if (append)
        {
            fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }

        if (fd == -1)
        {
            perror("open");
            return;
        }

        // copy from STDOUT to fd
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
        }

        close(fd);
    }

    if (!input_file.empty())
    {
        int fd = open(input_file.c_str(), O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            return;
        }

        // copy input from fd to STDIN
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
        }

        close(fd);
    }

    if (!error_file.empty())
    {
        int fd;
        if (append)
        {
            fd = open(error_file.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            fd = open(error_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }

        if (fd == -1)
        {
            perror("open");
            return;
        }

        if (dup2(fd, STDERR_FILENO) == -1)
        {
            perror("dup2");
        }

        close(fd);
    }
}

std::vector<std::string> expand_wildcard(const std::string &pattern)
{

    glob_t glob_result;

    // Set all bytes of glob_result to 0
    memset(&glob_result, 0, sizeof(glob_result));

    glob(pattern.c_str(), GLOB_TILDE | GLOB_BRACE, nullptr, &glob_result);

    std::vector<std::string> expanded; // vector các string lưu tên các path thỏa mãn
    // vòng for để đẩy tên các path thỏa mãn vào vector string
    for (size_t i = 0; i < glob_result.gl_pathc; i++)
    {
        expanded.push_back(glob_result.gl_pathv[i]);
    }

    // giải phóng glob_result
    globfree(&glob_result);

    return expanded;
}
// ham xu li: dau vao: vector string cac command nho, dau ra: thi hanh

vector<string> wildcard(vector<string> &tokens)
{
    std::vector<std::string> expanded_tokens;
    for (const std::string &token : tokens)
    {
        if (token.find('*') != std::string::npos || token.find('?') != std::string::npos)
        {
            // Nếu token chứa wildcard ? hoặc *, mở rộng nó thành danh sách các tệp phù hợp bằng hàm expand_wildcard và lưu vào vector các string expanded
            // expanded là vector các string là tên các file thỏa mãn ứng với MỖI TOKEN
            std::vector<std::string> expanded = expand_wildcard(token);
            // expanded_tokens là vector tên các path (file) thỏa mãn ÍT NHẤT 1 token
            // chèn expanded vào expanded_tokens
            expanded_tokens.insert(expanded_tokens.end(), expanded.begin(), expanded.end());
        }
        else
        {
            expanded_tokens.push_back(token);
        }
    }
    return expanded_tokens;
}
/*
input: vector<string> (1 command don)
output: vector<string>
eg: {ls, -l} => {asdsa,asdas,asdasd}
    - pipling: dua output (vector<string>) vao args
*/
vector<string> getCommandOuput(vector<string> &args)
{
    // string command;
    vector<string> result;
    std::string command;
    for (const auto &word : args)
    {
        command += word + " ";
    }
    // std::string result;
    FILE *pipe = popen(command.c_str(), "r");
    if (pipe)
    {
        constexpr int buffer_size = 128;
        char buffer[buffer_size];

        while (!feof(pipe))
        {
            if (fgets(buffer, buffer_size, pipe) != nullptr)
            {
                // result.push_back("\"");
                result.push_back(buffer);
                // result.push_back("\"");
            }
        }

        pclose(pipe);
    }
    return result;
}
bool CheckPipling(vector<string> args)
{
    for (const auto &line : args)
    {
        if (line == "|")
        {
            // pipling = 1;
            return 1;
        }
    }
    return 0;
}

void GetFullCommand(vector<string> &args)
{
    // vector<string> temp = args;
    vector<string> result;
    std::string command;
    for (const auto &word : args)
    {
        command += word + " ";
    }
    vector<string> result1;
    // {ls1, |, ls2, |, ls3} =>{ls1,ls2,ls3}
    vector<string> temp = split(command, "|");

    std::string input = "";
    result.push_back(temp.back());
    for (int i = 0; i < temp.size() - 1; i++)
    {
        vector<string> full_command{temp[i], input};
        result1 = getCommandOuput(full_command);
        string new_input;
        for (const auto &r : result1)
        {
            new_input += r;
        }
        input = new_input;
    }
    for (int i = 0; i < result1.size(); i++)
    {
        result.push_back(result1[i]);
    }
    string output_string;
    for (int i = 1; i < result.size(); i++)
    {
        output_string += result[i] + " ";
    }
    vector<string> Full_command{result[0], output_string};
    args = getCommandOuput(Full_command);
}

void execute_one_command(std::string &command)
{
    // Command -> vector of string
    // cd hieudz -> {"cd", "hieudz"}
    std::vector<std::string> args = ProcessCommandLine(command);
    cout << "a1" << endl;
    // wildcard
    args = wildcard(args);
    cout << "a2" << endl;
    // Handle redirection
    handle_redirection(args);
    cout << "a3" << endl;
    // pipling
    if (CheckPipling)
    {
        cout << "a4" << endl;
        GetFullCommand(args);
    }
    cout << "a5" << endl;
    // if (CheckPipling)
    // {
    //     vector<string> out = GetFullCommand(args);
    //     cout<<"=====Output of Pipling======"<<endl;
    //     for (auto i : out)
    //     {
    //         cout << i << endl;
    //     }
    // }
    // else
    // {
    // get out
    if (args[0] == "exit")
    {cout << "a6" << endl;
        exit(0);
    }
    // Execute commands
    if (!execute_internal_command(args))
    {
        cout << "a7" << endl;
        // Execute external command
        execute_external_command(args);
    }
    // }
}

void execute_command(std::string &command)
{
    if (AndCharacter(command))
    {
        vector<string> tokens = split(command, "&");

        for (int i = 0; i < tokens.size(); i++)
        {
            execute_one_command(tokens[i]);
        }
    }
    else
    {
        execute_one_command(command);
    }
}

int main()
{
    std::string command;

    while (true)
    {
        std::cout << "Shell> ";
        std::getline(std::cin, command);

        if (command.empty())
        {
            continue;
        }
        execute_command(command);
    }

    return 0;
}