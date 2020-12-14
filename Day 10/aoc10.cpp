#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "../Global/my_utils.cpp"

struct diff_counts {
    int one{ };
    int two{ };
    int three{ };
    // Includes all "extr" adapters with this joltage. The real number of adapters with this joltage is one plus this
    int zero{ };

    inline bool isAllZero() const {
        return zero == 0
               && one == 0
               && two == 0
               && three == 0;
    }
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

diff_counts countDiffs(const std::vector<int>& diffs) {
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

std::map<int, diff_counts> makeDiffTree(const std::vector<int>& adapters) {
    std::map<int, diff_counts> map{ };
    std::vector<int> sorted{ 0 };
    sorted.insert(sorted.end(), adapters.begin(), adapters.end());
    std::sort(sorted.begin(), sorted.end());

    for(int i{ 0 }; i < sorted.size(); i++) {
        diff_counts count{ };
        for(int j{ i + 1 }, diff{ -1 };
            j < sorted.size() && (diff = sorted[j] - sorted[i]) <= 3;
            j++) {
            switch(diff) {
                case 0:  ++count.zero; ++i;  break;
                case 1:  ++count.one;   break;
                case 2:  ++count.two;   break;
                case 3:  ++count.three; break;
            }
        }
        // if(!count.isAllZero()) {
        map[sorted[i]] = count;
        // }
    }
    
    return map;
}

// This deals with multiple adapters with the same joltage
// Actually, I know the number gets bigger if you add a duplicate,
// but I haven't bothered to figure out if it's the right number
int64_t countPathsTo(int root,
                 int end,
                 const std::map<int, diff_counts>& map,
                 std::map<std::pair<int, int>, int64_t>& cached = std::map<std::pair<int, int>, int64_t>{ }) {

    if(mine::hasKey({ end, root }, cached)) {
        return cached[{ end, root }];
    }

    int64_t count{ 0 };
    if(root == end) {
        count = 1;
    } else {
        auto diffs{ map.at(root) };
        if(diffs.one != 0) {
            count += mine::factorial(diffs.one) * countPathsTo(root + 1, end, map, cached);
        }
        if(diffs.two != 0) {
            count += mine::factorial(diffs.two) * countPathsTo(root + 2, end, map, cached);
        }
        if(diffs.three != 0) {
            count += mine::factorial(diffs.three) * countPathsTo(root + 3, end, map, cached);
        }
    }

    cached[{end, root}] = count;
    return count;
}

int main() {
    auto adapters{ mine::getData<int>("10 - input.txt") };
    
    // part 1
    auto diffs{ makeDiffs(adapters) };
    auto count{ countDiffs(diffs) };
    std::cout << count.one * count.three << '\n';

    // part 2
    auto diffTree{ makeDiffTree(adapters) };
    std::cout << countPathsTo(0, *std::max_element(adapters.begin(), adapters.end()), diffTree) << '\n';
}