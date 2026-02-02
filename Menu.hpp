#ifndef MENU_HPP
#define MENU_HPP

#include <iostream>
#include <string>
#include <cctype>

#include "Simulation.hpp"
#include "Report.hpp"
#include "Robot.hpp"
#include "Map.hpp"

class Menu {
public:

    int choice;

    int printmenu() {
        std::cout << "\n==============================\n";
        std::cout << "   Self-Cleaning Robot Sim\n";
        std::cout << "==============================\n\n";
        if (map.getMapName() == "NIL") {
            std::cout << "Current map: (none)\n";  // No map loaded
        } 
        else if (map.getStartingPos() == "NIL"){
            std::cout << "Current map: " << map.getMapName() << "\n";
            std::cout << "Starting position: " << map.getStartingPos() << "\n";
        }
        else{
            std::cout << "Current map: " << map.getMapName() << "\n";
            std::cout << "Starting position: " << map.getStartingPos() << "\n";
            std::cout << "Robot Mode: " << robot.getMode() << "\n\n";
            std::cout << "Battery remaining: " << robot.getBattery() << "%\n";
            std::cout << "Water remaining: " << robot.getWater() << "%\n";
            }
        
            int maxchoice;
        if (map.getMapName() == "NIL") {
            std::cout << "\n1) Load Map (.txt)\n";
            maxchoice = 1;
        } else if (map.getStartingPos() == "NIL") {
            std::cout << "\n1) Load New Map (.txt)\n";
            std::cout << "2) Choose Starting Point\n";
            maxchoice = 2;
        } else {
            std::cout << "1) Load New Map (.txt)\n";
            std::cout << "2) Choose Starting Point\n";
            std::cout << "3) Choose Robot Mode\n";
            std::cout << "4) Show Map\n";
            std::cout << "5) Auto Run Simulation\n";
            std::cout << "6) Step Through Simulation\n";
            std::cout << "7) Restart Simulation\n";
            std::cout << "8) Show Cleaning Report\n";
            std::cout << "9) Save Cleaning Report (.txt)\n\n";
            maxchoice = 9;
        }   
            

        std::cout << "0) Exit\n";
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;
        
        if (choice < 0 || choice > maxchoice) {
            choice = -1; // Invalid choice
        }
        return choice;
    }

    void handleChoice(int choice) {
        switch (choice) {
            case 1: loadMap(); 
            break;

            case 2: chooseStartingPoint();  
            break;

            case 3: chooseRobotMode();
            break;

            case 4: ; 
            break;

            case 5: ; 
            break;

            case 6:; 
            break;
            
            case 7: ; 
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

    void loadMap() {
        std::string filename;
        std::cout << "Enter the filename to load the map: ";
        std::cin >> filename; // Set the current map filename
        map.setMapName(filename);  // Update the map filename
        std::cout << "Loading map: " << filename << "\n";
    }

    void chooseRobotMode() {
        int mode;
        std::cout << "Choose Robot Mode: \n1) Explore\n2) Vacuum\n3) Mop\n";
        std::cout << "\nEnter your choice: ";
        std::cin >> mode;
        switch (mode) {
            case 1: robot.setMode("Explore"); break;
            case 2: robot.setMode("Vacuum"); break;
            case 3: robot.setMode("Mop"); break;
            default: robot.setMode("Explore"); break; // Default to "Explore"
        }
        std::cout << "You selected mode: " << robot.getMode() << "\n";
    }

    void chooseStartingPoint() {
        int startPoint;
        std::cout << "Choose Starting Point: \n1) Top Left\n2) Top Right\n3) Bottom Left\n4) Bottom Right\n";
        std::cout << "\nEnter your choice: ";
        std::cin >> startPoint;
        switch (startPoint) {
            case 1: map.setStartingPos("Top Left"); break;
            case 2: map.setStartingPos("Top Right"); break;
            case 3: map.setStartingPos("Bottom Left"); break;
            case 4: map.setStartingPos("Bottom Right"); break;
            default: map.setStartingPos("(none)"); break;  // Default
        }
        std::cout << "You selected starting point: " << map.getStartingPos() << "\n";
    }
};

#endif // MENU_HPP
