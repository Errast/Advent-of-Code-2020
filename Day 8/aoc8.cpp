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

struct base_operation {
    const int arg{ };
    base_operation(int arg) : arg{ arg } { }
    virtual ~base_operation() { }

    virtual void execute(int& instrPos, int& acc) const = 0;

    virtual operations getOp() const = 0;
};

struct operation_nop : base_operation {
    operation_nop(int arg) : base_operation(arg) { }

    void execute(int& instrPos, int& acc) const override {
        instrPos++;
    }

    operations getOp() const override {
        return operations::op_nop;
    }
};

struct operation_acc : base_operation {
    operation_acc(int arg) : base_operation(arg) { }

    void execute(int& instrPos, int& acc) const override {
        acc += arg;
        instrPos++;
    }

    operations getOp() const override {
        return operations::op_acc;
    }
};

struct operation_jmp : base_operation {
    operation_jmp(int arg) : base_operation(arg) { }

    void execute(int& instrPos, int& acc) const override {
        instrPos += arg;
    }

    operations getOp() const override {
        return operations::op_jmp;
    }
};

base_operation* parseOp(std::string str) {
    size_t argSeparator{ str.find(' ') };
    std::string opName{ str.substr(0, argSeparator) };
    int arg{ std::stoi(str.substr(argSeparator + 1)) };
    if(opName == "acc")
        return new operation_acc{ arg };
    if(opName == "jmp")
        return new operation_jmp{ arg };
    
    return new operation_nop{ arg };
}

bool infiniteLoop(std::vector<base_operation*>& program, int& instrPos, int& acc) {
    const size_t programLength{ program.size() };
    // because std::vector<bool> is a thing
    std::vector<int> visited( programLength );
    while(instrPos < programLength) {
        if(visited[instrPos])
            return true;
        visited[instrPos] = true;
        program[instrPos]->execute(instrPos, acc);
    }

    return false;
}

void fixInfiniteLoop(std::vector<base_operation*>& program, int& instrPos, int& acc) {
    for(auto& instr : program) {
        if(instr->getOp() != operations::op_nop && instr->getOp() != operations::op_jmp) {
            continue;
        }

        bool isJmp{ instr->getOp() == operations::op_jmp };
        auto oldInstr{ instr };
        instr = isJmp ? static_cast<base_operation*>(new operation_nop(oldInstr->arg))
                      : static_cast<base_operation*>(new operation_jmp(oldInstr->arg));
        instrPos = acc = 0;
        if(!infiniteLoop(program, instrPos, acc)) {
            return;
        }
        delete instr;
        instr = oldInstr;
    }
}

int main() {
    std::ifstream input{ "8 - input.txt" };
    std::vector<std::string> strings{ };
    for(std::string line; std::getline(input, line);) {
        strings.emplace_back(line);
    }

    std::vector<base_operation*> program{ };
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