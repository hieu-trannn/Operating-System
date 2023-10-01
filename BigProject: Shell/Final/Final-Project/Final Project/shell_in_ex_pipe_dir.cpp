#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h> //for using the function sleep in cd
#include <glob.h>
#include <cstdio>

using namespace std;

// Execute external command: ls, grep, cat,... (in /usr/bin)
void execute_external_command(vector<string> &args)
{
    // fork to make sure program continues running
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        vector<char *> c_args;
        c_args.reserve(args.size() + 1);

        for (const auto &arg : args)
        {
            // const_cast: cast away constness (temp). arg.c_str: const pointer to arg
            c_args.push_back(const_cast<char *>(arg.c_str()));
        }
        c_args.push_back(NULL);
        // c_args.data: A pointer to the first element in the array used internally by the vector.
        execvp(c_args[0], c_args.data());
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL);
    }
}

// Execute internal command: cd, pwd, echo
// if single command does not have internal command, return 0
int execute_internal_command(vector<string> &args)
{
    if (args[0] == "cd")
    {
        if (args.size() > 2)
        {
            cout << "bash: cd: too many arguments" << endl;
        }
        else if (args.size() == 2)
        {
            if (chdir(args[1].c_str()) == -1)
            {
                perror("chdir");
            }
        }
        else
        {
            args = {"cd", "/home"};
            if (chdir(args[1].c_str()) == -1)
            {
                perror("chdir");
            }
        }
        return 1;
    }
    else if (args[0] == "pwd")
    {
        char current_dir[1024];
        // return NULL if current_dir: not or size: small
        if (getcwd(current_dir, sizeof(current_dir)) != NULL)
        {
            cout << current_dir << endl;
        }
        else
        {
            perror("getcwd");
        }
        return 1;
    }
    else if (args[0] == "echo")
    {
        for (int i = 1; i < args.size(); ++i)
        {
            cout << args[i] << " ";
        }
        cout << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}

// find & character
bool AndCharacter(string &command)
{
    int found = command.find('&');
    if (found != string::npos)
    {
        return 1;
    }
    return 0;
}

//  eliminate " " at the begin & end
// ' cd hieudz  ' => 'cd hieudz'
void trimString(string &input)
{

    // eliminate " " at the begin
    // []: lamda function, true if ch: not white space character
    //The function returns an iterator pointing to the first element in the range [first, last) 
    //that satisfies the condition specified by the predicate pred.
    input.erase(input.begin(), std::find_if(input.begin(), input.end(), [](int ch)
                                            { return !std::isspace(ch); }));

    // eliminate " " at the end
    // .base(): This part converts the reverse iterator obtained from std::find_if 
    // to a forward iterator by accessing its base iterator.
    input.erase(std::find_if(input.rbegin(), input.rend(), [](int ch)
                             { return !std::isspace(ch); })
                    .base(),
                input.end());
}

// one complex command with & to many simple commands
//"ls -l & pwd & cd /home" -> "ls -l" "pwd" "cd /home"
// simple command => vector: "ls -l" => "ls" "-l"
vector<string> split(string &command, const string &delimiter)
{
    trimString(command);
    vector<string> tokens;
    int start = 0;
    int end = command.find(delimiter);

    while (end != string::npos)
    {
        tokens.push_back(command.substr(start, end - start));
        start = end + delimiter.length();
        end = command.find(delimiter, start);
    }

    tokens.push_back(command.substr(start, end));
    return tokens;
}

// Function return position of Quote symbol
vector<int> delimiterPos(string &command, const string &delimiter)
{
    vector<int> pos;
    int trace = 0UL;
    while ((command.find(delimiter, trace) != string::npos) && (trace <= command.length()))
    {
        trace = command.find(delimiter, trace);
        pos.push_back(trace);
        trace++;
    }
    return pos;
}
// if command has character ' => process: "cd 'Operating System'" => "cd" "Operating System"
// if not: split normally: "cd /home" => "cd" "/home"
vector<string> ProcessCommandLine(string &command)
{
    vector<string> tokens;
    vector<int> posQuote, posBlank;
    posQuote = delimiterPos(command, "\'");
    posBlank = delimiterPos(command, " ");

    if (posQuote.size() % 2 == 1)
    {
        cout << "cd: missing apostrophe" << endl;
        return tokens;
    }
    else if (posQuote.size() > 0)
    {
        for (int i = 0; i < posQuote.size(); i = i + 2)
        {
            for (auto blank = posBlank.begin(); blank != posBlank.end();)
            {
                if ((*blank > posQuote.at(i)) && (*blank < posQuote.at(i + 1)))
                    posBlank.erase(std::remove(posBlank.begin(), posBlank.end(), *blank), posBlank.end());
                else
                    blank++;
            }
        }

        tokens.push_back(command.substr(0, posBlank.at(0)));

        for (int i = 0; i < posBlank.size() - 1;)
        {
            string my_str = command.substr(posBlank.at(0) + 1, posBlank.at(1) - posBlank.at(0) - 1);
            my_str.erase(remove(my_str.begin(), my_str.end(), '\''), my_str.end());
            tokens.push_back(my_str);
            posBlank.erase(posBlank.begin());
        }
        if (command.length() - 1 > posBlank.at(0))
        {
            string my_str = command.substr(posBlank.at(0) + 1, command.length() - 1);
            my_str.erase(remove(my_str.begin(), my_str.end(), '\''), my_str.end());
            tokens.push_back(my_str);
        }

        return tokens;
    }
    else
        return split(command, " ");
}

// save /load output of command to/from file
void handle_redirection(vector<string> &args)
{
    string output_file;
    string input_file;
    string error_file;
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
            // open for writing only, create, append, 0644: read & write permisson for owner. read for other
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

        // Redirect the output file descriptor to stdout
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

        // Redirect the input file descriptor to stdin
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

// need to process wildcard:
// a* => ab.txt, ac.txt, ad.txt
vector<string> expand_wildcard(const string &pattern)
{

    glob_t glob_result;

    // Set all bytes of glob_result to 0
    memset(&glob_result, 0, sizeof(glob_result));

    glob(pattern.c_str(), GLOB_TILDE | GLOB_BRACE, nullptr, &glob_result);

    vector<string> expanded; // vector các string lưu tên các path thỏa mãn
    // vòng for để đẩy tên các path thỏa mãn vào vector string
    for (int i = 0; i < glob_result.gl_pathc; i++)
    {
        expanded.push_back(glob_result.gl_pathv[i]);
    }

    // giải phóng glob_result
    globfree(&glob_result);

    return expanded;
}

vector<string> wildcard(vector<string> &tokens)
{
    vector<string> expanded_tokens;
    for (const string &token : tokens)
    {
        if (token.find('*') != string::npos || token.find('?') != string::npos)
        {
            // Nếu token chứa wildcard ? hoặc *, mở rộng nó thành danh sách các tệp phù hợp bằng hàm expand_wildcard và lưu vào vector các string expanded
            // expanded là vector các string là tên các file thỏa mãn ứng với MỖI TOKEN
            vector<string> expanded = expand_wildcard(token);
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
input: vector<string> (1 single command)
output: result of this command in form of vector<string>
*/
vector<string> getCommandOuput(vector<string> &args)
{
    vector<string> result;
    string command;
    for (const auto &word : args)
    {
        command += word + " ";
    }

    FILE *pipe = popen((command + " 2>/dev/null").c_str(), "r");
    if (pipe)
    {
        // constexpr indicates that the value,
        // or return value, is constant and, where possible, is computed at compile time.
        constexpr int buffer_size = 128;
        char buffer[buffer_size];

        // get output line by line
        while (fgets(buffer, buffer_size, pipe) != nullptr)
        {
            result.push_back(buffer);
        }

        pclose(pipe);
    }
    return result;
}

// return 1 if single command (in form of vector<string>) has |
bool CheckPipling(const vector<string> &args)
{
    for (const auto &line : args)
    {
        if (line == "|")
        {
            return 1;
        }
    }
    return 0;
}

// return 1 if single command (in form of vector<string>) has character `
bool CheckItalicComma(vector<string> &args)
{
    for (const auto &line : args)
    {
        if (*line.begin() == '`')
        {
            return 1;
        }
    }
    return 0;
}

void execute_one_command(string &command);

// Execute single command in form of vector<string>(with Italic Comma). Print result to Terminal
// Eg: {"echo","`date`"} => print: "Chu nhat" "ngay 16" "thang 7"....
void GetItalicCommaCommandOutput(vector<string> &args)
{
    vector<string> out_command;
    vector<string> temp = args;

    string input = "";

    // skip first command (echo)
    for (int i = 1; i < temp.size(); i++)
    {
        // eliminate character `
        if (*temp[i].begin() == '`')
        {
            temp[i] = temp[i].substr(1, temp[i].length() - 2);
            vector<string> inline_command;
            inline_command.push_back(temp[i]);
            vector<string> temp_out = getCommandOuput(inline_command);
            for (int i = 0; i < temp_out.size(); i++)
            {
                out_command.push_back(temp_out[i]);
            }
        }
        else
        {
            out_command.push_back(temp[i]);
        }
    }
    string output_string;
    output_string = "'";
    for (int i = 0; i < out_command.size() - 1; i++)
    {
        output_string += out_command[i] + " ";
    }
    output_string += out_command[out_command.size() - 1] + "'";
    trimString(temp.front());
    trimString(output_string);
    string output = temp.front() + " " + output_string;
    execute_one_command(output);
}

// input: single command, output: result
void execute_one_command(string &command)
{
    // Single Command -> vector of string
    //" cd hieudz " -> {"cd", "hieudz"}
    vector<string> args = ProcessCommandLine(command);

    // call wildcard
    args = wildcard(args);

    // Handle redirection
    handle_redirection(args);

    // if single command has character "|" or "`", execute this special circumstance
    if (CheckPipling(args))
    {
        vector<string> Output = getCommandOuput(args);
        for (auto &it : Output)
        {
            cout << it;
        }
    }
    else if (CheckItalicComma(args))
    {
        GetItalicCommaCommandOutput(args);
    }
    else if (args[0] == "exit")
    {
        exit(0);
    }
    // Execute internal commands
    else if (!execute_internal_command(args))
    {
        // Execute external command
        execute_external_command(args);
    }
}
// process comma with &
// "cd 'Operating & System' & cd 'Operating& System'" => "cd 'Operating : System' & cd 'Operating: System'"
void replaceAmpersand(string &str)
{
    bool insideQuotes = false;

    for (int i = 0; i < str.length(); ++i)
    {
        if (str[i] == '\'')
        {
            insideQuotes = !insideQuotes;
        }
        else if (insideQuotes && str[i] == '&')
        {
            str[i] = ':';
        }
    }
}

// Execute complex command (with character &)
void execute_command(string &command)
{
    if (AndCharacter(command))
    {
        // replace & in single comma
        replaceAmpersand(command);

        //"ls & pwd" => {"ls", "pwd"}
        vector<string> tokens = split(command, "&");
        for (int i = 0; i < tokens.size(); i++)
        {
            // replace : with &
            for (auto &it : tokens[i])
            {
                if (it == ':')
                {
                    it = '&';
                }
            }
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
    string command;

    while (true)
    {
        cout << "Command: ";
        std::getline(std::cin, command);

        if (command.empty())
        {
            continue;
        }
        execute_command(command);
    }

    return 0;
}