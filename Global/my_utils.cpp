#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <map>
#include <algorithm>

namespace mine {
    template<typename T>
    std::vector<T> getData(const char* fileName) {
        std::ifstream input{ fileName };
        const std::istream_iterator<T> start{ input };
        const std::istream_iterator<T> end{ };
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

    template<class InputIt, class OutputIt, class UnaryOperation>
    inline std::back_insert_iterator<OutputIt> transform(const InputIt& input, OutputIt& output, UnaryOperation unary_op) {
        return std::transform(std::begin(input), std::end(input), std::back_inserter(output), unary_op);
    }

    template<class InputIt, class UnaryFunction>
    inline UnaryFunction for_each(const InputIt& input, UnaryFunction f) {
        return std::for_each(std::begin(input), std::end(input), f);
    }
}