#include "NearestCellStrategy.hpp"
#include "Simulator.hpp"
#include <vector>
#include <set>
#include <limits>
#include <iostream>
#include <fstream>

using namespace std;

string NearestCellStrategy::getName() const {
    return "Nearest-A*";
}

void NearestCellStrategy::run(Simulator& simulator, const string&, const string& runLabel) {
    Map& map = simulator.getMap();
    Robot& robot = simulator.getRobot();

    set<Point> allWalkable;
    set<Point> visited;

    for (int y = 1; y < map.getRows() - 1; ++y) {
        for (int x = 1; x < map.getCols() - 1; ++x) {
            if (map.isValid(x, y)) {
                allWalkable.insert({x, y});
            }
        }
    }

    simulator.cleanCurrentCell();
    visited.insert(robot.pos);

    while (visited.size() < allWalkable.size()) {
        Point bestTarget = {-1, -1};
        vector<Point> bestPath;
        size_t bestLength = numeric_limits<size_t>::max();

        for (const Point& p : allWalkable) {
            if (visited.count(p)) {
                continue;
            }

            if (robot.pos == p) {
                bestTarget = p;
                bestPath.clear();
                bestLength = 0;
                break;
            }

            vector<Point> path = map.getPathHome(robot.pos, p);
            if (!path.empty() && path.size() < bestLength) {
                bestTarget = p;
                bestPath = path;
                bestLength = path.size();
            }
        }

        if (bestTarget.x == -1) {
            cout << "\nNo more reachable uncleaned cells found." << endl;
            break;
        }

        for (int i = 0; i < static_cast<int>(bestPath.size()); ++i) {
            bool isLast = (i == static_cast<int>(bestPath.size()) - 1);
            bool isNavigating = !isLast || visited.count(bestPath[i]) > 0;
            simulator.moveRobot(bestPath[i], isNavigating);
            visited.insert(bestPath[i]);
        }

        if (robot.pos == bestTarget && bestPath.empty()) {
            simulator.cleanCurrentCell();
            visited.insert(robot.pos);
        }
        else if (robot.pos == bestTarget && !visited.count(robot.pos)) {
            simulator.cleanCurrentCell();
            visited.insert(robot.pos);
        }
    }

    simulator.returnToDock();
    simulator.recordHistory(runLabel);
}