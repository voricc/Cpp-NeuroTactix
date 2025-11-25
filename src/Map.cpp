#include "Map.h"

Map::Map() {
    grid.resize(GRID_WIDTH, std::vector<Tile>(GRID_HEIGHT));
}

void Map::generate(unsigned int seed) {
    std::srand(seed);
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            Tile& tile = grid[x][y];
            tile.shape.setSize(sf::Vector2f(TILE_SIZE - 2, TILE_SIZE - 2));
            tile.shape.setPosition(x * TILE_SIZE, y * TILE_SIZE);
            tile.shape.setOutlineThickness(1);
            tile.shape.setOutlineColor(sf::Color(50, 50, 50));

            int r = std::rand() % 100;
            if (r < 10) {
                tile.type = TileType::WALL;
                tile.shape.setFillColor(sf::Color(100, 100, 100));
            } else if (r < 20) {
                tile.type = TileType::COVER;
                tile.shape.setFillColor(sf::Color(100, 150, 100, 150));
            } else {
                tile.type = TileType::EMPTY;
                tile.shape.setFillColor(sf::Color(30, 30, 30));
            }
        }
    }
}

void Map::draw(sf::RenderWindow& window) {
    for (const auto& col : grid) {
        for (const auto& tile : col) {
            window.draw(tile.shape);
        }
    }
}

bool Map::isWalkable(int x, int y) const {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return false;
    return grid[x][y].type != TileType::WALL;
}

sf::Vector2f Map::getGridPosition(int x, int y) const {
    return sf::Vector2f(x * TILE_SIZE + TILE_SIZE / 2.f, y * TILE_SIZE + TILE_SIZE / 2.f);
}
