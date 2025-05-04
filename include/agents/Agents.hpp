#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"

namespace AutonomousCity {
    
    class Agent {

        struct Cell; //forward declare this here to avoid build errors due to circular reference
        enum class AgentState {
            wandering 
        };

        public:
        Agent(sf::Vector2f pos, const std::string& texturePath, bool debugMode);
        Agent(sf::Vector2f pos, bool debugMode);
        sf::Vector2f getCurrentPos() const;
        sf::Vector2f getVelocity() const;
        sf::Vector2f getAcceleration() const;
        sf::Vector2f getDesired() const;

        sf::Vector2f setCurrentPos(sf::Vector2f pos);
        sf::Vector2f setVelocity(sf::Vector2f velocity);
        sf::Vector2f setAcceleration(sf::Vector2f acceleration);
        sf::Vector2f setDesired();

        void update();
        void locomotion();
        void slowDown();
        void draw(sf::RenderWindow &window);
        bool collisionDetection(Cell* cell);
        void setState();
        AgentState getState() const;

        private:
        bool debugOn;
        sf::Vector2f currentPos;
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        sf::Vector2f desiredPos;
        unsigned int mass;
        float maxspeed;
        float wanderingDistance;
        AgentState agentState;
    };
}