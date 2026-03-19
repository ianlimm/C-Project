#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>
#include <map>
#include "Point.hpp"

using namespace std;

class Cell;

class Map {
private:
    vector<vector<Cell*>> grid;
    int rows = 0;
    int cols = 0;

    string lastLoadError;
    string lastFilename;

    int invalidLine = -1;
    int invalidColumn = -1;
    char invalidSymbol = '\0';

    int emptyLineNumber = -1;

public:
    ~Map();

    bool load(const string& filename);
    void display(Point robotPos, char icon, string color, bool showRobot);
    void resetMap();
    bool isValid(int x, int y);
    bool isDirty(Point p);
    void cleanCell(Point p);
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    vector<Point> getPathHome(Point start, Point goal);

    string getLastLoadError() const { return lastLoadError; }

    bool hasInvalidSymbolDetails() const;
    int getInvalidLine() const { return invalidLine; }
    int getInvalidColumn() const { return invalidColumn; }
    char getInvalidSymbol() const { return invalidSymbol; }
    bool rewriteInvalidSymbolInFile(char replacement);

    bool hasEmptyLineDetails() const;
    int getEmptyLineNumber() const { return emptyLineNumber; }
    bool rewriteEmptyLineInFile();

    int countWalkableCells() const;
    int countReachableWalkableCellsFrom(Point start) const;
    int countUnreachableWalkableCellsFrom(Point start) const;
};

#endif