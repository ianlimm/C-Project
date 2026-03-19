#include "Simulator.hpp"
#include "BCDStrategy.hpp"
#include "FrontierStrategy.hpp"
#include "NearestCellStrategy.hpp"

#include <iostream>
#include <fstream>
#include <limits>
#include <thread>
#include <chrono>
#include <memory>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

namespace {
    Point resolveStartPoint(Map& mapObj, int sp) {
        int top = 1;
        int bottom = mapObj.getRows() - 2;
        int left = 1;
        int right = mapObj.getCols() - 2;

        if (sp == 1) {
            for (int x = left; x <= right; ++x) {
                if (mapObj.isValid(x, top)) return {x, top};
            }
            for (int y = top; y <= bottom; ++y) {
                if (mapObj.isValid(left, y)) return {left, y};
            }
        }
        else if (sp == 2) {
            for (int x = right; x >= left; --x) {
                if (mapObj.isValid(x, top)) return {x, top};
            }
            for (int y = top; y <= bottom; ++y) {
                if (mapObj.isValid(right, y)) return {right, y};
            }
        }
        else if (sp == 3) {
            for (int x = left; x <= right; ++x) {
                if (mapObj.isValid(x, bottom)) return {x, bottom};
            }
            for (int y = bottom; y >= top; --y) {
                if (mapObj.isValid(left, y)) return {left, y};
            }
        }
        else if (sp == 4) {
            for (int x = right; x >= left; --x) {
                if (mapObj.isValid(x, bottom)) return {x, bottom};
            }
            for (int y = bottom; y >= top; --y) {
                if (mapObj.isValid(right, y)) return {right, y};
            }
        }

        return {-1, -1};
    }

    string dockName(int sp) {
        if (sp == 1) return "Top Left";
        if (sp == 2) return "Top Right";
        if (sp == 3) return "Bottom Left";
        return "Bottom Right";
    }

    void printReachabilityWarnings(Map& mapObj) {
        bool printedHeader = false;

        for (int sp = 1; sp <= 4; ++sp) {
            Point startPoint = resolveStartPoint(mapObj, sp);

            if (startPoint.x == -1 || startPoint.y == -1) {
                if (!printedHeader) {
                    cout << "\nMAP REACHABILITY CHECK" << endl;
                    printedHeader = true;
                }

                cout << "- " << dockName(sp) << ": no valid dock-adjacent start position found." << endl;
                continue;
            }

            int unreachableCount = mapObj.countUnreachableWalkableCellsFrom(startPoint);

            if (unreachableCount > 0) {
                if (!printedHeader) {
                    cout << "\nMAP REACHABILITY CHECK" << endl;
                    printedHeader = true;
                }

                cout << "- " << dockName(sp)
                     << ": warning, this map contains unreachable walkable cells from the selected dock"
                     << " (" << unreachableCount << " unreachable)." << endl;
            }
        }

        if (!printedHeader) {
            cout << "\nMAP REACHABILITY CHECK" << endl;
            cout << "- All 4 dock options can reach every walkable cell in the map." << endl;
        }
    }

    void setRobotMode(Robot& robot, RobotMode* newMode) {
        string previousMode = robot.mode->getName();
        robot.setMode(newMode);
        string currentMode = robot.mode->getName();

        if (previousMode != currentMode) {
            cout << "MODE CHANGE: " << previousMode << " -> " << currentMode << endl;
        }
    }

    void printModeLegend() {
        cout << "MODE LEGEND" << endl;
        cout << DefaultMode().getColor() << "[DEFAULT MODE]" << "\033[0m"
             << " = Cleaning normal cells" << endl;
        cout << HighPowerMode().getColor() << "[HIGH POWER MODE]" << "\033[0m"
             << " = Cleaning dirty cells" << endl;
        cout << NavigationMode().getColor() << "[NAVIGATION MODE]" << "\033[0m"
             << " = Travelling / repositioning / returning to dock" << endl;
    }

    void printMapSeparator() {
        cout << "\n==================================================\n" << endl;
    }

    int readValidatedChoice(const string& prompt, int minValue, int maxValue) {
        int value;

        while (true) {
            cout << prompt;

            if (!(cin >> value)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "INVALID INPUT. PLEASE ENTER A NUMBER FROM "
                     << minValue << " TO " << maxValue << "." << endl << endl;
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (value < minValue || value > maxValue) {
                cout << "INVALID CHOICE. PLEASE ENTER A NUMBER FROM "
                     << minValue << " TO " << maxValue << "." << endl << endl;
                continue;
            }

            return value;
        }
    }

    char readReplacementSymbol() {
        cout << "Choose replacement symbol:" << endl;
        cout << "1. # = WALL" << endl;
        cout << "2. X = OBSTACLE" << endl;
        cout << "3. O = DIRT" << endl;
        cout << "4. [SPACE] = EMPTY CELL" << endl;

        int choice = readValidatedChoice("\nChoice: ", 1, 4);

        if (choice == 1) return '#';
        if (choice == 2) return 'X';
        if (choice == 3) return 'O';
        return ' ';
    }

    string symbolToDisplay(char c) {
        if (c == ' ') return "[SPACE]";
        return string(1, c);
    }

    string extractStrategyName(const string& startPointField) {
        size_t pos = startPointField.find('|');
        if (pos == string::npos) {
            return startPointField;
        }

        string strategy = startPointField.substr(pos + 1);

        while (!strategy.empty() && strategy.front() == ' ') {
            strategy.erase(strategy.begin());
        }

        while (!strategy.empty() && strategy.back() == ' ') {
            strategy.pop_back();
        }

        return strategy;
    }

    string extractStartPosition(const string& startPointField) {
        size_t pos = startPointField.find('|');
        string start = (pos == string::npos) ? startPointField : startPointField.substr(0, pos);

        while (!start.empty() && start.front() == ' ') {
            start.erase(start.begin());
        }

        while (!start.empty() && start.back() == ' ') {
            start.pop_back();
        }

        return start;
    }
}

void Simulator::displayCurrentState() {
    printMapSeparator();
    map.display(robot.pos, robot.icon, robot.mode->getColor(), true);
    cout << endl;
    printModeLegend();

    if (moveDelayMs > 0) {
        this_thread::sleep_for(chrono::milliseconds(moveDelayMs));
    }
}

void Simulator::cleanCurrentCell() {
    if (map.isDirty(robot.pos)) {
        setRobotMode(robot, new HighPowerMode());
        robot.dirtCleaned++;
    } else {
        setRobotMode(robot, new DefaultMode());
    }

    robot.energyUsed += robot.mode->getCost();
    map.cleanCell(robot.pos);
    displayCurrentState();
}

void Simulator::moveRobot(Point next, bool isNavigating) {
    if (!map.isValid(next.x, next.y)) {
        return;
    }

    if (isNavigating) {
        setRobotMode(robot, new NavigationMode());
    } else {
        if (map.isDirty(next)) {
            setRobotMode(robot, new HighPowerMode());
            robot.dirtCleaned++;
        } else {
            setRobotMode(robot, new DefaultMode());
        }
    }

    robot.updateIcon(robot.pos, next);
    robot.pos = next;
    robot.energyUsed += robot.mode->getCost();

    if (!isNavigating) {
        map.cleanCell(next);
    }

    displayCurrentState();
}

void Simulator::returnToDock() {
    cout << "\nCLEANING DONE. RETURNING TO DOCK..." << endl;

    if (robot.pos == robot.startPos) {
        cout << "ROBOT IS ALREADY AT THE STARTING POSITION." << endl;
        return;
    }

    vector<Point> pathHome = map.getPathHome(robot.pos, robot.startPos);

    for (const Point& p : pathHome) {
        moveRobot(p, true);
    }

    cout << "ROBOT HAS RETURNED TO STARTING POSITION." << endl;
}

void Simulator::recordHistory(const string& runLabel) {
    history.push_back({
        static_cast<int>(history.size()) + 1,
        currentMapName,
        runLabel,
        robot.energyUsed,
        robot.dirtCleaned
    });
}

void Simulator::start() {
    while (true) {
        cout << "\n--- ROBOT CLEANING SIMULATOR ---" << endl;
        cout << "1. Load Map" << endl;

        if (mapLoaded) {
            cout << "2. Show Layout" << endl;
            cout << "3. Run Simulation" << endl;
            cout << "4. History" << endl;
            cout << "5. Save" << endl;
        }

        cout << "0. Exit" << endl;

        int maxChoice = mapLoaded ? 5 : 1;
        int choice = readValidatedChoice("\nChoice: ", 0, maxChoice);

        cout << endl;

        if (choice == 0) {
            break;
        }
        else if (choice == 1) {
            string inputMapName;
            cout << "Enter Filename: ";
            getline(cin, inputMapName);
            cout << endl;

            if (inputMapName.empty()) {
                cout << "FILENAME CANNOT BE EMPTY!" << endl;
                continue;
            }

            while (true) {
                if (map.load(inputMapName)) {
                    currentMapName = inputMapName;
                    mapLoaded = true;
                    cout << "MAP LOADED SUCCESSFULLY." << endl;
                    printReachabilityWarnings(map);
                    break;
                }

                cout << "FAILED TO LOAD MAP." << endl;
                cout << "Reason: " << map.getLastLoadError() << endl;
                cout << "Allowed symbols are:" << endl;
                cout << "  # = WALL" << endl;
                cout << "  X = OBSTACLE" << endl;
                cout << "  O = DIRT" << endl;
                cout << "  [SPACE] = EMPTY CELL" << endl;

                if (map.hasInvalidSymbolDetails()) {
                    cout << "\nDo you want to rewrite the loaded map file?" << endl;
                    cout << "1. Yes" << endl;
                    cout << "2. No" << endl;

                    int rewriteChoice = readValidatedChoice("\nChoice: ", 1, 2);
                    cout << endl;

                    if (rewriteChoice == 2) {
                        cout << "MAP WAS NOT CHANGED. PLEASE LOAD ANOTHER MAP." << endl;
                        break;
                    }

                    cout << "Invalid symbol found: "
                         << symbolToDisplay(map.getInvalidSymbol())
                         << " at line " << map.getInvalidLine()
                         << ", column " << map.getInvalidColumn() << "." << endl;

                    char replacement = readReplacementSymbol();
                    cout << endl;

                    if (map.rewriteInvalidSymbolInFile(replacement)) {
                        cout << "MAP FILE UPDATED SUCCESSFULLY." << endl;
                        cout << "Reloading map..." << endl << endl;
                        continue;
                    } else {
                        cout << "FAILED TO REWRITE THE MAP FILE." << endl;
                        break;
                    }
                }
                else if (map.hasEmptyLineDetails()) {
                    cout << "\nAn empty line was found at line "
                         << map.getEmptyLineNumber() << "." << endl;

                    cout << "Do you want to rewrite the loaded map file and remove this empty line?" << endl;
                    cout << "1. Yes" << endl;
                    cout << "2. No" << endl;

                    int rewriteChoice = readValidatedChoice("\nChoice: ", 1, 2);
                    cout << endl;

                    if (rewriteChoice == 2) {
                        cout << "MAP WAS NOT CHANGED. PLEASE LOAD ANOTHER MAP." << endl;
                        break;
                    }

                    if (map.rewriteEmptyLineInFile()) {
                        cout << "EMPTY LINE REMOVED SUCCESSFULLY." << endl;
                        cout << "Reloading map..." << endl << endl;
                        continue;
                    } else {
                        cout << "FAILED TO REWRITE THE MAP FILE." << endl;
                        break;
                    }
                }
                else {
                    break;
                }
            }
        }
        else if (choice == 2 && mapLoaded) {
            map.resetMap();
            printMapSeparator();
            map.display({-1, -1}, ' ', "", false);
            cout << endl;
            printModeLegend();
        }
        else if (choice == 3 && mapLoaded) {
            cout << "Select Starting Dock:" << endl;
            cout << "1. Top Left" << endl;
            cout << "2. Top Right" << endl;
            cout << "3. Bottom Left" << endl;
            cout << "4. Bottom Right" << endl;

            int sp = readValidatedChoice("\nChoice: ", 1, 4);
            cout << endl;

            string dockLabel;
            if (sp == 1) dockLabel = "Top-Left";
            else if (sp == 2) dockLabel = "Top-Right";
            else if (sp == 3) dockLabel = "Bottom-Left";
            else dockLabel = "Bottom-Right";

            Point startPoint = resolveStartPoint(map, sp);

            if (startPoint.x == -1 || startPoint.y == -1) {
                cout << "NO VALID START POSITION FOUND NEAR THAT CORNER!" << endl;
                continue;
            }

            int unreachableCount = map.countUnreachableWalkableCellsFrom(startPoint);
            if (unreachableCount > 0) {
                cout << "WARNING: THIS MAP CONTAINS UNREACHABLE WALKABLE CELLS FROM THE SELECTED DOCK"
                     << " (" << unreachableCount << " unreachable)." << endl;
            }

            cout << "Select Cleaning Algorithm:" << endl;
            cout << "1. BCD-Style Sweep (Recommended for less complex, open maps)" << endl;
            cout << "2. Frontier-Based Coverage (Recommended for cluttered, irregular maps)" << endl;
            cout << "3. Nearest Uncleaned Cell + A* (Recommended as a balanced middle-ground)" << endl;

            int algoChoice = readValidatedChoice("\nChoice: ", 1, 3);
            cout << endl;

            unique_ptr<CleaningStrategy> strategy;
            if (algoChoice == 1) {
                strategy = make_unique<BCDStrategy>();
            }
            else if (algoChoice == 2) {
                strategy = make_unique<FrontierStrategy>();
            }
            else {
                strategy = make_unique<NearestCellStrategy>();
            }

            cout << "Select Buffer Time:" << endl;
            cout << "1. 0 sec" << endl;
            cout << "2. 0.5 sec" << endl;
            cout << "3. 1 sec" << endl;

            int delayChoice = readValidatedChoice("\nChoice: ", 1, 3);
            cout << endl;

            if (delayChoice == 1) moveDelayMs = 0;
            else if (delayChoice == 2) moveDelayMs = 500;
            else moveDelayMs = 1000;

            robot.reset(startPoint);
            map.resetMap();

            cout << "Simulation starting from " << dockLabel
                 << " using " << strategy->getName()
                 << " with buffer " << (moveDelayMs / 1000.0) << " sec..." << endl;

            string runLabel = dockLabel + " | " + strategy->getName();
            strategy->run(*this, dockLabel, runLabel);
        }
        else if (choice == 4 && mapLoaded) {
            printHistory();
        }
        else if (choice == 5 && mapLoaded) {
            saveHistoryToFile();
        }
    }
}

void Simulator::printHistory() {
    if (history.empty()) {
        cout << "\nNO SIMULATION HISTORY AVAILABLE YET." << endl;
        return;
    }

    const int runWidth = 8;
    const int mapWidth = 24;
    const int startWidth = 28;
    const int energyWidth = 12;
    const int cleanedWidth = 12;

    cout << "\nSIMULATION HISTORY REPORT" << endl;
    cout << "--------------------------------------------------------------------------" << endl;

    cout << left
         << setw(runWidth) << "Run"
         << setw(mapWidth) << "Map"
         << setw(startWidth) << "Start / Algorithm"
         << setw(energyWidth) << "Energy"
         << setw(cleanedWidth) << "Cleaned"
         << endl;

    cout << string(runWidth + mapWidth + startWidth + energyWidth + cleanedWidth, '-') << endl;

    for (const auto& l : history) {
        cout << left
             << setw(runWidth) << l.runNumber
             << setw(mapWidth) << l.mapName
             << setw(startWidth) << l.startPoint
             << setw(energyWidth) << l.energyUsed
             << setw(cleanedWidth) << l.dirtCleaned
             << endl;
    }

    std::map<string, int> bestEnergyByMap;

    for (const auto& l : history) {
        if (!bestEnergyByMap.count(l.mapName) || l.energyUsed < bestEnergyByMap[l.mapName]) {
            bestEnergyByMap[l.mapName] = l.energyUsed;
        }
    }

    cout << "\nBEST EFFICIENCY BY MAP" << endl;
    cout << "--------------------------------------------------------------------------" << endl;

    for (const auto& best : bestEnergyByMap) {
        vector<SimulationLog> bestRuns;

        for (const auto& l : history) {
            if (l.mapName == best.first && l.energyUsed == best.second) {
                bestRuns.push_back(l);
            }
        }

        cout << "Map          : " << best.first << endl;
        cout << "Lowest Energy: " << best.second << endl;
        cout << "Best Run(s)  : ";

        for (size_t i = 0; i < bestRuns.size(); ++i) {
            cout << "[" << bestRuns[i].runNumber << ", "
                 << extractStrategyName(bestRuns[i].startPoint) << ", "
                 << extractStartPosition(bestRuns[i].startPoint) << "]";
            if (i + 1 < bestRuns.size()) {
                cout << ", ";
            }
        }

        cout << endl << endl;
    }
}

void Simulator::saveHistoryToFile() {
    ofstream ofs("simulation_report.txt");

    if (!ofs.is_open()) {
        cout << "FAILED TO SAVE REPORT. COULD NOT OPEN simulation_report.txt" << endl;
        return;
    }

    const int runWidth = 8;
    const int mapWidth = 24;
    const int startWidth = 28;
    const int energyWidth = 12;
    const int cleanedWidth = 12;

    ofs << "SIMULATION HISTORY REPORT\n";
    ofs << "--------------------------------------------------------------------------\n";

    ofs << left
        << setw(runWidth) << "Run"
        << setw(mapWidth) << "Map"
        << setw(startWidth) << "Start / Algorithm"
        << setw(energyWidth) << "Energy"
        << setw(cleanedWidth) << "Cleaned"
        << "\n";

    ofs << string(runWidth + mapWidth + startWidth + energyWidth + cleanedWidth, '-') << "\n";

    if (history.empty()) {
        ofs << "NO SIMULATION HISTORY AVAILABLE YET.\n";
    } else {
        for (const auto& l : history) {
            ofs << left
                << setw(runWidth) << l.runNumber
                << setw(mapWidth) << l.mapName
                << setw(startWidth) << l.startPoint
                << setw(energyWidth) << l.energyUsed
                << setw(cleanedWidth) << l.dirtCleaned
                << "\n";
        }

        std::map<string, int> bestEnergyByMap;

        for (const auto& l : history) {
            if (!bestEnergyByMap.count(l.mapName) || l.energyUsed < bestEnergyByMap[l.mapName]) {
                bestEnergyByMap[l.mapName] = l.energyUsed;
            }
        }

        ofs << "\nBEST EFFICIENCY BY MAP\n";
        ofs << "--------------------------------------------------------------------------\n";

        for (const auto& best : bestEnergyByMap) {
            vector<SimulationLog> bestRuns;

            for (const auto& l : history) {
                if (l.mapName == best.first && l.energyUsed == best.second) {
                    bestRuns.push_back(l);
                }
            }

            ofs << "Map          : " << best.first << "\n";
            ofs << "Lowest Energy: " << best.second << "\n";
            ofs << "Best Run(s)  : ";

            for (size_t i = 0; i < bestRuns.size(); ++i) {
                ofs << "[" << bestRuns[i].runNumber << ", "
                    << extractStrategyName(bestRuns[i].startPoint) << ", "
                    << extractStartPosition(bestRuns[i].startPoint) << "]";
                if (i + 1 < bestRuns.size()) {
                    ofs << ", ";
                }
            }

            ofs << "\n\n";
        }
    }

    cout << "Report saved to simulation_report.txt" << endl;
}