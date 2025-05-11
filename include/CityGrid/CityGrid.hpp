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
    class Agent;//forward declare this here to avoid build errors due to circular reference
    struct Cell
    {
        Tile tile;
        std::unordered_set<AutonomousCity::Agent *> occupants;
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

        sf::Vector2i getGridPos(sf::Vector2f agentPos);
        Cell& getCell(sf::Vector2i gridPos);

        bool saveToFile(const std::string &filename) const;
        bool loadFromFile(const std::string &filename);
        bool addAgent(Agent *agent, sf::Vector2i gridPos);
        bool removeAgent(Agent *agent, sf::Vector2i gridPos);
    private:
        unsigned int width;
        unsigned int height;
        std::vector<std::vector<Cell>> grid;
        TextureManager &textureManager;
    };

}
