#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <map>

namespace mine {
    template<typename T>
    std::vector<T> getData(const char* fileName) {
        std::ifstream input{ fileName };
        const std::istream_iterator<int> start{ input };
        const std::istream_iterator<int> end{ };
        return std::vector<T>{ start, end };
    }

    constexpr int64_t factorial(int64_t num) {
        if(num < 0) {
            throw std::invalid_argument{ "int factorial(int num): num cannot be negative" };
        }

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
}