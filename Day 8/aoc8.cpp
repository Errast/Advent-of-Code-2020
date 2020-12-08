#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../Global/my_utils.cpp"

enum class operations {
    op_nop,
    op_acc,
    op_jmp
};

struct operation {
    int arg{ };
    operations op;
    operation(operations op, int arg) : arg{ arg }, op{ op } { }

    void execute(int& instrPos, int& acc) {
        switch(op) {
            case operations::op_nop:
                instrPos++;
                break;
            case operations::op_acc:
                acc += arg;
                instrPos++;
                break;
            case operations::op_jmp:
                instrPos += arg;
        }
    }
};

operation parseOp(std::string str) {
    size_t argSeparator{ str.find(' ') };
    std::string opName{ str.substr(0, argSeparator) };
    int arg{ std::stoi(str.substr(argSeparator + 1)) };
    if(opName == "acc")
        return operation{ operations::op_acc, arg };
    if(opName == "jmp")
        return operation{ operations::op_jmp, arg };
    
    return operation{ operations::op_nop, arg };
}

bool infiniteLoop(std::vector<operation>& program, int& instrPos, int& acc) {
    const size_t programLength{ program.size() };
    // because std::vector<bool> is a thing
    std::vector<int> visited( programLength );
    while(instrPos < programLength) {
        if(visited[instrPos])
            return true;
        visited[instrPos] = true;
        program[instrPos].execute(instrPos, acc);
    }

    return false;
}

void fixInfiniteLoop(std::vector<operation>& program, int& instrPos, int& acc) {
    for(auto& instr : program) {
        if(instr.op != operations::op_nop && instr.op != operations::op_jmp) {
            continue;
        }

        bool isJmp{ instr.op == operations::op_jmp };
        // if(isJmp && instr.arg > 0) {
        //     continue;
        // }

        instr.op = isJmp ? operations::op_nop : operations::op_jmp;
        instrPos = acc = 0;
        if(!infiniteLoop(program, instrPos, acc)) {
            return;
        }
        instr.op = instr.op = isJmp ? operations::op_jmp : operations::op_nop;
    }
}

int main() {
    std::ifstream input{ "8 - input.txt" };
    input.imbue(std::locale(std::locale{ }, new field_reader{ }));
    std::istream_iterator<std::string> start{ input };
    std::istream_iterator<std::string> end{ };
    std::vector<std::string> strings{ start, end };

    std::vector<operation> program{ };
    std::transform(strings.begin(), strings.end(), std::back_inserter(program), parseOp);

    // part 1
    int instrPos{ 0 };
    int acc{ 0 };
    infiniteLoop(program, instrPos, acc);
    std::cout << acc << '\n';

    // part 2
    fixInfiniteLoop(program, instrPos, acc);
    std::cout << acc << '\n';
}