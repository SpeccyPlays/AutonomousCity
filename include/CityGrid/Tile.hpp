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

    class Tile {
        public:
            enum class TileType {
                Empty,
                Road,
                RoadHori,
                RoadVert,
                Ttop,
                Tbottom,
                Pavement,
                Building,
                TrafficLight,
            };
            Tile(TextureManager* manager); // default tile
            Tile(Tile::TileType type, TileState state, const std::string& texturePath, TextureManager* manager);
            Tile(Tile::TileType type, TileState state, const std::string& texturePath, TextureManager* manager, float tileSize);

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
