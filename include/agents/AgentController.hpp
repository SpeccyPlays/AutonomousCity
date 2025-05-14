#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/assets/TextureManager.hpp"
#include "../include/agents/Agents.hpp"
#include <vector>

namespace AutonomousCity {
    
    class AgentController {
        struct Cell;
        
        public:
            AgentController(int amountOfAgents, CityGrid *cityGrid, const unsigned int pxWidth, const unsigned int pxHeight, sf::RenderWindow *renderWindow, TextureManager &manager);
            void run(sf::Vector2f desired, float deltaTime); //remove later, used for intial tests
            void update();
            void draw();
            void locomotion();
            void setDebug(bool debug);
            bool getDebug() const;
            bool checkBoundary(Agent& agent);
            bool checkRight(Agent& agent);
        private:
            std::vector<Agent> agents;
            CityGrid* grid;
            sf::RenderWindow* window;
            TextureManager &textureManager;
            unsigned int width;
            unsigned int height;
            bool debugOn;
            void drawLine(sf::Vector2f start, sf::Vector2f end);
            void drawCollisionBox(Agent* agent);
            void obsticleDetections(Agent* agent, std::unordered_set<AutonomousCity::Agent *> &occupants);
    };
};