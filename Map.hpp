#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
using namespace std;

#include "Menu.hpp"
#include "Simulation.hpp"
#include "Report.hpp"
#include "Robot.hpp"

class Map {
public:
    const int ROWS = 20;
    const int COLS = 20;

    Map () : mapname("NIL"), startingPos("NIL") {}    

    //getters methods
    string getMapName() const { return mapname; }  
    string getStartingPos() const { return startingPos; }

    //setters methods
    void setMapName(const string& name) { mapname = name; }
    void setStartingPos(const string& pos) { startingPos = pos; }

    void loadmap() {
        cout << "Enter the filename to load the map: ";
        cin >> mapname; // Set the current map filename  // Update the map filename
        cout << "Finding " << mapname << " ... ... ...\n";
        if (fileExists(mapname)) {
            cout << "Map file found successfully!\n";
            
        } else {
            cout << "Error: Map file not found. Please check the filename and try again.\n";
            mapname = "NIL";
            return;
        }
        cout << "Loading map: " << mapname << "\n";
    }

    void generatemap() {
        cout << "Enter the filename to load the map: ";
        cin >> mapname; // Set the current map filename  // Update the map filename
        cout << "Loading map: " << mapname << "\n";

        
    }

    void printMap() {
        cout << "Loading map: " << mapname << "\n";
        ifstream file(mapname);
        
        //generate array
        char** grid = new char*[ROWS];
     for (int i = 0; i < ROWS; i++) {
        grid[i] = new char[COLS];
        for (int j = 0; j < COLS; j++) {
            file >> grid[i][j];
        }
     }
        file.close();
    
     // Display
     cout << "\n20x20 Environment:" << endl;
     for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << grid[i][j];
        }
        cout << endl;
      }
    }
    


 private:
    string mapname; // Name of the map
    string startingPos;  // starting position of the robot
    int currentPosX;    // Current X position of the robot
    int currentPosY;    // Current Y position of the robot


    bool fileExists(const string& path) {
    namespace fs = filesystem;
    return fs::exists(path) && fs::is_regular_file(path);
    }   
    
    void loadarrayfromfile() {
        
    }
    
};

#endif // MAP_HPP
