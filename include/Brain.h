#pragma once
#include "Common.h"
#include <map>
#include <tuple>

struct AIState {
    bool enemyInSight;
    bool lowHealth;
    bool isCommanderNearby;

    bool operator<(const AIState& other) const {
        return std::tie(enemyInSight, lowHealth, isCommanderNearby) < 
               std::tie(other.enemyInSight, other.lowHealth, other.isCommanderNearby);
    }
};

class QBrain {
public:
    Action chooseAction(AIState state);
    void updateTable(AIState state, Action action, float reward);

private:
    std::map<AIState, std::map<Action, float>> qTable;
    float epsilon = 0.2f;
    float alpha = 0.1f;
    float gamma = 0.9f;
};
