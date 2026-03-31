#ifndef BCD_STRATEGY_HPP
#define BCD_STRATEGY_HPP

#include "CleaningStrategy.hpp"

class BCDStrategy : public CleaningStrategy {
public:
    string getName() const override;
    void run(Simulator& simulator, const string& dockLabel, const string& runLabel) override;
};

#endif
