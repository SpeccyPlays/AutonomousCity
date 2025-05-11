#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "../include/assets/TextureManager.hpp"

namespace AutonomousCity {
    
    enum class TileState {
        Empty,
        Wet,
        Dry,
        Oil,
    };

    enum class TileType {
        Empty,
        Road,
        Pavement,
        Building,
        TrafficLight,
    };

    class Tile {
    public:
        Tile(TextureManager* manager); // default tile
        Tile(TileType type, TileState state, const std::string& texturePath, TextureManager* manager);
        Tile(TileType type, TileState state, const std::string& texturePath, TextureManager* manager, float tileSize);

        TileType getType() const;
        TileState getState() const;
        std::string getTexturePath() const;
        const sf::Texture& getTexture() const;
        const float getTileSize() const;
        void setTexture(const std::string& texturePath);

    private:
        float tileSize;
        TileType type;
        TileState state;
        std::string texturePath;
        TextureManager* textureManager;
    };

}
