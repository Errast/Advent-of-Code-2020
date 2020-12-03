#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <iostream>

// a tree is marked as "true"

constexpr int PATTERN_WIDTH = 31;
using slope_map = std::vector<std::bitset<PATTERN_WIDTH>>;

std::bitset<PATTERN_WIDTH> rowToBitset(std::string row, char treeChar = '#') {
    std::bitset<PATTERN_WIDTH> bitset { };
    for(int i { 0 }; i < row.length(); i++) {
        bitset[i] = row.at(i) == treeChar;
    }

    return bitset;
}

int treesTillBottom(const slope_map &map, int slopeRight, int slopeDown, int startPosX = 0, int startPosY = 0) {
    int treeCount { 0 };
    
    for(int xPos { startPosX }, yPos { startPosY }; yPos < map.size(); xPos = (xPos + slopeRight) % PATTERN_WIDTH, yPos += slopeDown) {
        if(map[yPos][xPos])
            treeCount++;
    }

    return treeCount;
}

int main() {
    std::ifstream input { "3 - input.txt" };
    std::istream_iterator<std::string> start { input }, end { };
    std::vector<std::string> rows { start, end };

    slope_map map { };
    std::transform(rows.begin(), rows.end(), std::back_inserter(map), ::rowToBitset);

    // part 1
    int part1 { treesTillBottom(map, 3, 1) };
    std::cout << part1 << '\n';

    // part 2
    std::cout << treesTillBottom(map, 1, 1)
                * part1
                * treesTillBottom(map, 5, 1)
                * treesTillBottom(map, 7, 1)
                * treesTillBottom(map, 1, 2) << '\n';
}