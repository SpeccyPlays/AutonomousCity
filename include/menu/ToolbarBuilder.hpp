#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../include/menu/Toolbar.hpp"
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/agents/AgentController.hpp"

namespace AutonomousCity {
    class ToolbarBuilder {
        private:
            sf::Font font;
            std::vector<Toolbar> toolbars;
        public: 
            void buildToolbars(AutonomousCity::CityGrid* cityGrid, AutonomousCity::AgentController* agentController);
            void buildFileMenu(AutonomousCity::CityGrid* cityGrid, AutonomousCity::AgentController* agentController);
            void addToolbar(Toolbar Toolbar);
            bool handleClick(sf::Vector2f mousePos);
            void draw(sf::RenderWindow& window);
    };
}