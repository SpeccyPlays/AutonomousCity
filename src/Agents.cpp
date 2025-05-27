#include "../include/agents/Agents.hpp"
#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include <random>
#include <iostream>

namespace AutonomousCity {
    /**
     * Constructors
     */
   Agent::Agent(sf::Vector2f pos)
   {
        currentPos = pos;
        maxspeed = 50;
        wanderingDistance = 0.01f;
        agentState = AgentState::Wandering;
        velocity = {0.f, 0.f};
        accelerationRate = maxspeed * 0.1f;
        decelerationRate = 0.8f;
        currentSpeed = 0.f;
        rngSeed = std::mt19937(std::random_device{}());
        wanderDist = std::uniform_real_distribution<float>(-wanderingDistance, wanderingDistance);
        std::uniform_real_distribution<float> angleDist(-1.5708f, 1.5708f);
        angle = angleDist(rngSeed);
        texturePath = "include/assets/car.png";

    };
    OldPerson::OldPerson(sf::Vector2f pos) : Agent(pos) {
        maxspeed = 40;
        wanderingDistance = 0.02f;
        agentState = AgentState::Wandering;
        accelerationRate = maxspeed * 0.1f;
        decelerationRate = 0.9f;
        texturePath = "include/assets/caroldperson.png";
    }; 
    /**
     * Code
     */
    void Agent::update(sf::Vector2f desired, float deltaTime){
        float speedLimit = maxspeed;
        accelerate(deltaTime);
        setVelocity();
        sf::Vector2f desiredPos = currentPos + velocity * deltaTime;
        setDesired(desiredPos);
    }
    void Agent::addWander(){
        sf::Vector2f wanderAmount = {wanderDist(rngSeed), wanderDist(rngSeed)};
        addSteering(wanderDist(rngSeed));
    }
    void Agent::locomotion(float deltaTime){
        currentPos += velocity * deltaTime;
    };
    sf::Vector2f Agent::getDesired() const {
        return desiredPos;
    }
    void Agent::setDesired(sf::Vector2f desired){
        desiredPos = desired;
    };
    void Agent::addVelocity(sf::Vector2f toAdd){
        velocity += toAdd;
    }
    sf::Vector2f Agent::limitVector(sf::Vector2f vectorToLimit, float limitValue){
        return vectorToLimit.normalized() * limitValue;
    }
    void Agent::setVelocity(){
        velocity.x = std::cos(angle) * currentSpeed;
        velocity.y = std::sin(angle) * currentSpeed;
    }
    void Agent::slowDown(){
        currentSpeed *= decelerationRate;
        if (currentSpeed < 1){
            currentSpeed = 0.f;
        }
        setVelocity();
    };
    sf::Vector2f Agent::getCurrentPos() const {
        return currentPos;
    }
    sf::Vector2f Agent::getVelocity() const {
        return velocity;
    }
    void Agent::accelerate(float deltaTime){
        currentSpeed += (accelerationRate * deltaTime);
        if (currentSpeed > maxspeed){
            currentSpeed = maxspeed;
        }
    }
    void Agent::addSteering(float amount){
        angle += amount;
    }
    void Agent::setAngle(float angle){
        angle = angle;
    };
    float Agent::getAngle() const{
        return angle;
    };
    std::string Agent::getTexturePath() const{
        return texturePath;
    }
    const float Agent::getCurrentSpeed() const {
        return currentSpeed;
    }
    void Agent::setCurrentSpeed(float newSpeed){
        currentSpeed = newSpeed;
    }
    
}