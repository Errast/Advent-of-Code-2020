#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

struct boarding_pass {
    int row;
    int column;

    int getSeatId() const {
        return row * 8 + column;
    }
};

boarding_pass parseBoardingPass(std::string string) {
    int row { 0 };
    for(int i { 0 }; i < 7; i++) {
        row += (string.at(i) == 'B') * (64 >> i);
    }

    int col { 0 };
    for(int i { 0 }; i < 3; i++) {
        col += (string.at(i + 7) == 'R') * (4 >> i);
    }

    return { row, col };
}

int main() {
    std::ifstream input { "5 - input.txt" };
    std::istream_iterator<std::string> start { input }, end { };
    std::vector<std::string> strings { start, end };

    std::vector<boarding_pass> passes { };
    std::transform(strings.begin(), strings.end(), std::back_inserter(passes), ::parseBoardingPass);

    std::cout << std::max_element(passes.begin(), passes.end(), [](const auto x, const auto y){ return x.getSeatId() < y.getSeatId(); })->getSeatId();

}