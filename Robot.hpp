#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <iostream>
#include <string>
using namespace std;

#include "Menu.hpp"
#include "Simulation.hpp"
#include "Report.hpp"
#include "Map.hpp"

class Robot {
public:
    Robot() : water(100), battery(100), mode("Explore") {}

    void moveRight();
    void moveLeft();

    //getters methods
    int getWater() const { return water; }
    int getBattery() const { return battery; }
    string getMode() const { return mode; }

    //setters methods
    void setWater(int w) { water = w; }
    void setBattery(int b) { battery = b; }
    void setMode(const string& m) { mode = m; }

private:
   int water;
   int battery;
   string mode;
};

#endif // ROBOT_HPP
