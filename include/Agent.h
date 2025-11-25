#pragma once
#include "Common.h"
#include "Map.h"
#include "Brain.h"

class Agent {
public:
    Agent(Team team, sf::Vector2f startPos, QBrain* sharedBrain);
    virtual ~Agent() = default;

    virtual void update(const Map& map, std::vector<Agent*>& others, float dt) = 0;
    void draw(sf::RenderWindow& window);

    bool isAlive() const { return hp > 0; }
    Team getTeam() const { return team; }
    sf::Vector2f getPos() const { return shape.getPosition(); }
    void takeDamage(float dmg);
    void heal(float amount);

protected:
    float hp, maxHp, speed, range, damage;
    sf::CircleShape shape;
    Team team;
    QBrain* brain;
    AIState lastState;
    Action lastAction;

    AIState getCurrentState(const std::vector<Agent*>& others);
    void executeAction(Action act, const Map& map, std::vector<Agent*>& others, float dt);
};

class Soldier : public Agent {
public:
    Soldier(Team t, sf::Vector2f p, QBrain* b);
    void update(const Map& map, std::vector<Agent*>& others, float dt) override;
};

class Sniper : public Agent {
public:
    Sniper(Team t, sf::Vector2f p, QBrain* b);
    void update(const Map& map, std::vector<Agent*>& others, float dt) override;
};

class Medic : public Agent {
public:
    Medic(Team t, sf::Vector2f p, QBrain* b);
    void update(const Map& map, std::vector<Agent*>& others, float dt) override;
};
