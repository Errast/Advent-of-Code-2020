#include <fstream>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <string>

using bag = std::vector<std::pair<std::string, int>>;

// this is fucking stupid
struct field_reader: std::ctype<char> {

    field_reader(): std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const* get_table() {
        static std::vector<std::ctype_base::mask> 
            rc(table_size, std::ctype_base::mask());

        rc['\n'] = std::ctype_base::space;
        return &rc[0];
    }
};

constexpr size_t CONTAIN_LENGTH = sizeof(" contain ") - 1;
void parseBag(const std::string& string, std::map<std::string, bag>& map) {
    size_t colorSeparatorIndex{ string.find(" contain ") };
    std::string color{ string.substr(0, colorSeparatorIndex - 1) };

    bag tempBag{ };
    if(string.substr(colorSeparatorIndex + CONTAIN_LENGTH) != "no other bags.") {
        size_t prevIndex{ colorSeparatorIndex + CONTAIN_LENGTH};
        size_t currentIndex;
        while((currentIndex = string.find(", ", prevIndex)) != std::string::npos) {
            size_t numIndex{ string.find(" ", prevIndex) };
            std::string stringNumBags{ string.substr(prevIndex, numIndex - prevIndex) };
            int numBags{ std::stoi(stringNumBags) };
            std::string bagColor;
            if(numBags == 1)
                bagColor = string.substr(numIndex + 1, currentIndex - numIndex - 1);
            else
                bagColor = string.substr(numIndex + 1, currentIndex - numIndex - 2);

            tempBag.push_back({ bagColor, numBags });
            prevIndex = currentIndex + 2;
        }

        currentIndex = string.size();
        size_t numIndex{ string.find(" ", prevIndex) };
        std::string stringNumBags{ string.substr(prevIndex, numIndex - prevIndex) };
        int numBags{ std::stoi(stringNumBags) };
        std::string bagColor;
        if(numBags == 1)
            bagColor = string.substr(numIndex + 1, currentIndex - numIndex - 2);
        else
            bagColor = string.substr(numIndex + 1, currentIndex - numIndex - 3);
        tempBag.push_back({ bagColor, numBags });
    }

    map[color] = tempBag;
}

// includes original bag in count
int totalDepth(std::string color, 
               std::map<std::string, bag>& map, 
               std::map<std::string, int>& cache = std::map<std::string, int>{ }) {
    if(cache.find(color) != cache.end()) {
        return cache[color];
    }

    int count{ 1 };
    for(auto bag : map[color]) {
        count += bag.second * totalDepth(bag.first, map, cache);
    }

    // std::cout << color << " " << count << '\n';
    cache[color] = count;
    return count;
}

bool eventuallyContains(std::string color,
                        std::string target,
                        std::map<std::string, bag>& map, 
                        std::map<std::string, bool>& cached) {
    if(cached.find(color) != cached.end()) {
        return cached[color];
    }

    if(color == target){
        cached[color] = true;
        return true;
    }

    // to prevent infinite loops
    cached[color] = false;
    for(auto bags : map[color]) {
        if(eventuallyContains(bags.first, target, map, cached)) {
            cached[color] = true;
            return true;
        }
    }
    return false;
}

int countContainers(std::string color, std::map<std::string, bag>& map) {
    std::map<std::string, bool> cached{ };
    int count{ 0 };
    for(auto pair : map) {
        if(pair.first != color && eventuallyContains(pair.first, color, map, cached))
            count++;
    }

    return count;
}

int main() {
    std::ifstream input{ "7 - input.txt" };
    input.imbue(std::locale(std::locale(), new field_reader()));
    std::istream_iterator<std::string> start { input }, end { };

    std::map<std::string, bag> bags{ };
    std::for_each(start, end, [&](std::string str){ parseBag(str, bags); });

    // part 1
    std::cout << countContainers("shiny gold bag", bags) << '\n';

    // part 2
    std::cout << totalDepth("shiny gold bag", bags) - 1 << '\n'; 
}