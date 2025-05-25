#include "../include/agents/Agents.hpp"
#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include <random>
#include <iostream>

namespace AutonomousCity {

   Agent::Agent(sf::Vector2f pos, sf::RenderWindow *renderWindow)
   {
        currentPos = pos;
        mass = 10;
        maxspeed = 100;
        wanderingDistance = 0.01f;
        steeringForce = 5.f;
        agentState = AgentState::Wandering;
        window = renderWindow;
        velocity = {0.f, 0.f};
        accelerationRate = 5.f;
        currentSpeed = 0.f;
        rngSeed = std::mt19937(std::random_device{}());
        wanderDist = std::uniform_real_distribution<float>(-wanderingDistance, wanderingDistance);
        std::uniform_real_distribution<float> angleDist(-1.5708f, 1.5708f);
        angle = angleDist(rngSeed);
        texturePath = "include/assets/car.png";

    };
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
        currentSpeed *= 0.8f;
        if (currentSpeed < 0){
            currentSpeed = 5.f;
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
}