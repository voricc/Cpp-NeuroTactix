#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <cmath>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;
const int TILE_SIZE = 40;
const int GRID_WIDTH = WINDOW_WIDTH / TILE_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / TILE_SIZE;

enum class Team { RED, BLUE };
enum class AgentType { SOLDIER, SNIPER, MEDIC, COMMANDER };
enum class Action { IDLE, MOVE_TO_ENEMY, ATTACK, FLEE, HEAL_ALLY };
enum class TileType { EMPTY, WALL, COVER };

struct Utils {
    static float getDistance(sf::Vector2f a, sf::Vector2f b) {
        return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
    }
    static int randomInt(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(min, max);
        return dist(gen);
    }
};
