#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"

namespace AutonomousCity {
    
    class Agent {

        struct Cell; //forward declare this here to avoid build errors due to circular reference
        enum class AgentState {
            Standard,
            Wandering
        };

        public:
        Agent(sf::Vector2f pos, sf::RenderWindow *renderWindow, const unsigned int &width, const unsigned int &height, const std::string& texturePath, bool debugMode);
        Agent(sf::Vector2f pos, sf::RenderWindow *renderWindow, const unsigned int &width, const unsigned int &height, bool debugMode);
        sf::Vector2f getCurrentPos() const;
        sf::Vector2f getVelocity() const;
        sf::Vector2f getAcceleration() const;
        sf::Vector2f getDesired() const;

        void setCurrentPos(sf::Vector2f pos);
        void addVelocity(sf::Vector2f toAdd);
        sf::Vector2f limitVector(sf::Vector2f vectorToLimit, float limitValue);
        void setVelocity(float speedLimit);
        void setAcceleration(sf::Vector2f acceleration);
        void setDesired(sf::Vector2i desired);

        void update(sf::Vector2i desired);
        void wandering();
        void addWander();
        void locomotion(float deltaTime);
        void slowDown();
        void draw();
        bool checkBoundary();
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
        float currentSpeed;
        float accelerationRate;
        float wanderingDistance;
        float steeringForce;
        AgentState agentState;
        sf::RenderWindow *window;
        const unsigned int &windowWidth;
        const unsigned int &windowHeight;
    };
}