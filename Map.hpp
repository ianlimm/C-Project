#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <string>

#include "Menu.hpp"
#include "Simulation.hpp"
#include "Report.hpp"
#include "Robot.hpp"

class Map {
public:
    Map () : mapname("NIL"), startingPos("NIL") {}    

    //getters methods
    std::string getMapName() const { return mapname; }  
    std::string getStartingPos() const { return startingPos; }

    //setters methods
    void setMapName(const std::string& name) { mapname = name; }
    void setStartingPos(const std::string& pos) { startingPos = pos; }

private:
    std::string mapname; // Name of the map
    std::string startingPos;    // starting position of the robot
    int currentPosX;    // Current X position of the robot
    int currentPosY;    // Current Y position of the robot
};

#endif // MAP_HPP
