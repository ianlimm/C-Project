#ifndef NEAREST_CELL_STRATEGY_HPP
#define NEAREST_CELL_STRATEGY_HPP

#include "CleaningStrategy.hpp"

class NearestCellStrategy : public CleaningStrategy {
public:
    string getName() const override;
    void run(Simulator& simulator, const string& dockLabel, const string& runLabel) override;
};

#endif