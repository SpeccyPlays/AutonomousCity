#include "../include/CityGrid/CityGrid.hpp"
#include "../include/CityGrid/Tile.hpp"
#include "../include/CityGrid/json.hpp"
#include "../include/agents/Agents.hpp"
#include <fstream>
#include <iostream>
#include <unordered_set>

using json = nlohmann::json;

namespace AutonomousCity
{
    CityGrid::CityGrid(unsigned int w, unsigned int h, TextureManager &manager, float tileSize)
        : width(w), height(h), textureManager(manager)
    {
        grid.resize(height, std::vector<Cell>(width, Cell(&textureManager)));
        this->tileSize = tileSize;
    };

    void CityGrid::setTile(unsigned int x, unsigned int y, const Tile &tile)
    {
        if (y < height && x < width)
            grid[y][x].tile = tile;
    };

    const Tile &CityGrid::getTile(unsigned int x, unsigned int y) const
    {
        return grid[y][x].tile;
    };

    Tile &CityGrid::getTile(unsigned int x, unsigned int y)
    {
        return grid[y][x].tile;
    };

    unsigned int CityGrid::getWidth() const { return width; }
    unsigned int CityGrid::getHeight() const { return height; }

    void CityGrid::draw(sf::RenderWindow &window) const
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            for (unsigned int x = 0; x < width; ++x)
            {
                const Tile &tile = grid[y][x].tile;
                const sf::Texture &texture = tile.getTexture();
                sf::Sprite sprite(texture);
                sprite.setPosition({x * tileSize, y * tileSize});
                window.draw(sprite);
            }
        }

        // Draw grid lines
        sf::Color lineColor(100, 100, 100);

        for (unsigned int x = 0; x <= width; ++x)
        {
            std::array<sf::Vertex, 2> line = {
                sf::Vertex{sf::Vector2f(x * tileSize, 0.f), lineColor},
                sf::Vertex{sf::Vector2f(x * tileSize, height * tileSize), lineColor}};
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }

        for (unsigned int y = 0; y <= height; ++y)
        {
            std::array<sf::Vertex, 2> line = {
                sf::Vertex{sf::Vector2f(0.f, y * tileSize), lineColor},
                sf::Vertex{sf::Vector2f(width * tileSize, y * tileSize), lineColor}};
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }
    };
    sf::Vector2i CityGrid::getGridPos(sf::Vector2f agentPos){
        return static_cast<sf::Vector2i>(agentPos / tileSize);
    };
    Cell& CityGrid::getCell(sf::Vector2i gridPos){
        return grid[gridPos.y][gridPos.x];
    }
    bool CityGrid::addAgent(Agent *agent, sf::Vector2i gridPos){
        if (gridPos.y < 0 || gridPos.x < 0 || gridPos.y > height || gridPos.x > width){
            //we're off the grid somehow
            return false;
        }
        grid[gridPos.y][gridPos.x].occupants.insert(agent); 
        return true;
    };
    bool CityGrid::removeAgent(Agent *agent, sf::Vector2i gridPos){
        if (gridPos.y < 0 || gridPos.x < 0 || gridPos.y > height || gridPos.x > width){
            //we're off the grid somehow
            return false;
        }
        grid[gridPos.y][gridPos.x].occupants.erase(agent);
        return true;
    };
    bool CityGrid::saveToFile(const std::string &filename) const
    {
        json j;
        j["width"] = width;
        j["height"] = height;
        j["tileSize"] = tileSize;

        for (unsigned int y = 0; y < height; ++y)
        {
            for (unsigned int x = 0; x < width; ++x)
            {
                const Tile &tile = grid[y][x].tile;
                j["tiles"].push_back({{"x", x},
                                      {"y", y},
                                      {"type", static_cast<int>(tile.getType())},
                                      {"state", static_cast<int>(tile.getState())},
                                      {"texture", tile.getTexturePath()},
                                    {"tileSize", tile.getTileSize()}});
            }
        }

        std::ofstream file(filename);
        if (!file.is_open())
            return false;
        file << j.dump(2);
        return true;
    };

    bool CityGrid::loadFromFile(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
            return false;

        json j;
        file >> j;

        width = j["width"];
        height = j["height"];
        tileSize = j["tileSize"];
        grid = std::vector<std::vector<Cell>>(height, std::vector<Cell>(width, Cell(&textureManager)));

        for (const auto &entry : j["tiles"])
        {
            unsigned int x = entry["x"];
            unsigned int y = entry["y"];
            TileType type = static_cast<TileType>(entry["type"]);
            TileState state = static_cast<TileState>(entry["state"]);
            std::string texture = entry["texture"];
            float tileSize = entry["tileSize"];
            grid[y][x].tile = Tile(type, state, texture, &textureManager, tileSize);
        }

        return true;
    };
};