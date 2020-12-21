#include "..\Global\my_utils.cpp"
#include <bitset>

constexpr int mem_bits = 36;

class SeaComputer {
    public:
    uint64_t maskBitsSet;
    uint64_t mask;
    std::map<uint64_t, uint64_t> memory;

    SeaComputer() : mask{ }, memory{ }, maskBitsSet{ } { }

    void instrMem(const std::string& line) {
        auto addrBegin{ line.find('[') + 1 };
        auto addrEnd{ line.find(']') };
        auto valBegin{ line.find('=') + 1 };
        uint64_t address{ static_cast<uint64_t>(std::stoll(line.substr(addrBegin, addrEnd - addrBegin))) };
        uint64_t value{ static_cast<uint64_t>(std::stoll(line.substr(valBegin))) };

        // Karnaugh map told me to do it ¯\_(ツ)_/¯
        memory[address] = (mask & maskBitsSet) | (value & ~maskBitsSet);
    }

    void instrMask(const std::string& line) {
        size_t valBegin{ line.find('=') + 1 };
        for(size_t i{ valBegin }, bit{ mem_bits - 1 }; i < line.size() && bit < mem_bits; ++i) {
            switch(line[i]) {
                case '0':
                    mask &= ~(1ui64 << bit);
                    maskBitsSet |= (1ui64 << bit);
                    --bit; 
                    break;
                case '1':
                    mask |= 1ui64 << bit;
                    maskBitsSet |= (1ui64 << bit);
                    --bit;
                    break;
                case 'X':
                    maskBitsSet &= ~(1ui64 << bit);
                    --bit;
                    break;
            }
        }
    }
};

int main() {
    std::bitset<64> f;
    auto lines{ mine::getLines("14 - input.txt") };
    SeaComputer thing{ };
    for(auto& line : lines) {
        // C++20 has an actual string::starts_with method
        if(line.rfind("mask", 0) == 0) {
            thing.instrMask(line);
        } else if(line.rfind("mem", 0) == 0) {
            thing.instrMem(line);
        }
    }
    
    // Part 1
    uint64_t sum{ 0 };
    std::for_each(thing.memory.begin(), thing.memory.end(), [&](auto value){ sum += value.second; });
    std::cout << sum << '\n';
}