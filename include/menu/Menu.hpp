#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/assets/TextureManager.hpp"
#include "../include/CityGrid/Tile.hpp"
#include <iostream>
#include <string>
#include <map>

namespace AutonomousCity {

    class Menu {
        
        public :
            Menu(CityGrid *cityGrid, sf::RenderWindow *renderWindow, TextureManager *manager);
            void decideAction(std::optional<sf::Event> event);
            bool loadTextureFromTile(std::string path);
            void setMenuOption();
            void addTile();
            void removeTile();
        private:
            enum class Mode {
                None,
                Editing
            };
            CityGrid* grid;
            sf::RenderWindow* window;
            TextureManager* textureManager;
            std::map<AutonomousCity::Tile::TileType, sf::Texture> typeToTexture;
            Mode currentMode; 
    };
}