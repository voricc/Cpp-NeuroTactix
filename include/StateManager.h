#pragma once
#include "Common.h"
#include "Agent.h"
#include "Map.h"

class GameEngine;

class State {
public:
    virtual ~State() = default;
    virtual void handleInput(GameEngine& game, sf::Event event) = 0;
    virtual void update(GameEngine& game, float dt) = 0;
    virtual void draw(GameEngine& game, sf::RenderWindow& window) = 0;
};

class GameEngine {
public:
    GameEngine();
    void run();
    void changeState(std::unique_ptr<State> newState);
    sf::RenderWindow window;
    sf::Font font;
private:
    std::unique_ptr<State> currentState;
};
