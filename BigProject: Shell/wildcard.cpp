#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <glob.h>
#include <cstring>

using namespace std;
// Hàm tách lệnh thành các token
std::vector<std::string> tokenize(const std::string &input)
{
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(input);

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}
// Hàm thực thi lệnh bằng shell
void execute_command(const std::string &command)
{
    pid_t pid = fork();
    vector<string> args = tokenize(command);
    if (pid < 0)
    {
        std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
        exit(1);
    }
    else if (pid == 0)
    {
        // Tiến trình con

        // Thực thi lệnh command bằng shell
        // execlp("bash", "bash", "-c", command.c_str(), nullptr);

        // std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
        // exit(1);
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

    // Tiến trình cha
    waitpid(pid, nullptr, 0);
}
void execute_command(vector<string> &args)
{
    pid_t pid = fork();
    // vector<string> args = tokenize(command);
    if (pid < 0)
    {
        std::cerr << "Lỗi khi tạo tiến trình." << std::endl;
        exit(1);
    }
    else if (pid == 0)
    {
        // Tiến trình con

        // Thực thi lệnh command bằng shell
        // execlp("bash", "bash", "-c", command.c_str(), nullptr);

        // std::cerr << "Lỗi khi thực thi lệnh." << std::endl;
        // exit(1);
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

    // Tiến trình cha
    waitpid(pid, nullptr, 0);
}

// Hàm mở rộng wildcard
// Đầu vào: pattern = cái pattern chung mà mình cần tìm các file phù hợp (đơn giản thì nó là cái mình gõ vào tìm ý)
//      Ví dụ pattern: s*.cpp, ??ell.cpp,...
// Đầu ra: một vector các string là tên các path (file) thỏa mãn pattern
std::vector<std::string> expand_wildcard(const std::string &pattern)
{
    // Globbing = process of using wildcard to evaluate sets of files with the same partial names/characters
    // Structure describing a globbing run
    /* glob_t gồm 3 trường:
        1. gl_pathc = số path (hoặc file) khớp pattern
        2. gl_pathv = con trỏ trỏ tới list tên của các path (file) thỏa mãn đó
        3. gl_offs = số lượng phần tử bị reserve tính từ pathv (nói đơn giản là như offset)

    Ví dụ: gl_pathc = 3, gl_pathv là con trỏ p, fl_offs = 2 thì tức sẽ có 3 file phù hợp với yêu cầu (pattern)
            và 3 file đó có tên được lưu trong các địa chỉ được trỏ bởi p+2, p+3 và p+4 (còn 2 vị trí đầu p và p+1 bị reserve) */
    glob_t glob_result;

    // Set all bytes of glob_result to 0
    memset(&glob_result, 0, sizeof(glob_result));

    // Kiếm tất cả các path phù hợp vs pattern (argument đầu tiên) và trả về lưu vào glob_result (argument cuối)
    // Argument thứ 2 = flag để control behave của hàm glob:
    // GLOB_TILDE = để hiểu ~user và ~ chính là thư mục home
    // GLOB_BRACE = cho phép mở rộng "{a,b}" thành "a" "b"
    // Argument thứ 3 là con trỏ hàm để kiểm soát nếu trong thư mục đang xét có 1 thư mục con/file con k có quyền đọc hoặc mở
    // --> ở đây mình set là null nên k cần care (nếu k phải null sẽ chạy hàm được trỏ tới bởi con trỏ hàm)
    // Hàm kiểu int:
    // thành công: return 0 và update các trường tương ứng của glob_result
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

//vector<string> wildcard(vector<string> &command)
void wildcard(string &command)
{
    std::vector<std::string> tokens = tokenize(command);
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
            // Token bình thường, không chứa wildcard
            expanded_tokens.push_back(token);
            // for (int i = 0; i<expanded_tokens.size(); i++)
            // {
            //     cout<<expanded_tokens[i];
            // }
        }
    }
    // std::string expanded_command;
    // for (const std::string &token : expanded_tokens)
    // {
        // expanded_command += "\"" + token + "\" ";
        // expanded_command += token + " " ;
    // }
    execute_command(expanded_tokens);
}

// Hàm thực thi lệnh shell
void execute_shell_command(const std::string &command)
{
    std::vector<std::string> tokens = tokenize(command);
    std::vector<std::string> expanded_tokens;

    // input: token, output: vector<string>

    // Mở rộng các wildcard trong lệnh
    // chạy vòng for với mỗi token (token ở đây = 1 từ cách nhau = dấu cách) trong lệnh
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
            // Token bình thường, không chứa wildcard
            expanded_tokens.push_back(token);
        }
    }
    // for (int i = 0; i < expanded_tokens.size(); i++)
    // {
    //     cout<<"==========";
    //     cout << expanded_tokens[i];
    //     cout<<"==========";
    // }

    // Ghép các token đã mở rộng thành lệnh và thực thi
    // VD: nhập ls te* bt??? thì có các bước:
    // tách token thành ls; te* và bt???
    // Khởi tạo vector expanded_tokens trống
    // token 1 = ls --> giữ nguyên --> expanded_tokens có ls
    // token 2 = te* --> có wildcard --> chạy hàm expand_wildcard() và thu được expanded = tex1.txt, tex2.cpp
    // -> expanded_tokens có ls, tex1.txt, tex2.cpp
    // token 3 = bt??? --> có wildcard --> chạy hàm expand_wildcard() và thu được expanded = btvn1, bt1.c
    // -> expanded_tokens có ls, tex1.txt, tex2.cpp, btvn1, bt1.c
    // ghép lại các token sau mở rộng thành lệnh: expanded_command = "ls" "text1.txt" "tex2.cpp" "btvn1" "bt1.c"
    // chạy hàm execute_command và thu được kết quả text1.txt tex1.txt tex2.cpp btvn1 bt1.c
    std::string expanded_command;
    for (const std::string &token : expanded_tokens)
    {
        // expanded_command += "\"" + token + "\" ";
        expanded_command += token + " ";
    }
    // std::cout << expanded_command <<std::endl;
    execute_command(expanded_command);
}

int main()
{
    while (true)
    {
        std::string input_string;
        std::cout << "Nhập lệnh: ";
        std::getline(std::cin, input_string);

        // Kiểm tra lệnh thoát
        if (input_string == "exit")
        {
            break;
        }

        wildcard(input_string);
        // execute_shell_command(input_string);
    }

    return 0;
}