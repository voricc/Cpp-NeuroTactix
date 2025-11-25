#include "Brain.h"

Action QBrain::chooseAction(AIState state) {
    if ((float(rand()) / RAND_MAX) < epsilon) {
        return static_cast<Action>(rand() % 5);
    }
    float maxQ = -9999.0f;
    Action bestAction = Action::IDLE;

    if (qTable[state].empty()) {
        for(int i=0; i<5; ++i) qTable[state][static_cast<Action>(i)] = 0.0f;
    }

    for (auto const& [action, val] : qTable[state]) {
        if (val > maxQ) {
            maxQ = val;
            bestAction = action;
        }
    }
    return bestAction;
}

void QBrain::updateTable(AIState state, Action action, float reward) {
    float oldQ = qTable[state][action];
    qTable[state][action] = oldQ + alpha * (reward - oldQ);
}
