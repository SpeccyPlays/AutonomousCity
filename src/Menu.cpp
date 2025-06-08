#include <SFML/Graphics.hpp>
#include "../include/menu/Menu.hpp"
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/assets/TextureManager.hpp"
#include "../include/CityGrid/Tile.hpp"

namespace AutonomousCity {
    Menu::Menu(CityGrid *cityGrid, sf::RenderWindow *renderWindow, TextureManager *manager) : grid(cityGrid), window(renderWindow), textureManager(manager){
        currentMode = Menu::Mode::None;
    };
    void Menu::decideAction(std::optional<sf::Event> event){
        if (!event.has_value()){
            return;
        };
        switch(currentMode){
            case Menu::Mode::None :
                break;
            case Menu::Mode::Editing :
                if (event->is<sf::Event::MouseButtonPressed>()){
                    if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left){
                        addTile();
                    }
                    else if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Right){
                        removeTile();
                    };
                };
                break;
        }
    };
    void Menu::addTile(){

    };
    void Menu::removeTile(){

    };
}