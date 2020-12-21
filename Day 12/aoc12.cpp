#include "..\Global\my_utils.cpp"

inline void correctAngle(int& angle) {
    angle %= 360;
    if(angle < 0)
        angle += 360;
}

    // Thinking of it like a graph:
    // north/south is y
    // east/west is x
    // 0 degrees is towards positive x

class MovingBase {
    protected:
    int angle{ 0 };

    int east_pos{ 0 };

    int north_pos{ 0 };
    
    public:
    MovingBase() { };

    int GetEastPos() const { return east_pos; }

    int GetNorthPos() const { return north_pos; }

    std::pair<int, int> GetCoords() const { return {east_pos, north_pos }; }

    int GetManhattanDistance() const { return std::abs(east_pos) + std::abs(north_pos); }

    virtual void ActionN(int amount) = 0;

    virtual void ActionS(int amount) = 0;

    virtual void ActionE(int amount) = 0;

    virtual void ActionW(int amount) = 0;

    virtual void ActionL(int amount) = 0;

    virtual void ActionR(int amount) = 0;

    virtual void ActionF(int amount) = 0;


    virtual ~MovingBase() { }
};

class Ship : public MovingBase {
    private:
    int angle{ 0 };

    public:
    Ship() { };

    int GetAngle() const { return angle; }

    virtual void ActionN(int amount) override { north_pos += amount; }

    virtual void ActionS(int amount) override { north_pos -= amount; }

    virtual void ActionE(int amount) override { east_pos += amount; }

    virtual void ActionW(int amount) override { east_pos -= amount; }

    virtual void ActionL(int amount) override { angle += amount; correctAngle(angle); }

    virtual void ActionR(int amount) override { angle -= amount; correctAngle(angle); }
    
    virtual void ActionF(int amount) override {
        switch(angle % 360) {
            case 0: east_pos += amount; break;
            case 90: north_pos += amount; break;
            case 180: east_pos -= amount; break;
            case 270: north_pos -= amount; break;
        }
    }
};

class Waypoint : public MovingBase {
    private:
    MovingBase* anchor;

    public:
    Waypoint(MovingBase* anchorObject) : anchor{ anchorObject } {
        east_pos = 10;
        north_pos = 1;
    }

    MovingBase* GetAnchor() const { return anchor; }

    virtual void ActionN(int amount) override { north_pos += amount; }
    
    virtual void ActionS(int amount) override { north_pos -= amount; }
    
    virtual void ActionE(int amount) override { east_pos += amount; }
    
    virtual void ActionW(int amount) override { east_pos -= amount; }
    
    virtual void ActionL(int amount) override { 
        correctAngle(amount);
        switch(amount) {
            case 90:
                std::swap(east_pos, north_pos); 
                east_pos *= -1;
                break;
            case 180:
                north_pos *= -1;
                east_pos *= -1;
                break;
            case 270:
                std::swap(east_pos, north_pos); 
                north_pos *= -1;
                break;
        }
    }

    virtual void ActionR(int amount) override { ActionL(-amount); }

    virtual void ActionF(int amount) override {
        anchor->ActionN(north_pos * amount);
        anchor->ActionE(east_pos * amount);
    }
};

void processAction(const std::string& action, MovingBase& ship) {
    if(action.size() >= 2) {
        int amount{ std::stoi(action.substr(1)) };
        switch(action.at(0)) {
            case 'N': ship.ActionN(amount); break;
            case 'S': ship.ActionS(amount); break;
            case 'E': ship.ActionE(amount); break;
            case 'W': ship.ActionW(amount); break;
            case 'L': ship.ActionL(amount); break;
            case 'R': ship.ActionR(amount); break;
            case 'F': ship.ActionF(amount); break;
        }
    }
}

int main() {
    auto actions{ mine::getData<std::string>("12 - input.txt") };

    // Part 1
    Ship ship1{ };
    mine::for_each(actions, [&](const std::string& str){ processAction(str, ship1); });    
    std::cout << ship1.GetManhattanDistance() << '\n';

    // Part 2
    Ship ship2{ };
    Waypoint waypoint{ &ship2 };
    mine::for_each(actions, [&](const std::string& str){ processAction(str, waypoint); });    
    std::cout << ship2.GetManhattanDistance() << '\n';
}