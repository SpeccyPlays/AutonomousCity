#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/agents/Agents.hpp"
#include <vector>

namespace AutonomousCity {
    
    class AgentController {
        struct Cell;
        
        public:
            AgentController(int amountOfAgents, CityGrid *cityGrid, const unsigned int &width, const unsigned int &height, sf::RenderWindow *renderWindow);
            void run(sf::Vector2f desired, float deltaTime); //remove later, used for intial tests
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