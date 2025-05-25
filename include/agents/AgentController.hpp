#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/assets/TextureManager.hpp"
#include "../include/agents/Agents.hpp"
#include "../include/agents/CollisionDetector.hpp"
#include <vector>

namespace AutonomousCity {
    
    class AgentController {
        struct Cell;
        
        public:
            AgentController(int amountOfAgents, CityGrid *cityGrid, const unsigned int pxWidth, const unsigned int pxHeight, sf::RenderWindow *renderWindow, TextureManager &manager);
            void run(sf::Vector2f desired, float deltaTime); //remove later, used for intial tests
            void draw();
            void setDebug(bool debug);
            bool getDebug() const;
        private:
            std::vector<Agent> agents;
            CityGrid* grid;
            sf::RenderWindow* window;
            TextureManager &textureManager;
            CollisionDetector collisionDetector;
            bool debugOn;
    };
};