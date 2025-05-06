#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/agents/Agents.hpp"
#include <vector>

namespace AutonomousCity {
    
    class AgentController {
        struct Cell;
        AgentController(int amountOfAgents, CityGrid *cityGrid, sf::RenderWindow *renderWindow);

        public:
            void update();
            void draw();
            void locomotion();
            void setDebug(bool debug);
            bool getDebug() const;
        private:
            std::vector<Agent> agents;
            CityGrid* grid;
            sf::RenderWindow* window;
            bool debugOn;
    };
};