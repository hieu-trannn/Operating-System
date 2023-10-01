#include <iostream>
#include <string>

void replaceAmpersand(std::string& str) {
    bool insideQuotes = false;

    for (std::size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '\'') {
            insideQuotes = !insideQuotes;
        }
        else if (insideQuotes && str[i] == '&') {
            str[i] = ':';
        }
    }
}

int main() {
    std::string str = "cd 'Operating & System' & cd 'Operating& System' 'asd &&adsf'";

    std::cout << "Original string: " << str << std::endl;
    
    replaceAmpersand(str);

    std::cout << "Modified string: " << str << std::endl;

    return 0;
}
