#pragma once

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>

namespace AutonomousCity {

    class TextureManager {
    public:
        const sf::Texture& getTexture(const std::string& path, float tileSize);
        const sf::Texture& getTexture(const std::string& path);

    private:
        std::unordered_map<std::string, sf::Texture> textures;
    };

}