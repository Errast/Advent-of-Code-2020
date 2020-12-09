#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>

constexpr std::string_view file_name{ "9 - input.txt" };
constexpr int previousNums() {
    if constexpr (file_name == "9 - test.txt") {
        return 5;
    } else if constexpr (file_name == "9 - input.txt") {
        return 25;
    }
    return -1;
}

bool isValid(int index, const std::vector<int>& nums) {
    if(index < previousNums()) {
        return true;
    }
    
    auto numIter{ nums.cbegin() + index };
    auto startIter{ numIter - previousNums()};
    for(auto elem{ startIter }; elem != numIter; ++elem) {
        if(std::find(startIter, numIter, *numIter - *elem) != numIter) {
            return true;
        }
    }

    return false;
}

int main() {
    std::ifstream input{ std::string{ file_name } };
    const std::istream_iterator<int> start{ input };
    const std::istream_iterator<int> end{ };
    const std::vector<int> nums{ start, end };

    // part 1
    int invalidNum{ };
    for(int i = previousNums(); i < nums.size(); ++i) {
        if(!isValid(i, nums)) {
            invalidNum = nums[i];
            break;
        }
    }
    std::cout << invalidNum << '\n';
}