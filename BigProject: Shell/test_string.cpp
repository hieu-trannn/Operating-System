// // #include <iostream>
// // #include <vector>
// // #include <bits/stdc++.h>
// // #include <string>
// // #include <cstring>
// // #include <unistd.h>
// // #include <sys/wait.h>
// // #include <fcntl.h>
// // #include <stdlib.h> //for using the function sleep
// // using namespace std;
// // int main()
// // {
// //     std::string command;

// //     std::getline(std::cin, command);

// //     for (auto it = command.begin(); it != command.end(); it++)
// //     {
// //         // character '
// //         if (*it == '7')
// //         {
// //             auto first = it;
// //             auto second = std::find(it + 1, command.end(), '7');
// //             string command1(command.begin(), first);
// //             string command2(first, second);
// //             string command3(second, command.end());
// //             cout << "======" << endl
// //                  << command1 << endl
// //                  << command2 << endl
// //                  << command3 << endl;
// //             // tokens = split(command1, delimiter);
// //             // tokens.push_back(command2);
// //             // vector<string> tokens3 = split(command3, delimiter);
// //             // vector<string> vect1(tokens.size() + tokens3.size());
// //             // merge(tokens.begin(),
// //             //       tokens.end(),
// //             //       tokens3.begin(),
// //             //       tokens3.end(),
// //             //       vect1.begin());
// //             // return vect1;
// //         }
// //     }
// // }

#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

std::vector<std::string> split(const std::string &command, const std::string &delimiter)
{
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
int main()
{
    vector<string> tokens;
    std::string command;
    std::cout << "Nhập một chuỗi: ";
    std::getline(std::cin, command);

    std::size_t firstQuotePos = command.find('\'');
    std::size_t secondQuotePos = command.find('\'', firstQuotePos + 1);

    if (firstQuotePos != std::string::npos && secondQuotePos != std::string::npos)
    {
        std::string string1 = command.substr(0, firstQuotePos-1);
        std::string string2 = command.substr(firstQuotePos+1, secondQuotePos - firstQuotePos-1);
        std::string string3 = command.substr(secondQuotePos+1);

        std::cout << "String 1: " << string1 << std::endl;
        std::cout << "String 2: " << string2 << std::endl;
        std::cout << "String 3: " << string3 << std::endl;

        tokens = split(string1, " ");
        tokens.push_back(string2);

        vector<string> tokens3 = split(string3, " ");
        // for (int i = 0; i < tokens3.size(); i++)
        // {
        //     tokens.push_back(tokens3[i]);
        //     // cout << i << vect1[i] << endl;
        // }
        // vector<string> vect1(tokens.size() + tokens3.size());
        // merge(tokens.begin(),
        //       tokens.end(),
        //       tokens3.begin(),
        //       tokens3.end(),
        //       vect1.begin());
        
        // for (int i = 0; i < vect1.size(); i++)
        // {
        //     cout << i << vect1[i] << endl;
        // }
        // for (int i = 0; i < tokens.size(); i++)
        // {
        //     cout << i << tokens[i] << endl;
        // }
    }
    return 0;
}

// #include <iostream>
// #include <string>
// #include <algorithm>

// std::string trimString(const std::string& input) {
//     std::string trimmed = input;

//     // Loại bỏ dấu cách ở đầu chuỗi
//     trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](int ch) {
//         return !std::isspace(ch);
//     }));

//     // Loại bỏ dấu cách ở cuối chuỗi
//     trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](int ch) {
//         return !std::isspace(ch);
//     }).base(), trimmed.end());

//     return trimmed;
// }

// int main() {
//     std::string input = "  Hello World       ";

//     std::string trimmed = trimString(input);

//     std::cout << "Trimmed string: '" << trimmed << "'" << std::endl;

//     return 0;
// }
