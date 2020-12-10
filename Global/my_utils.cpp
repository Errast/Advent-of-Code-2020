#pragma once
#include <vector>
#include <string>
#include <fstream>

template<typename T>
std::vector<T> getData(const char* fileName) {
    std::ifstream input{ fileName };
    const std::istream_iterator<int> start{ input };
    const std::istream_iterator<int> end{ };
    return std::vector<T>{ start, end };
}