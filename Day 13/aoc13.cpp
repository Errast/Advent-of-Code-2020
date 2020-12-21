#include "..\Global\my_utils.cpp"

struct BusDeparture {
    int id{ -1 };
    int offset{ -1 };
};



int main() {
    auto input{ mine::getData<std::string>("13 - input.txt") };
    int timestamp{ std::stoi(input[0]) };
    std::stringstream busStream{ input[1] };
    std::vector<int> busses{ };
    std::string temp;
    while(std::getline(busStream, temp, ',')) {
        if(temp != "x") {
            busses.push_back(std::stoi(temp));
        } else {
            busses.push_back(-1);
        }
    }

    // Part 1
    std::vector<BusDeparture> nextBusTime{ };
    mine::transform(busses, nextBusTime, [&](int busId){ int d = timestamp % busId; return BusDeparture{busId, d == 0 ? 0 : busId - d};});
    auto earliestBus{ *std::min_element(nextBusTime.begin(), nextBusTime.end(), [](BusDeparture a, BusDeparture b){ return a.offset < b.offset && a.id != -1; }) };
    std::cout << earliestBus.id * earliestBus.offset << '\n';

    // Part 2
    std::vector<BusDeparture> actualBusses{ };
    for(int i{ 0 }; i < busses.size(); ++i) {
        if(busses[i] != -1) {
            actualBusses.push_back({ busses[i], i });
        }
    }
    uint64_t time{ 1 };
    uint64_t incrAmount{ 1 };
    auto currentBus{ actualBusses.begin() };
    while(true) {
        if((time + currentBus->offset) % currentBus->id != 0) {
            time += incrAmount;
        } else {
            incrAmount *= currentBus->id;
            ++currentBus;
            if(currentBus == actualBusses.end()) {
                break;
            }
        }
    }

    std::cout << time << '\n';
    for(auto thing : actualBusses) {
        std::cout << thing.id << '\t' << thing.offset << '\t' << time + thing.offset << '\t' << (time + thing.offset) % thing.id << '\n';
    }
}