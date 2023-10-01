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
// flow ghep:
/*
    Gop dau nhay nghieng voi pipling

*/
// std::string execute_special_character(vector<string> &commands)
// {

// }
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

// change:
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
                result.push_back(buffer);
            }
        }

        pclose(pipe);
    }
    // cout<<"============="<<result<<"================"<<endl;
    return result;
}

// get the output of command to string result
// input: string
// output: string (result of command input)
std::string execute_command(const std::string &command)
{
    std::string result;
    FILE *pipe = popen(command.c_str(), "r");
    if (pipe)
    {
        constexpr int buffer_size = 128;
        char buffer[buffer_size];

        while (!feof(pipe))
        {
            if (fgets(buffer, buffer_size, pipe) != nullptr)
            {
                result += buffer;
            }
        }

        pclose(pipe);
    }
    // cout<<"============="<<result<<"================"<<endl;
    return result;
}

// get pipling string: turn args -> standard form
//  {ls1, |, ls2, |, ls3} =>{ls3 (out)(ls2 (out(ls1)))}

void GetFullCommand(vector<string> &args)
{
    // vector<string> temp = args;
    vector<string> result;
    std::string command;
    for (const auto& word : args) {
        command += word + " ";
    }
// vector<string> GetFullCommand(string &command)
// vector<string> GetFullCommand(vector<string> args)
// {
    
    // vector<string> temp = args;
    // vector<string> result;
    vector<string> result1;
    // std::string command;
    // for (const auto& word : args) {
    //     command += word + " ";
    // }
    // {ls1, |, ls2, |, ls3} =>{ls1,ls2,ls3}
    vector<string> temp = split(command, "|");
    // cout << "========" << endl;
    // for (auto i : temp)
    // {
    //     cout << i << endl;
    // }
    // execute pipeline
    //  std::string result;
    std::string input = "";
    result.push_back(temp.back());
    for (int i = 0; i < temp.size()-1; i++)
    {
        vector<string> full_command{temp[i], input};

        // std::string full_command = command + " " + input;
        // result = execute_command(full_command);
        result1 = getCommandOuput(full_command);
        // cout << "21313123123" << endl;
        // for (auto i : result1)
        // {
        //     cout << i << endl;
        //     // result.push_back()

        // }
        // cout << "asdasdasdasd" << endl;
        string new_input;
        for (const auto &r : result1)
        {
            new_input += r;
        }
        //  = result;
        input = new_input;
    }
    for (int i = 0; i < result1.size(); i++)
    {
        result.push_back(result1[i]);
    }
    args = result;
    // cout << "21313123123" << endl;
    // for (auto i : result)
    // {
    //     cout << i << endl;
    // }
    // cout << "asdasdasdasd" << endl;
    // return result;
}

// input: cac command rieng le (vd: cd hieudz), output: thuc hien lenh
std::string execute_pipeline(const std::vector<std::string> &commands)
{
    std::string result;
    std::string input = "";

    for (const auto &command : commands)
    {
        std::string full_command = command + " " + input;
        result = execute_command(full_command);
        input = result;
    }

    return result;
}

int main()
{

    std::string command = "ls |grep a";

    // tim | den khi het trong command all => tach thanh tung command rieng le
    // std::vector<std::string> commands;
    // size_t pipe_pos = command.find('|');

    // while (pipe_pos != std::string::npos) {
    //     std::string sub_command = command.substr(0, pipe_pos);
    //     commands.push_back(sub_command);
    //     command = command.substr(pipe_pos + 1);
    //     pipe_pos = command.find('|');
    // }

    // commands.push_back(command);
    // commands: cac lenh ngan boi dau |

    // cout<<"=====asda====="<<endl;
    // for (int i = 0; i< commands.size(); i++)
    // {
    //     cout<<commands[i]<<endl;
    // }

    // std::string output = execute_pipeline(commands);
    // std::cout << "Output: " << output << std::endl;
    vector<string> temp = split(command, " ");

    // vector<string> all = GetFullCommand(temp);
    GetFullCommand(temp);
    string output_string;
    for (int i = 1; i< temp.size(); i++)
    {
        output_string += temp[i] + " ";
    }
    vector<string> arg{temp[0],output_string};
    for (auto &a: arg)
    {
        cout<<a;
    }
    vector<string> result = getCommandOuput(arg);
    cout << "===================" << endl;
    for (auto i : result)
    {
        cout << i << endl;
    }
    // cout << "===================" << endl;
    // GetFullCommand(command);

        return 0;
}
