// Tile.cpp
#include "../include/CityGrid/Tile.hpp"
#include "../include/assets/TextureManager.hpp"
#include <iostream>

namespace AutonomousCity {

    Tile::Tile(TextureManager* manager)
    : type(TileType::Empty), state(TileState::Empty), texturePath(""), textureManager(manager) {}

    Tile::Tile(TileType type, TileState state, const std::string& texturePath, TextureManager* manager, float tileSize)
        : type(type), state(state), texturePath(texturePath), textureManager(manager) {
            this->tileSize = tileSize;
        }

    TileType Tile::getType() const {
        return type;
    }

    TileState Tile::getState() const {
        return state;
    }

    std::string Tile::getTexturePath() const {
        return texturePath;
    }

    const sf::Texture& Tile::getTexture() const {
        return textureManager->getTexture(texturePath, tileSize);
    }

    const float Tile::getTileSize() const {
        return tileSize;
    }

    void Tile::setTexture(const std::string& texturePath) {
        this->texturePath = texturePath;
    }

}
