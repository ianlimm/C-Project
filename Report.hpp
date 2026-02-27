#ifndef REPORT_HPP
#define REPORT_HPP

#include <iostream>
#include <string>
using namespace std;

#include "Menu.hpp"
#include "Simulation.hpp"
#include "Robot.hpp"
#include "Map.hpp"

class Report { 
public:
    void showFinalReport() {

// robot.getBatteryUsed(); // calculate initial battery - currentbatt
// waterused = robot.getWaterUsed(); // calculate initial water - currentwater
// robot.getStatus(); // "Cleaning Completed", "Dead Battery", "User Termination"
// status - completed (exploredarea == totalmapsize), deadbattery (if batterylife =0), user termination (user exit or user ctrl c)

// map.gettotalmapsize(); map size should only be available tiles (run a loop to count tiles that is marked clean or dirty)

    //cout << "======================";
    //cout << "Robot Cleaning Report";
    //cout << "======================" << endl;
    //cout << "Mission Status: " << robot.getStatus() << endl;
    //cout << "- Battery Used: " << robot.getBatteryUsed() << "%" << endl;
    //cout << "- Water Used:   " << robot.getWaterUsed() << "ml" << endl << endl;
    
    //cout << "-- Coverage --" <<endl;
    //cout << "Dirt Patches Removed: " << robot.getdirtcleared() << endl;
    //cout << "Stains Cleaned: " << robot.getstainscleaned << endl;
    //cout << "Total Area: " << robot.getexploredarea() / map.gettotalmapsize() << endl;

    cout << endl << "======================" <<endl;
    cout << "Robot Cleaning Report" << endl;
    cout << "======================" << endl <<endl;
    cout << "> Mission Status  : " << "<robot.getStatus()>" << endl;
    cout << "> Battery Used    : " << "<robot.getBatteryUsed()>" "%" << endl;
    cout << "> Water Used      : " << "<robot.getWaterUsed()>" << "ml" << endl << endl;
    
    cout << "> Coverage" <<endl;
    cout << "Dirt Patches Removed  : " << "<robot.getdirtcleared()>" << endl;
    cout << "Stains Cleaned        : " << "<robot.getstainscleaned>" << endl;
    cout << "Total Area            : " << "<robot.getexploredarea()> / <map.gettotalmapsize()> units" << endl << endl;

    }
private:

};

#endif // REPORT_HPP
