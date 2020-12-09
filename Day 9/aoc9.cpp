#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>

template<typename T>
using vec_iter_const = typename std::vector<T>::const_iterator;

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

template<typename T>
std::pair<vec_iter_const<T>, vec_iter_const<T>> findRangeSumTo(vec_iter_const<T> start, vec_iter_const<T> end, T value) {
    for(auto rangeStart{ start }; rangeStart != end; ++rangeStart) {
        auto rangeEnd{ rangeStart + 2 };
        T total { rangeStart[0] + rangeStart[1] };
        while(total < value && rangeEnd != end) {
            total += *rangeEnd;
            ++rangeEnd;
        }
        if(total == value) {
            return {rangeStart, rangeEnd - 1};
        }
    }
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

    // part 2
    auto result{ findRangeSumTo(nums.begin(), nums.end(), invalidNum) };
    int min{ *std::min_element(result.first, result.second) };
    int max{ *std::max_element(result.first, result.second) };
    std::cout << min + max << '\n';
}