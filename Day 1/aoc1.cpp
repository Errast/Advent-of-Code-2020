#include <fstream>
#include <vector>
#include <iostream>

static constexpr int SUM = 2020;

int getProductOfSums(std::vector<int> numbers, int numNumbers, int sum = SUM, int startAt = 0, int currentSum = 0) {
    if(numNumbers == 0)
        return currentSum == 0 ? 1 : -1;

    for(int i = 0; i < numbers.size(); i++) {
        if(int result = getProductOfSums(numbers, numNumbers - 1, sum - numbers[i], i, numbers[i] + currentSum); result != -1) {
            return numbers[i] * result;
        }
    }
}

int main(){
    std::ifstream input { "1 - input.txt"};
    std::istream_iterator<int> start { input }, end { };
    std::vector<int> numbers { start, end };
    int size = numbers.size();

    std::cout << getProductOfSums(numbers, 2) << '\n';
    std::cout << getProductOfSums(numbers, 3) << '\n';

    // part one
    for(int i { 0 }; i < size; i++){
        int iNum = numbers[i];
        for(int j { i }; j < size; j++){
            int jNum = numbers[j];
            if(iNum + jNum == SUM){
                std::cout << iNum << " * " << jNum << " = " << iNum * jNum << '\n';
                goto OutOfLoop;
            }
        }
    }
    OutOfLoop:
    
    // part two
    for(int i { 0 }; i < size; i++){
        int iNum = numbers[i];
        for(int j = { i }; j < size; j++){
            int jNum = numbers[j];
            for(int k = { j }; k < size; k++){
                int kNum = numbers[k];
                if(iNum + jNum + kNum == SUM) {
                    std::cout << iNum << " * " << jNum << " * " << kNum << " = " << iNum * jNum * kNum << '\n';
                    return 0;
                }
            }
        }
    }
}