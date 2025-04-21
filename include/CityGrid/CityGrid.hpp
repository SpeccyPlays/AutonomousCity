#pragma once
#include <SFML/Graphics.hpp>
#include "json.hpp"
#include "Tile.hpp"
#include <vector>
#include <string>
using json = nlohmann::json;

namespace AutonomousCity {

    class CityGrid {
    public:
        CityGrid(unsigned int width, unsigned int height);

        void setTile(unsigned int x, unsigned int y, const Tile& tile);
        const Tile& getTile(unsigned int x, unsigned int y) const;
        Tile& getTile(unsigned int x, unsigned int y);

        unsigned int getWidth() const;
        unsigned int getHeight() const;

        bool saveToFile(const std::string& filename) const;
        bool loadFromFile(const std::string& filename);
        void draw(sf::RenderWindow& window, float tileSize) const;

    private:
        unsigned int width;
        unsigned int height;
        std::vector<std::vector<Tile>> grid;
    };

}