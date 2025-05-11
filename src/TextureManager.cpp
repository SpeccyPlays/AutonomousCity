#include "../include/assets/TextureManager.hpp"
#include <iostream>

namespace AutonomousCity {

    const sf::Texture& TextureManager::getTexture(const std::string& path, float tilesize) {
        static sf::Texture fallbackTexture;
        static bool fallbackInitialized = false;

        // Initialize the fallback texture once
        if (!fallbackInitialized) {
            const unsigned int size = tilesize;
            sf::Image image(sf::Vector2u(size, size), sf::Color(0, 0, 0));

            fallbackTexture.loadFromImage(image);
            fallbackInitialized = true;
        }

        // Empty or invalid path -> fallback
        if (path.empty()) {
            return fallbackTexture;
        }
        
            auto it = textures.find(path);
            if (it == textures.end()) {
                sf::Texture tex;
                if (!tex.loadFromFile(path)) {
                    std::cerr << "Error loading texture: " << path << '\n';
                }
                textures[path] = tex;
            }
            return textures[path];
    }
    const sf::Texture& TextureManager::getTexture(const std::string& path){
            static sf::Texture fallbackTexture;
        static bool fallbackInitialized = false;

        // Initialize the fallback texture once
        if (!fallbackInitialized) {
            const unsigned int size = 16.f;
            sf::Image image(sf::Vector2u(size, size), sf::Color(0, 0, 0));

            fallbackTexture.loadFromImage(image);
            fallbackInitialized = true;
        }

        // Empty or invalid path -> fallback
        if (path.empty()) {
            return fallbackTexture;
        }
        
            auto it = textures.find(path);
            if (it == textures.end()) {
                sf::Texture tex;
                if (!tex.loadFromFile(path)) {
                    std::cerr << "Error loading texture: " << path << '\n';
                }
                textures[path] = tex;
            }
            return textures[path];
        }
}