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
            cout << "2) Generate Map\n";
            maxchoice = 2;
        } 
        else if (map.getStartingPos() == "NIL") {
            cout << "\n1) Load New Map (.txt)\n";
            cout << "2) Generate Map (.txt)\n";
            cout << "3) Choose Starting Point\n";
            maxchoice = 3;
        } 
        else {
            cout << "1) Load Map (.txt)\n";
            cout << "2) Generate Map\n";
            cout << "3) Choose Starting Point\n";
            cout << "4) Choose Robot Mode\n";
            cout << "5) Show Map\n";
            cout << "6) Auto Run Simulation\n";
            cout << "7) Step Through Simulation\n";
            cout << "8) Restart Simulation\n";
            cout << "9) Show Cleaning Report\n";
            cout << "10) Save Cleaning Report (.txt)\n\n";
            maxchoice = 10;
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
            case 1: 
                load(); 
                break;

            case 2: 
                generate();  
                break;

            case 3: 
                chooseStartingPoint();  
                break;

            case 4: 
                chooseRobotMode();  // Fixed: Now calls the correct function name
                break;

            case 5: 
                map.printMap(); 
                break;

            case 6: 
                // Auto Run Simulation - To be implemented
                cout << "Auto Run Simulation - Coming soon!\n";
                break;

            case 7:
                // Step Through Simulation - To be implemented
                cout << "Step Through Simulation - Coming soon!\n";
                break;
            
            case 8: 
                restartSimulation(); 
                break;

            case 9: 
                // Show Cleaning Report - To be implemented
                cout << "Show Cleaning Report - Coming soon!\n";
                break;

            case 10: 
                // Save Cleaning Report - To be implemented
                cout << "Save Cleaning Report - Coming soon!\n";
                break;
        }
    }

private:
    Map map;
    Robot robot;

    void load() {
        map.loadmap();
    }

    void generate() {
        map.generatemap();
    }

    void chooseRobotMode() {  // Single, consolidated function
        int mode;
        cout << "\nChoose Robot Mode: \n";
        cout << "1) Explore\n";
        cout << "2) Vacuum\n";
        cout << "3) Mop\n";
        cout << "\nEnter your choice: ";
        cin >> mode;
        
        switch (mode) {
            case 1: 
                robot.setMode("Explore"); 
                cout << "You selected mode: Explore\n";
                break;
            case 2: 
                robot.setMode("Vacuum"); 
                cout << "You selected mode: Vacuum\n";
                break;
            case 3: 
                robot.setMode("Mop"); 
                cout << "You selected mode: Mop\n";
                break;
            default: 
                robot.setMode("Explore"); 
                cout << "Invalid choice. Defaulting to Explore mode.\n";
                break;
        }
    }

    void chooseStartingPoint() {
        int startPoint;
        cout << "\nChoose Starting Point: \n";
        cout << "1) Top Left\n";
        cout << "2) Top Right\n";
        cout << "3) Bottom Left\n";
        cout << "4) Bottom Right\n";
        cout << "\nEnter your choice: ";
        cin >> startPoint;
        
        switch (startPoint) {
            case 1: 
                map.setStartingPos("Top Left"); 
                cout << "You selected starting point: Top Left\n";
                break;
            case 2: 
                map.setStartingPos("Top Right"); 
                cout << "You selected starting point: Top Right\n";
                break;
            case 3: 
                map.setStartingPos("Bottom Left"); 
                cout << "You selected starting point: Bottom Left\n";
                break;
            case 4: 
                map.setStartingPos("Bottom Right"); 
                cout << "You selected starting point: Bottom Right\n";
                break;
            default: 
                map.setStartingPos("(none)"); 
                cout << "Invalid choice. No starting point selected.\n";
                break;
        }
    }

    void restartSimulation() {
        map.setMapName("NIL");
        map.setStartingPos("NIL");
        robot.setBattery(100);
        robot.setWater(100);
        robot.setMode("Explore");  // Reset mode to default
        cout << "\nSimulation restarted.\n";
        cout << "Battery and water levels reset to 100%.\n";
        cout << "Please load a new map.\n";
    }
};

#endif // MENU_HPP