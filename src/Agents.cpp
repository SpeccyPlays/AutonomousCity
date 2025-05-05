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
        maxspeed = 10;
        wanderingDistance = 20;
        steeringForce = 0.05;
        agentState = AgentState::Wandering;
        window = renderWindow;
        velocity = {0.f, 0.f};
        accelerationRate = 10.f;
        currentSpeed = 0.f;
    };
   Agent::Agent(sf::Vector2f pos, sf::RenderWindow *renderWindow, const unsigned int &width, const unsigned int &height, bool debugMode)
    :windowWidth(width), windowHeight(height)
   {
        currentPos = pos;
        debugOn = debugMode;
        mass = 10;
        maxspeed = 100;
        wanderingDistance = 20;
        steeringForce = 0.05;
        agentState = AgentState::Wandering;
        window = renderWindow;
        velocity = {0.f, 0.f};
        accelerationRate = 10.f;
        currentSpeed = 0.f;
    };
    void Agent::update(sf::Vector2i desired){
        float speedLimit = maxspeed;
        switch(agentState){
            case AgentState::Wandering:
                wandering();
                break;
            default:
                setDesired(desired);
                break;
        }
        if (checkBoundary()){
            slowDown();
            setDesired(static_cast<sf::Vector2i>(currentPos + velocity));
        } 
        setVelocity(speedLimit);
    }
    void Agent::wandering(){
        sf::Vector2f tempDesired = currentPos + (velocity * maxspeed);
        //all this to generate a random angle and distance
        std::random_device randomAngle;
        std::uniform_real_distribution<float> angleRange(0.0f, 360.0f);
        sf::Angle angle = sf::degrees(angleRange(randomAngle));
        float radians = angle.asRadians();
        std::random_device randomDistance;
        std::uniform_real_distribution<float> dist(maxspeed / 2, maxspeed);
        float distance = dist(randomDistance);
        tempDesired.x += distance * cos(radians);
        tempDesired.y += distance * sin(radians);
        setDesired(static_cast<sf::Vector2i>(tempDesired));
    }
    bool Agent::checkBoundary(){
        bool willhitBoundary = false;
        float boundary = maxspeed / 2;
        sf::Vector2f steeringCorrection = {0.f, 0.f};
        sf::Vector2f nextPos = currentPos + velocity;
        if (nextPos.x < boundary){
            steeringCorrection.x += 10.f;           
            willhitBoundary = true;
        }
        else if (nextPos.x > windowWidth - boundary){
            steeringCorrection.x += -10.f;
            willhitBoundary = true;
        }
        if (nextPos.y < boundary){     
            steeringCorrection.y += 10.f;
            willhitBoundary = true;
        }
        else if (nextPos.y > windowHeight - boundary){
            steeringCorrection.y += -10.f;
            willhitBoundary = true;
        }
        velocity += steeringCorrection;
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
            sf::Font font("include/assets/arial.ttf"); 
            sf::Text text(font);
            float x = 0.f;
            float y = 0.f;
            text.setPosition({x, y});
            text.setCharacterSize(18);
            text.setFillColor(sf::Color::Red);
            std::stringstream ss;
            ss << "Current pos: (" << (int)currentPos.x << ", " << (int)currentPos.y << ") Desired pos: (" << (int)desiredPos.x << ", " << (int)desiredPos.y << ")";
            text.setString(ss.str());
            window->draw(text);
        };
    };
    void Agent::locomotion(float deltaTime){
        currentPos += velocity * deltaTime;
    };
    void Agent::setDesired(sf::Vector2i desired){
        desiredPos = static_cast<sf::Vector2f>(desired);
    };
    void Agent::addVelocity(sf::Vector2f toAdd){
        velocity += toAdd;
    }
    sf::Vector2f Agent::limitVector(sf::Vector2f vectorToLimit, float limitValue){
        return vectorToLimit.normalized() * limitValue;
    }
    void Agent::setVelocity(float speedLimit){
        sf::Vector2f difference = desiredPos - currentPos;
        float distance = difference.length();
        if ((int)distance < 1){
            velocity = {0, 0};
            currentSpeed = 0.f;
            return;
        }
        velocity = difference.normalized();
        if (currentSpeed < speedLimit) {
            currentSpeed += accelerationRate;
        };
        if (currentSpeed > speedLimit){
            currentSpeed = speedLimit;
        };
        velocity = velocity * currentSpeed;
    };
    void Agent::slowDown(){
        currentSpeed -= accelerationRate;
        if (currentSpeed < 0){
            currentSpeed = 0.f;
        }
        velocity = velocity * currentSpeed;
    };
}