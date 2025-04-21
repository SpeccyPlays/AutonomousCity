#include "../include/CityGrid/Tile.hpp"

namespace AutonomousCity {

    Tile::Tile()
        : type(TileType::Empty), state(TileState::Normal), texturePath("") {}

    Tile::Tile(TileType type, TileState state, const std::string& texturePath)
        : type(type), state(state), texturePath(texturePath) {
        setTexture(texturePath);
    }

    void Tile::setType(TileType newType) {
        type = newType;
    }

    void Tile::setState(TileState newState) {
        state = newState;
    }

    void Tile::setTexture(const std::string& path) {
        texturePath = path;
        texture.loadFromFile(texturePath); // will need error checking in the future
    }

    TileType Tile::getType() const {
        return type;
    }

    TileState Tile::getState() const {
        return state;
    }

    const sf::Texture& Tile::getTexture() const {
        return texture;
    }

    const std::string& Tile::getTexturePath() const {
        return texturePath;
    }

}
