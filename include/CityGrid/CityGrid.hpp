#pragma once
#include "Tile.hpp"
#include "../include/assets/TextureManager.hpp"
#include "../include/agents/Agents.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_set>

namespace AutonomousCity
{

    struct Cell
    {
        Tile tile;
        std::unordered_set<Agent *> occupants;
        Cell(TextureManager *manager) : tile(manager) {}
    };

    class CityGrid
    {
    public:
        CityGrid(unsigned int width, unsigned int height, TextureManager &textureManager);

        void draw(sf::RenderWindow &window, float tileSize) const;

        void setTile(unsigned int x, unsigned int y, const Tile &tile);
        const Tile &getTile(unsigned int x, unsigned int y) const;
        Tile &getTile(unsigned int x, unsigned int y);

        unsigned int getWidth() const;
        unsigned int getHeight() const;

        bool saveToFile(const std::string &filename) const;
        bool loadFromFile(const std::string &filename);

    private:
        unsigned int width;
        unsigned int height;
        std::vector<std::vector<Cell>> grid;
        TextureManager &textureManager;
    };

}
