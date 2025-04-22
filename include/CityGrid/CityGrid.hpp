#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Tile.hpp"
#include "../include/assets/TextureManager.hpp"

namespace AutonomousCity {

    class CityGrid {
    public:
        CityGrid(unsigned int width, unsigned int height, TextureManager& textureManager);

        void draw(sf::RenderWindow& window, float tileSize) const;

        void setTile(unsigned int x, unsigned int y, const Tile& tile);
        const Tile& getTile(unsigned int x, unsigned int y) const;
        Tile& getTile(unsigned int x, unsigned int y);

        unsigned int getWidth() const;
        unsigned int getHeight() const;

        bool saveToFile(const std::string& filename) const;
        bool loadFromFile(const std::string& filename);

    private:
        unsigned int width;
        unsigned int height;
        std::vector<std::vector<Tile>> grid;
        TextureManager& textureManager;
    };

}
