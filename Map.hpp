#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <random>
#include <algorithm>
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
    void setStartingPos(const string& pos) { 
        startingPos = pos; 
        setstartingposonmap(); // Update the map with the new starting position
    }

    void loadmap() {
        cout << "Enter the filename to load the map: ";
        cin >> mapname; // Set the current map filename  // Update the map filename
        cout << "Finding " << mapname << " ... ... ...\n";
        if (fileExists(mapname)) {
            cout << "Map file found successfully!\n";
            clearGrid();
            loadarrayfromfile();
            if (startingPos != "NIL") {
                setstartingposonmap();
            }
            
        } else {
            cout << "Error: Map file not found. Please check the filename and try again.\n";
            mapname = "NIL";
            return;
        }
        
    }

    void generatemap() {
        int inputRows, inputCols;
        cout << "Enter the map name: ";
        cin >> mapname;
        cout << "Numbers of Rows (Excluding Borders): ";
        cin >> inputRows;
        cout << "Numbers of Columns (Excluding Borders): "; 
        cin >> inputCols;
        
        // Ensure input is valid
        if (inputRows < 1) inputRows = 1;
        if (inputCols < 1) inputCols = 1;

        cout << "Number of Obstacles: "; cin >> totobstacles;
        cout << "Number of Dirt Patches: "; cin >> totdirt;
        cout << "Number of Stains: "; cin >> totstains;

        // Set actual member dimensions including borders
        maprows = inputRows + 2;
        mapcols = inputCols + 2;
        int interiorCells = inputRows * inputCols;

        // Clamp items to ensure they fit in the interior
        int totalItems = totdirt + totstains + totobstacles;
        if (totalItems > interiorCells) {
            totobstacles = min(totobstacles, interiorCells);
            totstains = min(totstains, interiorCells - totobstacles);
            totdirt = min(totdirt, interiorCells - totobstacles - totstains);
        }

        clearGrid();
        gridmap.assign(maprows, vector<char>(mapcols, '.'));
        if (startingPos != "NIL") {
                setstartingposonmap();
            }

        // Borders '#'
        for (int r = 0; r < maprows; r++) {
            gridmap[r][0] = '#';
            gridmap[r][mapcols - 1] = '#';
        }
        for (int c = 0; c < mapcols; c++) {
            gridmap[0][c] = '#';
            gridmap[maprows - 1][c] = '#';
        }

        // List ONLY interior cells (Avoid index 0 and index maprows-1)
        vector<pair<int,int>> cells;
        cells.reserve(interiorCells);
        for (int r = 1; r <= inputRows; r++) { // Fixed: Use inputRows, not maprows
            for (int c = 1; c <= inputCols; c++) { // Fixed: Use inputCols, not mapcols
                cells.push_back({r, c});
            }
        }

        // Random placement logic
        random_device rd;
        mt19937 rng(rd());
        shuffle(cells.begin(), cells.end(), rng);

        int idx = 0;
        for (int i = 0; i < totobstacles; i++, idx++)
            gridmap[cells[idx].first][cells[idx].second] = 'O'; 

        for (int i = 0; i < totstains; i++, idx++)
            gridmap[cells[idx].first][cells[idx].second] = '@';

        for (int i = 0; i < totdirt; i++, idx++)
            gridmap[cells[idx].first][cells[idx].second] = '!';
}

    void printMap() {
        cout << "Loading map: " << mapname << "\n";
    
     // Display
        for (int i = 0; i < maprows; i++) {
            for (int j = 0; j < mapcols; j++) {
                if(i == currentPosY && j == currentPosX) {
                    cout << 'R'; // Display the robot's current position
                } else {
                cout << gridmap[i][j];
                }
            }
            cout << endl;
        }
    }
    


 private:
    string mapname; // Name of the map
    string startingPos;  // starting position of the robot
    int currentPosX;    // Current X position of the robot
    int currentPosY;    // Current Y position of the robot
    int maprows;          // Number of rows in the map
    int mapcols;          // Number of columns in the map
    int totdirt;         // Total dirt in the map
    int totstains;       // Total stains in the map
    int totobstacles;    // Total obstacles in the map
    vector<vector<char>> gridmap; //Gridmap for Map


    bool fileExists(const string& path) {
    namespace fs = filesystem;
    return fs::exists(path) && fs::is_regular_file(path);
    }   

    void clearGrid() {
        gridmap.clear();
    }
    
    void loadarrayfromfile() {
        ifstream file(mapname);
        if (!file) return;

        gridmap.clear(); // Ensure we don't append to old map data
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            vector<char> row;
            for (char c : line) {
                // Skips spaces/newlines to ensure only map characters are stored
                if (!isspace(c)) {
                    row.push_back(c);
                }
            }
            if (!row.empty()) gridmap.push_back(row);
        }
        file.close();

        // Dynamically update map dimensions based on the file content
        maprows = gridmap.size();
        mapcols = gridmap[0].size(); 
    }

    void setstartingposonmap() {
        bool notwall = false;
        if (startingPos == "Top Left") {
            currentPosX = 1;
            currentPosY = 1;
            while(!notwall){
                if (gridmap[currentPosY][currentPosX] == '#' || gridmap[currentPosY][currentPosX] == 'O') {
                    currentPosX++;
                } else {
                    notwall = true;
                }
              
            }
        } else if (startingPos == "Top Right") {
            currentPosX = 1;
            currentPosY = mapcols - 2;
            while(!notwall){
                if (gridmap[currentPosY][currentPosX] == '#' || gridmap[currentPosY][currentPosX] == 'O') {
                    currentPosX--;
                } else {
                    notwall = true;
                }
              
            }
        } else if (startingPos == "Bottom Left") {
            currentPosX = maprows - 2;
            currentPosY = 1;
            while(!notwall){
                if (gridmap[currentPosY][currentPosX] == '#' || gridmap[currentPosY][currentPosX] == 'O') {
                    currentPosX++;
                } else {
                    notwall = true;
                }
              
            }
        } else if (startingPos == "Bottom Right") {
            currentPosX = maprows - 2;
            currentPosY = mapcols - 2;
            while(!notwall){
                if (gridmap[currentPosY][currentPosX] == '#' || gridmap[currentPosY][currentPosX] == 'O') {
                    currentPosX--;
                } else {
                    notwall = true;
                }
              
            }
        } 

    }
    
};

#endif // MAP_HPP
