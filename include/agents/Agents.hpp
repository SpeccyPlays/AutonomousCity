#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include <random>
#include <string>

namespace AutonomousCity {
    
    class Agent {

        struct Cell; //forward declare this here to avoid build errors due to circular reference
        enum class AgentState {
            Standard,
            Wandering
        };

        public:
        Agent(sf::Vector2f pos, sf::RenderWindow *renderWindow, const unsigned int &width, const unsigned int &height);
        sf::Vector2f getCurrentPos() const;
        sf::Vector2f getVelocity() const;
        sf::Vector2f getAcceleration() const;
        sf::Vector2f getDesired() const;

        void addVelocity(sf::Vector2f toAdd);
        sf::Vector2f limitVector(sf::Vector2f vectorToLimit, float limitValue);
        void setVelocity();
        void accelerate(float deltaTime);
        void setDesired(sf::Vector2f desired);
        void setAngle(float angle);
        float getAngle() const;
        void addSteering(float amount);
        const float getCurrentSpeed() const;

        void update(sf::Vector2f desired, float deltaTime);
        void addWander();
        void locomotion(float deltaTime);
        void slowDown();
        AgentState getState() const;
        std::string getTexturePath() const;

        private:
        bool debugOn;
        sf::Vector2f currentPos;
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        sf::Vector2f desiredPos;
        float angle;
        unsigned int mass;
        float maxspeed;
        float currentSpeed;
        float accelerationRate;
        float wanderingDistance;
        float steeringForce;
        AgentState agentState;
        sf::RenderWindow *window;
        std::string texturePath;
        const unsigned int &windowWidth;
        const unsigned int &windowHeight;
        //used for random wandering
        std::mt19937 rngSeed;
        std::uniform_real_distribution<float> wanderDist;
        std::uniform_real_distribution<float> randomAngle;
    };
}