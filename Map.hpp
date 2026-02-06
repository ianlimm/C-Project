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
        cout << "Enter the map name: ";
        cin >> mapname; // Set the current map filename  // Update the map filename
        cout << "Numbers of Rows (Excluding Borders): ";
        cin >> maprows;
        cout << "Numbers of Columns (Excluding Borders): "; 
        cin >> mapcols;
        cout << "Number of Obstacles: ";
        cin >> totobstacles;
        cout << "Number of Dirt Patches: ";
        cin >> totdirt;
        cout << "Number of Stains: ";
        cin >> totstains;

        if (maprows < 1) maprows = 1;
        if (mapcols < 1) mapcols = 1;

        int totalRows = maprows + 2;
        int totalCols = mapcols + 2;
        int interiorCells = maprows * mapcols;

        // Clamp counts to fit
        if (totdirt  < 0) totdirt  = 0;
        if (totstains < 0) totstains = 0;
        if (totobstacles  < 0) totobstacles  = 0;

        int totalItems = totdirt + totstains + totobstacles;
        if (totalItems > interiorCells) {
            int remaining = interiorCells;

            totobstacles = min(totobstacles, remaining);
            remaining -= totobstacles;

            totstains = min(totstains, remaining);
            remaining -= totstains;

            totdirt = min(totdirt, remaining);
        }

        // Create empty grid ('.' = empty)
        vector<vector<char>> gridmap(totalRows, vector<char>(totalCols, '.'));

        // Borders '#'
        for (int r = 0; r < totalRows; r++) {
            gridmap[r][0] = '#';
            gridmap[r][totalCols - 1] = '#';
        }
        for (int c = 0; c < totalCols; c++) {
            gridmap[0][c] = '#';
            gridmap[totalRows - 1][c] = '#';
        }

        // List all interior cells
        vector<pair<int,int>> cells;
        cells.reserve(interiorCells);
        for (int r = 1; r <= maprows; r++) {
            for (int c = 1; c <= mapcols; c++) {
                cells.push_back({r, c});
            }
        }

        // Shuffle and place items
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

        for (int i = 0; i < totalRows; i++) {
            for (int j = 0; j < totalCols; j++) {
                cout << gridmap[i][j];
        }
        cout << endl;
      }
        
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
    int maprows;          // Number of rows in the map
    int mapcols;          // Number of columns in the map
    int totdirt;         // Total dirt in the map
    int totstains;       // Total stains in the map
    int totobstacles;    // Total obstacles in the map


    bool fileExists(const string& path) {
    namespace fs = filesystem;
    return fs::exists(path) && fs::is_regular_file(path);
    }   
    
    void loadarrayfromfile() {
        
    }
    
};

#endif // MAP_HPP
