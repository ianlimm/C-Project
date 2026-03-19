#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "Map.hpp"
#include "Robot.hpp"
#include <vector>
#include <string>

using namespace std;

class Simulator {
private:
    Map map;
    Robot robot;
    bool mapLoaded = false;
    string currentMapName;
    vector<SimulationLog> history;
    int moveDelayMs = 1000;

    void printHistory();
    void saveHistoryToFile();

public:
    void start();

    Map& getMap() { return map; }
    Robot& getRobot() { return robot; }

    void setMoveDelayMs(int delayMs) { moveDelayMs = delayMs; }
    int getMoveDelayMs() const { return moveDelayMs; }

    void displayCurrentState();
    void cleanCurrentCell();
    void moveRobot(Point next, bool isNavigating);
    void returnToDock();
    void recordHistory(const string& runLabel);
};

#endif