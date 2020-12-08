#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "../Global/my_utils.cpp"

struct operation {
    int arg{ };
    operation(int arg) : arg{ arg } { }

    virtual void execute(int& instrPos, int& acc) = 0;
};

struct op_nop : operation {
    op_nop(int arg) : operation( arg ) { }

    void execute(int& instrPos, int& acc) override {
        instrPos++;
    }
};

struct op_acc : operation {
    op_acc(int amount) : operation( arg ) { }
    
    void execute(int& instrPos, int& acc) override {
        acc += arg;
        instrPos++;
    }
};

struct op_jmp : operation {
    op_jmp(int offset) : operation( arg ) { }

    void execute(int& instrPos, int& acc) override {
        instrPos += arg;
    }
};

operation* parseOp(std::string str) {
    operation* op{ };
    size_t argSeparator{ str.find(' ') };
    std::string opName{ str.substr(0, argSeparator) };
    int arg{ std::stoi(str.substr(argSeparator + 1)) };
    if(opName == "nop")
        op = new op_nop{ arg };
    else if(opName == "acc")
        op = new op_acc{ arg };
    else if(opName == "jmp")
        op = new op_jmp{ arg };

    return op;
}

bool infiniteLoop(const std::vector<operation*>& program, int& instrPos, int& acc) {
    const int programLength{ program.size() };
    // because std::vector<bool> is a thing
    std::vector<int> visited( programLength );
    while(instrPos < programLength) {
        if(visited[instrPos])
            return false;
        visited[instrPos] = true;
        program[instrPos]->execute(instrPos, acc);
    }

    return true;
}

int main() {
    std::ifstream input{ "8 - input.txt" };
    input.imbue(std::locale(std::locale{ }, new field_reader{ }));
    std::istream_iterator<std::string> start{ input };
    std::istream_iterator<std::string> end{ };
    std::vector<std::string> strings{ start, end };

    std::vector<operation*> program{ };
    std::transform(strings.begin(), strings.end(), std::back_inserter(program), parseOp);

    // part 1
    int instrPos{ 0 };
    int acc{ 0 };
    infiniteLoop(program, instrPos, acc);
    std::cout << acc;
}