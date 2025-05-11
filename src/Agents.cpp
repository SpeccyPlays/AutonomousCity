#include "../include/agents/Agents.hpp"
#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include <random>
#include <iostream>

namespace AutonomousCity {

    Agent::Agent(sf::Vector2f pos, sf::RenderWindow *renderWindow, const unsigned int &width, const unsigned int &height, const std::string& texturePath, bool debugMode)
    :windowWidth(width), windowHeight(height)
    {
        currentPos = pos;
        debugOn = debugMode;
        mass = 10;
        maxspeed = 100;
        wanderingDistance = 10.f;
        steeringForce = 5.f;
        agentState = AgentState::Wandering;
        window = renderWindow;
        velocity = {0.f, 0.f};
        accelerationRate = maxspeed / 2;
        currentSpeed = 0.f;
    };
   Agent::Agent(sf::Vector2f pos, sf::RenderWindow *renderWindow, const unsigned int &width, const unsigned int &height, bool debugMode)
    :windowWidth(width), windowHeight(height)
   {
        currentPos = pos;
        debugOn = debugMode;
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

    };
    void Agent::update(sf::Vector2f desired, float deltaTime){
        float speedLimit = maxspeed;
        if (checkBoundary()){
            //slowDown();
        }
        else if (agentState == AgentState::Wandering){
            addWander();
        }
        accelerate(deltaTime);
        setVelocity();
        sf::Vector2f desiredPos = currentPos + velocity * deltaTime;
        setDesired(desiredPos);
    }
    void Agent::addWander(){
        sf::Vector2f wanderAmount = {wanderDist(rngSeed), wanderDist(rngSeed)};
        //setDesired(currentPos + velocity + wanderAmount);
        addSteering(wanderDist(rngSeed));
    }
    bool Agent::checkBoundary(){
        bool willhitBoundary = false;
        float boundary = maxspeed / 2;
        sf::Vector2f steeringCorrection = {0.f, 0.f};
        sf::Vector2f nextPos = currentPos + velocity;
        float steeringAmount = 0.05;
        if (nextPos.x < boundary){
            addSteering(steeringAmount);     
            willhitBoundary = true;
        }
        else if (nextPos.x > windowWidth - boundary){
            addSteering(steeringAmount);
            willhitBoundary = true;
        }
        if (nextPos.y < boundary){     
            addSteering(steeringAmount);
            willhitBoundary = true;
        }
        else if (nextPos.y > windowHeight - boundary){
            addSteering(steeringAmount);
            willhitBoundary = true;
        }
        return willhitBoundary;
    }
    void Agent::draw(){
        sf::CircleShape agent(mass/ 2);
        agent.setOrigin({mass /2, mass / 2});
        agent.setOutlineThickness(2);
        agent.setPosition(currentPos);
        agent.setFillColor(sf::Color::Transparent);
        window->draw(agent);
        if (debugOn){
            sf::Color lineColor(255, 255, 255);
            sf::Vector2f endPoint = currentPos + velocity;
            std::array<sf::Vertex, 2> line = {
                sf::Vertex{sf::Vector2f(currentPos), lineColor},
                sf::Vertex{sf::Vector2f(endPoint), lineColor}};
            window->draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        };
    };
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
}