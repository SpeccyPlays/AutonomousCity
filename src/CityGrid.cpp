#include "../include/CityGrid/CityGrid.hpp"
#include <fstream>
#include <iostream>

namespace AutonomousCity
{

    CityGrid::CityGrid(unsigned int width, unsigned int height)
        : width(width), height(height), grid(height, std::vector<Tile>(width)) {}

    void CityGrid::setTile(unsigned int x, unsigned int y, const Tile &tile)
    {
        if (y < height && x < width && y >= 0 && x >= 0)
        {
            grid[y][x] = tile;
        }
    }

    const Tile &CityGrid::getTile(unsigned int x, unsigned int y) const
    {
        return grid[y][x];
    }

    Tile &CityGrid::getTile(unsigned int x, unsigned int y)
    {
        return grid[y][x];
    }

    unsigned int CityGrid::getWidth() const { return width; }
    unsigned int CityGrid::getHeight() const { return height; }

    void CityGrid::draw(sf::RenderWindow &window, float tileSize) const
    {
        /* Comment out drawing the whole grid first and just draw gridlines
        for (unsigned int y = 0; y < height; ++y)
        {
            for (unsigned int x = 0; x < width; ++x)
            {
                const Tile &tile = grid[y][x];
                const sf::Texture &texture = tile.getTexture();
                sf::Sprite sprite(texture);
                sprite.setPosition({x * tileSize, y * tileSize});
                ;
                window.draw(sprite);
            }
        }
        */

        // Draw grid lines
        sf::Color lineColor(100, 100, 100); // Light gray

        // Vertical lines
        for (unsigned int x = 0; x <= width; ++x)
        {
            std::array<sf::Vertex, 2> line = {
                sf::Vertex{sf::Vector2f(x * tileSize, 0.f), lineColor},
                sf::Vertex{sf::Vector2f(x * tileSize, height * tileSize), lineColor}};
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }

        // Horizontal lines
        for (unsigned int y = 0; y <= height; ++y)
        {
            std::array<sf::Vertex, 2> line = {
                sf::Vertex{sf::Vector2f(0.f, y * tileSize), lineColor},
                sf::Vertex{sf::Vector2f(width * tileSize, y * tileSize), lineColor}};
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }
    }

    bool CityGrid::saveToFile(const std::string &filename) const
    {
        json j;
        j["width"] = width;
        j["height"] = height;

        for (unsigned int y = 0; y < height; ++y)
        {
            for (unsigned int x = 0; x < width; ++x)
            {
                const Tile &tile = grid[y][x];
                j["tiles"].push_back({{"x", x},
                                      {"y", y},
                                      {"type", static_cast<int>(tile.getType())},
                                      {"state", static_cast<int>(tile.getState())},
                                      {"texture", tile.getTexturePath()}});
            }
        }

        std::ofstream file(filename);
        if (!file.is_open())
            return false;
        file << j.dump(2);
        return true;
    }

    bool CityGrid::loadFromFile(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
            return false;

        json j;
        file >> j;

        width = j["width"];
        height = j["height"];
        grid = std::vector<std::vector<Tile>>(height, std::vector<Tile>(width));

        for (const auto &entry : j["tiles"])
        {
            unsigned int x = entry["x"];
            unsigned int y = entry["y"];
            TileType type = static_cast<TileType>(entry["type"]);
            TileState state = static_cast<TileState>(entry["state"]);
            std::string texture = entry["texture"];

            grid[y][x] = Tile(type, state, texture);
        }

        return true;
    }

}
