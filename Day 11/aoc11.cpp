#include "..\Global\my_utils.cpp"
#include <algorithm>
#include <functional>
#include <memory>
#include <execution>
#include <iostream>
#include <array>


enum class seat_status {
    not_seat,
    empty,
    occupied,
    unknown
};

enum direction {
    dir_up,
    dir_up_right,
    dir_right,
    dir_down_right,
    dir_down,
    dir_down_left,
    dir_left,
    dir_up_left,
    dir_last
};

using seat_row = std::vector<seat_status>;
using plane_state = std::vector<seat_row>;
using sim_rule = std::function<seat_status(int, int, const plane_state&)>;

std::string toString(const plane_state& plane) {
    std::string str{ };
    for(auto& row : plane) {
        for(auto seat : row) {
            char c;
            switch(seat){ 
                case seat_status::not_seat: c = '.'; break;
                case seat_status::empty:    c = 'L'; break;
                case seat_status::occupied: c = '#'; break;
            }
            str.push_back(c);
        }
        str.push_back('\n');
    }

    return str;
}

seat_row makeRow(std::string str) {
    seat_row row{ };
    for(const char c : str) {
        switch(c) {
            case '.': row.emplace_back(seat_status::not_seat); break;
            case 'L': row.emplace_back(seat_status::empty); break;
            case '#': row.emplace_back(seat_status::occupied); break;
            default: row.emplace_back(seat_status::unknown); break;
        }
    }

    return row;
}

int advanceSeatSim(
        const plane_state& prevState, const std::vector<sim_rule>& rules, plane_state& newStateOut) {
    plane_state newState{ prevState };

    int changes{ 0 };
    for(int i{ 0 }; i < prevState.size(); ++i) {
        for(int j{ 0 }; j < prevState[i].size(); ++j) {
            for(auto rule : rules) {
                newState[i][j] = rule(i, j, prevState);
                // Stop after the first change
                if(newState[i][j] != prevState[i][j]) {
                    ++changes;
                    break;
                }
            }
        }
    }

    newStateOut = std::move(newState);
    return changes;
}

int runSeatSim(const plane_state& prevState, const std::vector<sim_rule>& rules, plane_state& finalSimOut) {
    int changes{ 1 };
    plane_state thing{ prevState };
    int count{ 0 };
    while(changes != 0) {
        // std::cout << toString(thing) << '\n';
        changes = advanceSeatSim(thing, rules, thing);
        ++count;
    }
    finalSimOut = std::move(thing);
    return count;
}

#define RULE_LAMBDA [](int row, int col, const plane_state& prevState)
const std::vector<sim_rule> default_rules{ 
    // empty -> occupied rule
    RULE_LAMBDA { 
        if(prevState[row][col] == seat_status::empty) {
            for(int i{ std::max(row - 1, 0) }; i <= row + 1 && i < prevState.size(); ++i) {
                for(int j{ std::max(col - 1, 0) }; j <= col + 1 && j < prevState[i].size(); ++j) {
                    if(i == row && j == col) {
                        continue;
                    }

                    if(prevState[i][j] == seat_status::occupied) {
                        return seat_status::empty;
                    }
                }
            }
            return seat_status::occupied;
        }
        return prevState[row][col];
    },
    // occupied -> empty rule
    RULE_LAMBDA {
        if(prevState[row][col] == seat_status::occupied) {
            int occupiedCount{ 0 };
            for(int i{ std::max(row - 1, 0) }; i <= row + 1 && i < prevState.size(); ++i) {
                for(int j{ std::max(col - 1, 0) }; j <= col + 1 && j < prevState[i].size(); ++j) {
                    if(i == row && j == col) {
                        continue;
                    }

                    if(prevState[i][j] == seat_status::occupied) {
                        ++occupiedCount;
                    }
                }
            }

            if(occupiedCount >= 4) {
                return seat_status::empty;
            }
            return seat_status::occupied;
        }
        return (prevState[row][col]);
    }
};

inline void advanceDirection(int& coordX, int& coordY, direction dir) {
    switch(dir) {
        case dir_up: --coordY; break;
        case dir_up_right: --coordY; ++coordX; break;
        case dir_right: ++coordX; break;
        case dir_down_right: ++coordY; ++coordX; break;
        case dir_down: ++coordY; break;
        case dir_down_left: ++coordY; --coordX; break;
        case dir_left: --coordX; break;
        case dir_up_left: --coordX; --coordY; break;
    }
}

const std::vector<sim_rule> part2_rules {
    // empty -> occupied rule
    RULE_LAMBDA {
        if(prevState[row][col] == seat_status::empty) {
            for(int dir{ dir_up }; dir != dir_last; ++dir) {
                for(int i{ row }, j{ col }; i < prevState.size() && j < prevState[i].size(); advanceDirection(j, i, static_cast<direction>(dir))) {
                    if(i == row && j == col) {
                        continue;
                    }

                    if(prevState[i][j] == seat_status::occupied) {
                        return seat_status::empty;
                    }

                    if(prevState[i][j] != seat_status::not_seat) {
                        break;
                    }
                }
            }
            return seat_status::occupied;
        }
        return prevState[row][col];
    },
    // occupied -> empty rule
    RULE_LAMBDA {
        if(prevState[row][col] == seat_status::occupied) {
            int occupiedCount{ 0 };
            for(int dir{ dir_up }; dir != dir_last; ++dir) {
                for(int i{ row }, j{ col }; i < prevState.size() && j < prevState[i].size(); advanceDirection(j, i, static_cast<direction>(dir))) {
                    if(i == row && j == col) {
                        continue;
                    }

                    if(prevState[i][j] == seat_status::occupied) {
                        ++occupiedCount;
                    }
                    
                    if(prevState[i][j] != seat_status::not_seat) {
                        break;
                    }
                }
            }
            if(occupiedCount >= 5) {
                return seat_status::empty;
            }
            return seat_status::occupied;
        }
        return prevState[row][col];
    }
};

int countSeatsEqual(const plane_state& plane, seat_status status) {
    int count{ 0 };
    for(auto& row : plane) {
        for(auto seat : row) {
            if(seat == status) {
                ++count;
            }
        }
    }
    return count;
}

int main() {
    auto input{ mine::getData<std::string>("11 - input.txt") };
    plane_state seats{ };
    mine::transform(input, seats, makeRow);
    // part 1
    plane_state finalState;
    // runSeatSim(seats, default_rules, finalState);
    // std::cout << countSeatsEqual(finalState, seat_status::occupied);

    // part 2
    runSeatSim(seats, part2_rules, finalState);
    std::cout << countSeatsEqual(finalState, seat_status::occupied);
}