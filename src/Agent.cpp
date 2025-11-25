#include "Agent.h"

Agent::Agent(Team t, sf::Vector2f startPos, QBrain* sharedBrain) 
    : team(t), brain(sharedBrain), lastAction(Action::IDLE) {
    shape.setPosition(startPos);
    shape.setOrigin(10, 10);
}

void Agent::draw(sf::RenderWindow& window) {
    if (!isAlive()) return;
    shape.setPosition(shape.getPosition());
    window.draw(shape);
    sf::RectangleShape bar(sf::Vector2f(20 * (hp/maxHp), 4));
    bar.setPosition(shape.getPosition().x - 10, shape.getPosition().y - 15);
    bar.setFillColor(sf::Color::Green);
    window.draw(bar);
}

void Agent::takeDamage(float dmg) {
    hp -= dmg;
    if (brain) brain->updateTable(lastState, lastAction, -5.0f);
    if (hp <= 0) if (brain) brain->updateTable(lastState, lastAction, -100.0f);
}

void Agent::heal(float amount) { hp = std::min(hp + amount, maxHp); }

AIState Agent::getCurrentState(const std::vector<Agent*>& others) {
    AIState s;
    s.lowHealth = (hp < (maxHp * 0.3f));
    s.enemyInSight = false;
    s.isCommanderNearby = false;
    for (auto* other : others) {
        if (!other->isAlive()) continue;
        if (other->getTeam() != team && Utils::getDistance(getPos(), other->getPos()) < range) s.enemyInSight = true;
    }
    return s;
}

void Agent::executeAction(Action act, const Map& map, std::vector<Agent*>& others, float dt) {
    lastAction = act;
    sf::Vector2f currentPos = shape.getPosition();
    float reward = -0.1f;

    switch (act) {
        case Action::MOVE_TO_ENEMY: {
            Agent* target = nullptr; float minDist = 9999.f;
            for (auto* o : others) {
                if (o->getTeam() != team && o->isAlive()) {
                    float d = Utils::getDistance(currentPos, o->getPos());
                    if (d < minDist) { minDist = d; target = o; }
                }
            }
            if (target) {
                sf::Vector2f dir = target->getPos() - currentPos;
                float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
                if (len > 0) dir /= len;
                sf::Vector2f nextPos = currentPos + dir * speed * dt;
                if (map.isWalkable(nextPos.x/TILE_SIZE, nextPos.y/TILE_SIZE)) shape.move(dir * speed * dt);
            }
            break;
        }
        case Action::ATTACK: {
            for (auto* o : others) {
                if (o->getTeam() != team && o->isAlive() && Utils::getDistance(currentPos, o->getPos()) <= range) {
                    o->takeDamage(damage); reward = 10.0f; break;
                }
            }
            break;
        }
        case Action::FLEE: shape.move(sf::Vector2f((rand()%3 -1)*speed*dt, (rand()%3 -1)*speed*dt)); break;
        default: break;
    }
    if (brain) brain->updateTable(lastState, act, reward);
}

Soldier::Soldier(Team t, sf::Vector2f p, QBrain* b) : Agent(t, p, b) {
    hp = 100; maxHp = 100; speed = 50.f; range = 150.f; damage = 10.f;
    shape.setRadius(10.f); shape.setFillColor(t == Team::RED ? sf::Color::Red : sf::Color::Blue);
}
void Soldier::update(const Map& map, std::vector<Agent*>& others, float dt) {
    lastState = getCurrentState(others); executeAction(brain->chooseAction(lastState), map, others, dt);
}

Sniper::Sniper(Team t, sf::Vector2f p, QBrain* b) : Agent(t, p, b) {
    hp = 60; maxHp = 60; speed = 40.f; range = 300.f; damage = 40.f;
    shape.setRadius(8.f); shape.setPointCount(3); shape.setFillColor(t == Team::RED ? sf::Color(200, 50, 50) : sf::Color(50, 50, 200));
}
void Sniper::update(const Map& map, std::vector<Agent*>& others, float dt) {
    lastState = getCurrentState(others); executeAction(brain->chooseAction(lastState), map, others, dt);
}

Medic::Medic(Team t, sf::Vector2f p, QBrain* b) : Agent(t, p, b) {
    hp = 80; maxHp = 80; speed = 60.f; range = 100.f; damage = 5.f;
    shape.setRadius(9.f); shape.setOutlineThickness(2); shape.setOutlineColor(sf::Color::White);
    shape.setFillColor(t == Team::RED ? sf::Color::Red : sf::Color::Blue);
}
void Medic::update(const Map& map, std::vector<Agent*>& others, float dt) {
    lastState = getCurrentState(others);
    Action act = brain->chooseAction(lastState);
    if (act == Action::HEAL_ALLY) {
        for(auto* a : others) if(a->getTeam() == team && a != this && a->isAlive() && Utils::getDistance(getPos(), a->getPos()) < 50.f) a->heal(20.f);
    } else executeAction(act, map, others, dt);
}
