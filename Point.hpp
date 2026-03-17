#ifndef POINT_HPP
#define POINT_HPP
#include <string>

using namespace std;

struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point& other) const { return !(*this == other); }
    bool operator<(const Point& other) const { 
        return y < other.y || (y == other.y && x < other.x);
    }
};

struct SimulationLog {
    int runNumber;
    string mapName;
    string startPoint;
    int energyUsed;
    int dirtCleaned;
};
#endif