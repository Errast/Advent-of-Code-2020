#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace mine {
    template<typename T>
    std::vector<T> getData(const char* filename) {
        std::ifstream input{ filename };
        const std::istream_iterator<T> start{ input };
        const std::istream_iterator<T> end{ };
        return std::vector<T>{ start, end };
    }

    std::vector<std::string> getLines(const char* filename) {
        std::ifstream input{ filename };
        std::vector<std::string> strings{ };
        for(std::string line; std::getline(input, line);) {
            strings.emplace_back(line);
        }

        return strings;
    }

    constexpr uint64_t factorial(uint64_t num) {
        int total { 1 };
        for(int i = 2; i <= num; i++) {
            total *= num;
        }
        return total;
    }

    template<typename T, typename S>
    inline bool hasKey(const T& key, const std::map<T, S>& map) {
        return map.find(key) != map.end();
    }

    template<typename T>
    int totalDepth(T value, 
                   const std::map<T, std::vector<std::pair<int, T>>>& map, 
                   std::map<T, int>& cache = std::map<T, int>{ }) {
        if(mine::hasKey(value, cache)) {
            return cache[value];
        }
    
        int count{ 1 };
        for(auto elem : map.at(value)) {
            count += elem.first * totalDepth(elem.second, map, cache);
        }
        cache[value] = count;
        return count;
    }

    template<class InputIt, class OutputIt, class UnaryOperation>
    inline std::back_insert_iterator<OutputIt> transform(const InputIt& input, OutputIt& output, UnaryOperation unary_op) {
        return std::transform(std::begin(input), std::end(input), std::back_inserter(output), unary_op);
    }

    template<class InputIt, class UnaryFunction>
    inline UnaryFunction for_each(InputIt& input, UnaryFunction f) {
        return std::for_each(std::begin(input), std::end(input), f);
    }

    std::vector<std::string>& split(const std::string& string, char delimeter, std::vector<std::string>& itemsOut) {
        std::stringstream stream{ string };
        std::string temp;
        while(std::getline(stream, temp, delimeter)) {
            itemsOut.push_back(temp);
        }
        return itemsOut;
    }
}