#ifndef FRONTIER_STRATEGY_HPP
#define FRONTIER_STRATEGY_HPP

#include "CleaningStrategy.hpp"

class FrontierStrategy : public CleaningStrategy {
public:
    string getName() const override;
    void run(Simulator& simulator, const string& dockLabel, const string& runLabel) override;
};

#endif
