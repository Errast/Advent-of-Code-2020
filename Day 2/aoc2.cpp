#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <locale>

// this is fucking stupid
struct field_reader: std::ctype<char> {

    field_reader(): std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const* get_table() {
        static std::vector<std::ctype_base::mask> 
            rc(table_size, std::ctype_base::mask());

        rc['\n'] = std::ctype_base::space;
        return &rc[0];
    }
};

struct PasswordPolicy {
    char letter;
    int num1;
    int num2;
    std::string password;
};

bool isValid(PasswordPolicy pp) {
    int occurrences = std::count(pp.password.begin(), pp.password.end(), pp.letter);
    return pp.num1 <= occurrences && pp.num2 >= occurrences;
}

bool isValid2(PasswordPolicy pp) {
    return (pp.password.at(pp.num1 - 1) == pp.letter) != (pp.password.at(pp.num2 - 1) == pp.letter);
}

PasswordPolicy makePasswordPolicy(std::string str) {
    int dash { static_cast<int>(str.find('-')) };
    int space { static_cast<int>(str.find(' ')) };
    int colon { static_cast<int>(str.find(':')) };

    int min { std::stoi(str.substr(0, dash)) };
    int max { std::stoi(str.substr(dash + 1, space - dash - 1)) };
    char letter { str.at(space + 1) };
    std::string password { str.substr(colon + 2) };

    return { letter, min, max, password };
}

int main() {
    std::ifstream input { "2 - input.txt" };
    input.imbue(std::locale(std::locale(), new field_reader()));
    std::istream_iterator<std::string> start { input }, end { };
    std::vector<std::string> passwords { start, end };

    std::vector<PasswordPolicy> pps { };
    std::transform(passwords.begin(), passwords.end(), std::back_inserter(pps), ::makePasswordPolicy);

    // part 1
    std::vector<bool> valid { };
    std::transform(pps.begin(), pps.end(), std::back_inserter(valid), ::isValid);
    std::cout << std::count(valid.begin(), valid.end(), true);

    // part 2
    std::vector<bool> valid2 { };
    std::transform(pps.begin(), pps.end(), std::back_inserter(valid2), ::isValid2);
    std::cout << '\n' << std::count(valid2.begin(), valid2.end(), true);
}