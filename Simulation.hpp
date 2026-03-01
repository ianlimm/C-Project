#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <iostream>
#include <string>
#include <vector>
#include "Menu.hpp"
#include "Report.hpp"
#include "Robot.hpp"
#include "Map.hpp"

using namespace std;

class Simulation {
public:
     int determineLR() {
        // Check if left cell is a wall/obstacle
        if (startingposy > 0 && gridmap[startingposx][startingposy - 1] == '#' || gridmap[startingposx][startingposy - 1] == 'O') {
            cout << "Left side has a wall/obstacle. Moving right instead.\n";
            return 1;  // Move right
        }
        // Check if right cell is a wall/obstacle  
        else if (startingposy + 1 < gridmap[0].size() && gridmap[startingposx][startingposy + 1] == '#' || gridmap[startingposx][startingposy + 1] == 'O') {
            cout << "Right side has a wall/obstacle. Moving left instead.\n";
            return -1;  // Move left
        }
        else {
            // No walls on either side, can move freely
            cout << "No walls detected. Moving forward.\n";
            return 0;
        }
    }
    
private:
    vector<vector<char>> gridmap;
    int startingposx;
    int startingposy;
    string movement;
};

#endif // SIMULATION_HPP
