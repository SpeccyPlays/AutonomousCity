#pragma once

#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include <random>
#include <string>

namespace AutonomousCity {
    
    class Agent {

        struct Cell; //forward declare this here to avoid build errors due to circular reference

        protected:
            enum class AgentState {
                Standard,
                Wandering
            };
            bool debugOn;
            sf::Vector2f currentPos;
            sf::Vector2f velocity;
            sf::Vector2f acceleration;
            sf::Vector2f desiredPos;
            float angle;
            float maxspeed;
            float currentSpeed;
            float accelerationRate;
            float decelerationRate;
            float wanderingDistance;
            AgentState agentState;
            std::string texturePath;
            //used for random wandering
            std::mt19937 rngSeed;
            std::uniform_real_distribution<float> wanderDist;
            std::uniform_real_distribution<float> randomAngle;

        public:
            virtual ~Agent() = default;
            Agent(sf::Vector2f pos);
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
            void setCurrentSpeed(float newSpeed);
            void update(sf::Vector2f desired, float deltaTime);
            void addWander();
            void locomotion(float deltaTime);
            void slowDown();
            AgentState getState() const;
            std::string getTexturePath() const;

        
    };
    //Variations
    class OldPerson: public Agent{
        public:
            OldPerson(sf::Vector2f pos);
    };
}