/*****
 * strategy.h
 * Main header for the strategies
 * 
 * See LICENSE for details.
 *
 *****/

#ifndef bridgesim_strategy_h
#define bridgesim_strategy_h

#include "bridge_sim.h"

namespace BridgeSim
{

/*************************************
 * Strategy interface
 *************************************/
class IStrategy
{
public:
    virtual void initialize(const State& state, BridgeId startBridge=0) = 0;
    virtual bool is_finished() const = 0;
    virtual Move generateNextMove() = 0;
    virtual void execute(const Move& move) = 0;
    virtual float elapsedTime() const = 0;
    virtual Statistics results() = 0;
    virtual ~IStrategy() {}
};

/*****
 * Local strategy
 * 
 * Move everyone across one bridge at a time.
 * Try to use the fastest person at the bridge.
 *****/
class LocalStrategy : public IStrategy
{
public:
    virtual void initialize(const State& state, BridgeId startBridge=0);
    virtual bool is_finished() const;
    virtual Move generateNextMove();
    virtual void execute(const Move& move);
    virtual float elapsedTime() const;
    virtual Statistics results();
    virtual ~LocalStrategy() {}

private:
    Model       m_model;
    std::vector<float> m_timeBridgeCompleted;
};

}  /* namespace BridgeSim */

#endif /* bridgesim_strategy_h */

