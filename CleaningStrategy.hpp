#ifndef CLEANING_STRATEGY_HPP
#define CLEANING_STRATEGY_HPP

#include <string>

using namespace std;

class Simulator;

class CleaningStrategy {
public:
    virtual ~CleaningStrategy() {}
    virtual string getName() const = 0;
    virtual void run(Simulator& simulator, const string& dockLabel, const string& runLabel) = 0;
};

#endif