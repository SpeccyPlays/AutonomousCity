#pragma once

#include <string>
#include <SFML/Graphics.hpp>

namespace AutonomousCity {

    enum class TileType {
        Empty,
        Road,
        Pavement,
        Building,
        TrafficLight
    };

    enum class TileState {
        Normal,
        Wet,
        Oily
    };

    class Tile {
    public:
        Tile();
        Tile(TileType type, TileState state = TileState::Normal, const std::string& texturePath = "");

        void setType(TileType newType);
        void setState(TileState newState);
        void setTexture(const std::string& path);

        TileType getType() const;
        TileState getState() const;
        const sf::Texture& getTexture() const;
        const std::string& getTexturePath() const;

    private:
        TileType type;
        TileState state;
        std::string texturePath;

        sf::Texture texture; // note: loads from file directly for now
    };

}
