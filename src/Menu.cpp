#include "../include/menu/Menu.hpp"
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/assets/TextureManager.hpp"
#include "../include/CityGrid/Tile.hpp"

namespace AutonomousCity {
    Menu::Menu(CityGrid *cityGrid, sf::RenderWindow *renderWindow, TextureManager *manager) : grid(cityGrid), window(renderWindow), textureManager(manager){
        
    }
}