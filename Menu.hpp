#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

#include "Simulation.hpp"
#include "Report.hpp"
#include "Robot.hpp"
#include "Map.hpp"

class Menu {
public:

    int choice;

    int printmenu() {
        cout << "\n==============================\n";
        cout << "   Self-Cleaning Robot Sim\n";
        cout << "==============================\n\n";
        if (map.getMapName() == "NIL") {
            cout << "Current map: (none)\n";  // No map loaded
        } 
        else if (map.getStartingPos() == "NIL"){
            cout << "Current map: " << map.getMapName() << "\n";
            cout << "Starting position: " << map.getStartingPos() << "\n";
        }
        else{
            cout << "Current map: " << map.getMapName() << "\n";
            cout << "Starting position: " << map.getStartingPos() << "\n";
            cout << "Robot Mode: " << robot.getMode() << "\n\n";
            cout << "Battery remaining: " << robot.getBattery() << "%\n";
            cout << "Water remaining: " << robot.getWater() << "%\n\n";
            }
        
            int maxchoice;
        if (map.getMapName() == "NIL") {
            cout << "\n1) Load Map (.txt)\n";
            maxchoice = 1;
        } else if (map.getStartingPos() == "NIL") {
            cout << "\n1) Load New Map (.txt)\n";
            cout << "2) Choose Starting Point\n";
            maxchoice = 2;
        } else {
            cout << "1) Load New Map (.txt)\n";
            cout << "2) Choose Starting Point\n";
            cout << "3) Choose Robot Mode\n";
            cout << "4) Show Map\n";
            cout << "5) Auto Run Simulation\n";
            cout << "6) Step Through Simulation\n";
            cout << "7) Restart Simulation\n";
            cout << "8) Show Cleaning Report\n";
            cout << "9) Save Cleaning Report (.txt)\n\n";
            maxchoice = 9;
        }   
            

        cout << "0) Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        if (choice < 0 || choice > maxchoice) {
            choice = -1; // Invalid choice
        }
        return choice;
    }

    void handleChoice(int choice) {
        switch (choice) {
            case 1: loadorgenerate(); 
            break;

            case 2: chooseStartingPoint();  
            break;

            case 3: chooseRobotMode();
            break;

            case 4: map.printMap(); 
            break;

            case 5: ; 
            break;

            case 6:; 
            break;
            
            case 7: restartSimulation(); 
            break;

            case 8: ; 
            break;

            case 9: ; 
            break;
        }
    }
private:
    Map map;
    Robot robot;

    void loadorgenerate() {
        int option;
        cout << "1) Load Map \n2) Generate Map\n";
        cout << "\nEnter your choice: ";
        cin >> option;
        switch (option) {
            case 1: map.loadmap(); break;
            case 2: map.generatemap(); break;
            default: cout << "Invalid option. Returning to main menu.\n"; break;
        }
    }

    void chooseRobotMode() {
        int mode;
        cout << "Choose Robot Mode: \n1) Explore\n2) Vacuum\n3) Mop\n";
        cout << "\nEnter your choice: ";
        cin >> mode;
        switch (mode) {
            case 1: robot.setMode("Explore"); break;
            case 2: robot.setMode("Vacuum"); break;
            case 3: robot.setMode("Mop"); break;
            default: robot.setMode("Explore"); break; // Default to "Explore"
        }
        cout << "You selected mode: " << robot.getMode() << "\n";
    }

    void chooseStartingPoint() {
        int startPoint;
        cout << "Choose Starting Point: \n1) Top Left\n2) Top Right\n3) Bottom Left\n4) Bottom Right\n";
        cout << "\nEnter your choice: ";
        cin >> startPoint;
        switch (startPoint) {
            case 1: map.setStartingPos("Top Left"); break;
            case 2: map.setStartingPos("Top Right"); break;
            case 3: map.setStartingPos("Bottom Left"); break;
            case 4: map.setStartingPos("Bottom Right"); break;
            default: map.setStartingPos("(none)"); break;  // Default
        }
        cout << "You selected starting point: " << map.getStartingPos() << "\n";
    }

    void restartSimulation() {
        map.setMapName("NIL");
        map.setStartingPos("NIL");
        robot.setBattery(100);
        robot.setWater(100);
        cout << "Simulation restarted. Battery and water levels reset to 100%. Please Load in a new map\n";
    }
};

#endif // MENU_HPP
