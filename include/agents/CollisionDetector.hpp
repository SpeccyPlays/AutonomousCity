#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/assets/TextureManager.hpp"
#include "../include/agents/Agents.hpp"
#include <vector>

namespace AutonomousCity {
    
    class CollisionDetector {
        public:
            CollisionDetector(CityGrid *cityGrid, sf::RenderWindow *renderWindow, TextureManager &manager);
            bool checkBoundary(Agent& agent);
            bool agentCollision(Agent* agent, std::unordered_set<AutonomousCity::Agent *> &occupants);
            void setDebug(bool debug);
        private:
            CityGrid* grid;
            sf::RenderWindow* window;
            TextureManager &textureManager;
            std::array<sf::Vector2f, 3> getDirectionalPoints(Agent* agent);
            float calcLookAheadMultipler(float currentSpeed, float size);
            void drawLine(sf::Vector2f start, sf::Vector2f end);
            void drawCollisionBox(Agent* agent);
            bool debugOn;
    };

};