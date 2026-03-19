#include "Map.hpp"
#include "Cell.hpp"
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

namespace {
    int heuristic(const Point& a, const Point& b) {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }

    struct AStarNode {
        Point p;
        int g;
        int f;
    };

    struct CompareNode {
        bool operator()(const AStarNode& a, const AStarNode& b) const {
            return a.f > b.f;
        }
    };

    void clearGrid(vector<vector<Cell*>>& grid) {
        for (auto& row : grid) {
            for (Cell* c : row) {
                delete c;
            }
        }
        grid.clear();
    }

    string describeSymbol(char c) {
        if (c == ' ') return "[SPACE]";
        return string(1, c);
    }
}

Map::~Map() {
    clearGrid(grid);
}

bool Map::hasInvalidSymbolDetails() const {
    return invalidLine > 0 && invalidColumn > 0 && invalidSymbol != '\0' && !lastFilename.empty();
}

bool Map::hasEmptyLineDetails() const {
    return emptyLineNumber > 0 && !lastFilename.empty();
}

bool Map::load(const string& filename) {
    ifstream file(filename);

    lastLoadError.clear();
    lastFilename = filename;

    invalidLine = -1;
    invalidColumn = -1;
    invalidSymbol = '\0';
    emptyLineNumber = -1;

    if (!file.is_open()) {
        lastLoadError = "FILE COULD NOT BE OPENED.";
        return false;
    }

    vector<vector<Cell*>> newGrid;
    string line;
    size_t expectedCols = 0;
    bool hasWalkableCell = false;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            clearGrid(newGrid);
            emptyLineNumber = lineNumber;
            lastLoadError = "LINE " + to_string(lineNumber) + " IS EMPTY. THE MAP MUST NOT CONTAIN EMPTY LINES.";
            return false;
        }

        if (expectedCols == 0) {
            expectedCols = line.size();
            if (expectedCols == 0) {
                clearGrid(newGrid);
                lastLoadError = "THE MAP IS EMPTY.";
                return false;
            }
        } else if (line.size() != expectedCols) {
            clearGrid(newGrid);
            lastLoadError = "LINE " + to_string(lineNumber) +
                            " HAS A DIFFERENT LENGTH. THE MAP MUST BE RECTANGULAR.";
            return false;
        }

        vector<Cell*> row;
        row.reserve(line.size());

        for (size_t col = 0; col < line.size(); ++col) {
            char c = line[col];
            Cell* cell = nullptr;

            if (c == '#') {
                cell = new Wall();
            }
            else if (c == 'X') {
                cell = new Obstacle();
            }
            else if (c == 'O') {
                cell = new Dirt();
                hasWalkableCell = true;
            }
            else if (c == ' ') {
                cell = new Empty();
                hasWalkableCell = true;
            }
            else {
                clearGrid(newGrid);
                invalidLine = lineNumber;
                invalidColumn = static_cast<int>(col) + 1;
                invalidSymbol = c;

                lastLoadError = "INVALID SYMBOL '" + describeSymbol(c) +
                                "' FOUND AT LINE " + to_string(lineNumber) +
                                ", COLUMN " + to_string(static_cast<int>(col) + 1) +
                                ". ONLY #, X, O, AND [SPACE] ARE ALLOWED.";
                return false;
            }

            row.push_back(cell);
        }

        newGrid.push_back(row);
    }

    if (newGrid.empty()) {
        clearGrid(newGrid);
        lastLoadError = "THE MAP IS EMPTY.";
        return false;
    }

    if (!hasWalkableCell) {
        clearGrid(newGrid);
        lastLoadError = "THE MAP HAS NO WALKABLE CELL. ADD O OR [SPACE] TO CREATE A VALID PATH AREA.";
        return false;
    }

    clearGrid(grid);
    grid = std::move(newGrid);
    rows = static_cast<int>(grid.size());
    cols = (rows > 0) ? static_cast<int>(grid[0].size()) : 0;

    lastLoadError.clear();
    invalidLine = -1;
    invalidColumn = -1;
    invalidSymbol = '\0';
    emptyLineNumber = -1;

    return true;
}

bool Map::rewriteInvalidSymbolInFile(char replacement) {
    if (!hasInvalidSymbolDetails()) {
        return false;
    }

    vector<string> lines;
    ifstream inFile(lastFilename);

    if (!inFile.is_open()) {
        return false;
    }

    string line;
    while (getline(inFile, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    inFile.close();

    if (invalidLine < 1 || invalidLine > static_cast<int>(lines.size())) {
        return false;
    }

    string& targetLine = lines[invalidLine - 1];

    if (invalidColumn < 1 || invalidColumn > static_cast<int>(targetLine.size())) {
        return false;
    }

    targetLine[invalidColumn - 1] = replacement;

    ofstream outFile(lastFilename, ios::trunc);
    if (!outFile.is_open()) {
        return false;
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        outFile << lines[i];
        if (i + 1 < lines.size()) {
            outFile << '\n';
        }
    }

    outFile.close();
    return true;
}

bool Map::rewriteEmptyLineInFile() {
    if (!hasEmptyLineDetails()) {
        return false;
    }

    vector<string> lines;
    ifstream inFile(lastFilename);

    if (!inFile.is_open()) {
        return false;
    }

    string line;
    while (getline(inFile, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    inFile.close();

    if (emptyLineNumber < 1 || emptyLineNumber > static_cast<int>(lines.size())) {
        return false;
    }

    lines.erase(lines.begin() + (emptyLineNumber - 1));

    ofstream outFile(lastFilename, ios::trunc);
    if (!outFile.is_open()) {
        return false;
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        outFile << lines[i];
        if (i + 1 < lines.size()) {
            outFile << '\n';
        }
    }

    outFile.close();
    return true;
}

int Map::countWalkableCells() const {
    int count = 0;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (grid[y][x]->isWalkable()) {
                count++;
            }
        }
    }

    return count;
}

int Map::countReachableWalkableCellsFrom(Point start) const {
    if (start.x < 0 || start.x >= cols || start.y < 0 || start.y >= rows) {
        return 0;
    }

    if (!grid[start.y][start.x]->isWalkable()) {
        return 0;
    }

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    queue<Point> q;
    q.push(start);
    visited[start.y][start.x] = true;

    int reachableCount = 0;
    const int dx[4] = {0, 0, 1, -1};
    const int dy[4] = {1, -1, 0, 0};

    while (!q.empty()) {
        Point current = q.front();
        q.pop();
        reachableCount++;

        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (ny < 0 || ny >= rows || nx < 0 || nx >= cols) {
                continue;
            }

            if (visited[ny][nx]) {
                continue;
            }

            if (!grid[ny][nx]->isWalkable()) {
                continue;
            }

            visited[ny][nx] = true;
            q.push({nx, ny});
        }
    }

    return reachableCount;
}

int Map::countUnreachableWalkableCellsFrom(Point start) const {
    int totalWalkable = countWalkableCells();
    int reachableWalkable = countReachableWalkableCellsFrom(start);
    return totalWalkable - reachableWalkable;
}

void Map::resetMap() {
    for (auto& row : grid) {
        for (Cell* c : row) {
            c->reset();
        }
    }
}

void Map::display(Point rp, char icon, string color, bool showRobot) {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (showRobot && x == rp.x && y == rp.y) {
                cout << color << icon << "\033[0m";
            } else {
                cout << grid[y][x]->getSymbol();
            }
        }
        cout << endl;
    }
}

bool Map::isValid(int x, int y) {
    if (y < 0 || y >= rows || x < 0 || x >= cols) return false;
    return grid[y][x]->isWalkable();
}

bool Map::isDirty(Point p) {
    return (p.y >= 0 && p.y < rows && p.x >= 0 && p.x < cols)
        ? grid[p.y][p.x]->isDirty()
        : false;
}

void Map::cleanCell(Point p) {
    if (p.y >= 0 && p.y < rows && p.x >= 0 && p.x < cols) {
        grid[p.y][p.x]->clean();
    }
}

vector<Point> Map::getPathHome(Point start, Point goal) {
    vector<Point> path;

    if (!isValid(start.x, start.y) || !isValid(goal.x, goal.y)) {
        return path;
    }

    if (start == goal) {
        return path;
    }

    priority_queue<AStarNode, vector<AStarNode>, CompareNode> openSet;
    map<Point, Point> parent;
    map<Point, int> gScore;
    map<Point, bool> closed;

    gScore[start] = 0;
    openSet.push({start, 0, heuristic(start, goal)});

    const int dx[4] = {0, 0, 1, -1};
    const int dy[4] = {1, -1, 0, 0};

    while (!openSet.empty()) {
        AStarNode current = openSet.top();
        openSet.pop();

        if (closed[current.p]) {
            continue;
        }
        closed[current.p] = true;

        if (current.p == goal) {
            Point curr = goal;
            while (!(curr == start)) {
                path.push_back(curr);
                curr = parent[curr];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; ++i) {
            Point next = {current.p.x + dx[i], current.p.y + dy[i]};

            if (!isValid(next.x, next.y) || closed[next]) {
                continue;
            }

            int tentativeG = gScore[current.p] + 1;

            if (!gScore.count(next) || tentativeG < gScore[next]) {
                parent[next] = current.p;
                gScore[next] = tentativeG;
                int fScore = tentativeG + heuristic(next, goal);
                openSet.push({next, tentativeG, fScore});
            }
        }
    }

    return path;
}