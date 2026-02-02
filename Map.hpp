#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <string>
using namespace std;

#include "Menu.hpp"
#include "Simulation.hpp"
#include "Report.hpp"
#include "Robot.hpp"

class Map {
public:
    Map () : mapname("NIL"), startingPos("NIL") {}    

    //getters methods
    string getMapName() const { return mapname; }  
    string getStartingPos() const { return startingPos; }

    //setters methods
    void setMapName(const string& name) { mapname = name; }
    void setStartingPos(const string& pos) { startingPos = pos; }

private:
    string mapname; // Name of the map
    string startingPos;    // starting position of the robot
    int currentPosX;    // Current X position of the robot
    int currentPosY;    // Current Y position of the robot
};

#endif // MAP_HPP
