#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../include/menu/Toolbar.hpp"

namespace AutonomousCity {
    class ToolbarBuilder {
        private:
            sf::Font font;
            std::vector<Toolbar> toolbars;
        public: 
            void buildToolbars();
            void buildFileMenu();
            void addToolbar(Toolbar Toolbar);
            bool handleClick(sf::Vector2f mousePos);
            void draw(sf::RenderWindow& window);
    };
}