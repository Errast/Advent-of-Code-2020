#include <fstream>
#include <iostream>
#include <bitset>
#include <vector>
#include <numeric>

using answer = std::bitset<256>;
using group_answers = std::vector<answer>;

answer collapseGroupOr(const group_answers& group) {
    answer collapsed{ };
    for(auto answer : group) {
        collapsed |= answer;
    }

    return collapsed;
}

answer collapseGroupAnd(const group_answers& group) {
    answer collapsed{ };
    collapsed.set();
    for(auto answer : group) {
        collapsed &= answer;
    }

    return collapsed;
}

void parseAnswers(std::ifstream& input, std::vector<group_answers>& list) {
    char currentChar{ 0 };
    group_answers tempGroupAnswers{ };
    answer tempAnswer{ };
    bool previousNewline{ false };
    while(input.get(currentChar)) {
        if(currentChar == '\n') {
            if(previousNewline) {
                list.push_back(tempGroupAnswers);
                tempGroupAnswers = group_answers{ };
            } else {
                tempGroupAnswers.push_back(tempAnswer);
            }
            tempAnswer = answer{ };
            previousNewline = true;
            continue;
        }

        tempAnswer.set(currentChar);
        previousNewline = false;
    }
    tempGroupAnswers.push_back(tempAnswer);
    list.push_back(tempGroupAnswers);
}

int main() {
    std::ifstream input{ "6 - input.txt" };

    std::vector<group_answers> answers{ };
    parseAnswers(input, answers);

    // part 1
    int sum{ std::accumulate(answers.begin(), answers.end(), 0,
        [](const int& acc, const group_answers& group){ return acc + collapseGroupOr(group).count(); }) };
    std::cout << sum << '\n';

    // part 2
    int sum2{ std::accumulate(answers.begin(), answers.end(), 0,
        [](const int& acc, const group_answers& group){ return acc + collapseGroupAnd(group).count(); }) };
    std::cout << sum2 << '\n';
}