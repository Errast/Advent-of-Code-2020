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

    inline bool operator==(const boarding_pass& bp) {
        return row == bp.row && column == bp.column;
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

    // part 1
    std::cout << std::max_element(passes.begin(), passes.end(), [](const auto x, const auto y){ return x.getSeatId() < y.getSeatId(); })->getSeatId() << '\n';

    // part 2
    const int minRow { std::min_element(passes.begin(), passes.end(), [](const auto x, const auto y){ return x.row < y.row; })->row };
    const int maxRow { std::max_element(passes.begin(), passes.end(), [](const auto x, const auto y){ return x.row < y.row; })->row };

    for(int row { minRow + 1}; row < maxRow; row++) {
        for( int col { 0 }; col < 8; col++) {
            if(std::find(passes.begin(), passes.end(), boarding_pass{ row, col }) == passes.end()) {
                std::cout << row << '\t' << col;
                return 0;
            }
        }
    }
}