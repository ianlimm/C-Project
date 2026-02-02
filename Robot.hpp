#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <iostream>
#include <string>

#include "Menu.hpp"
#include "Simulation.hpp"
#include "Report.hpp"
#include "Map.hpp"

class Robot {
public:
    Robot() : water(100), battery(100), mode("Explore") {}

    //getters methods
    int getWater() const { return water; }
    int getBattery() const { return battery; }
    std::string getMode() const { return mode; }

    //setters methods
    void setWater(int w) { water = w; }
    void setBattery(int b) { battery = b; }
    void setMode(const std::string& m) { mode = m; }

private:
   int water;
   int battery;
   std::string mode;
};

#endif // ROBOT_HPP
