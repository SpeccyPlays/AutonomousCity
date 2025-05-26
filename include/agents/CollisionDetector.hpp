#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/assets/TextureManager.hpp"
#include "../include/agents/Agents.hpp"
#include "../include/CityGrid/Tile.hpp"
#include <vector>

namespace AutonomousCity {
    
    class CollisionDetector {
        public:
            CollisionDetector(CityGrid *cityGrid, sf::RenderWindow *renderWindow, TextureManager &manager);
            bool checkBoundary(Agent& agent);
            bool agentCollision(Agent* agent, std::unordered_set<AutonomousCity::Agent *> &occupants);
            bool laneDisciplineCheck(Agent* agent);
            std::array<bool, 3> pathsBlocked(Agent& agent);
            void setDebug(bool debug);
        private:
            CityGrid* grid;
            sf::RenderWindow* window;
            TextureManager &textureManager;
            bool offGrid(sf::Vector2i gridPos);
            bool notRoadCheck(sf::Vector2i gridPos);
            sf::Vector2f getSidePoint(Agent* agent);
            std::array<sf::Vector2f, 3> getDirectionalPoints(Agent* agent);
            float calcLookAheadMultipler(float currentSpeed, float size);
            void drawLine(sf::Vector2f start, sf::Vector2f end, sf::Color lineColor);
            void drawCollisionBox(Agent* agent);
            bool debugOn;
    };

};