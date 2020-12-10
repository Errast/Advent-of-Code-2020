#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../Global/my_utils.cpp"

struct diff_counts {
    int zero{ };
    int one{ };
    int two{ };
    int three{ };
};

std::vector<int> makeDiffs(const std::vector<int>& adapters) {
    auto chainedAdapters{ adapters };
    chainedAdapters.emplace_back(0);
    std::sort(chainedAdapters.begin(), chainedAdapters.end());

    std::vector<int> diffs(chainedAdapters.size());
    for(int i{ 0 }; i < chainedAdapters.size() - 1; ++i) {
        diffs[i] = chainedAdapters[i + 1] - chainedAdapters[i];
    }

    diffs[chainedAdapters.size() - 1] = 3;
    return diffs;
}

diff_counts countDiffs(const std::vector<int>& adapters) {
    std::vector<int> diffs( makeDiffs(adapters) );
    diff_counts count{ };

    for(auto elem : diffs) {
        switch(elem) {
            case 0: ++count.zero;  break;
            case 1: ++count.one;   break;
            case 2: ++count.two;   break;
            case 3: ++count.three; break;
        }
    }

    return count;
}

int main() {
    auto adapters{ getData<int>("10 - input.txt") };
    
    auto count{ countDiffs(adapters) };
    std::cout << count.one * count.three << '\n';
}