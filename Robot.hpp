#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <string>
#include "Point.hpp"

using namespace std;

class RobotMode {
public:
    virtual int getCost() = 0;
    virtual string getColor() = 0;
    virtual string getName() = 0;
    virtual ~RobotMode() {}
};

class DefaultMode : public RobotMode {
public:
    int getCost() override { return 2; }
    string getColor() override { return "\033[32m"; }
    string getName() override { return "default mode"; }
};

class HighPowerMode : public RobotMode {
public:
    int getCost() override { return 3; }
    string getColor() override { return "\033[31m"; }
    string getName() override { return "high power mode"; }
};

class NavigationMode : public RobotMode {
public:
    int getCost() override { return 1; }
    string getColor() override { return "\033[34m"; }
    string getName() override { return "navigation mode"; }
};

class Robot {
public:
    Point pos{0, 0};
    Point startPos{0, 0};
    char icon = '^';
    int energyUsed = 0;
    int dirtCleaned = 0;
    RobotMode* mode;

    Robot() : mode(new DefaultMode()) {}
    ~Robot() { delete mode; }

    void reset(Point start) {
        pos = start;
        startPos = start;
        icon = '^';
        energyUsed = 0;
        dirtCleaned = 0;
        setMode(new DefaultMode());
    }

    void setMode(RobotMode* newMode) {
        if (mode != nullptr) {
            delete mode;
        }
        mode = newMode;
    }

    void updateIcon(Point oldPos, Point newPos) {
        if (newPos.x > oldPos.x) icon = '>';
        else if (newPos.x < oldPos.x) icon = '<';
        else if (newPos.y > oldPos.y) icon = 'v';
        else if (newPos.y < oldPos.y) icon = '^';
    }
};

#endif