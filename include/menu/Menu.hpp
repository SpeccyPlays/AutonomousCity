#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/assets/TextureManager.hpp"
#include "../include/CityGrid/Tile.hpp"

namespace AutonomousCity {

    class Menu {
        public :
            Menu(CityGrid *cityGrid, sf::RenderWindow *renderWindow, TextureManager *manager);
            bool loadTextureFromTile(std::string path);
            void setMenuOption();
            bool addTile();
            bool removeTile();
        private:
            CityGrid* grid;
            sf::RenderWindow* window;
            TextureManager* textureManager;
    };
}