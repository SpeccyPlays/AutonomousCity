#include "../include/agents/Agents.hpp"
#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"

namespace AutonomousCity {

    Agent::Agent(sf::Vector2f pos, const std::string& texturePath, bool debugMode){
        currentPos = pos;
        debugOn = debugMode;
        mass = 10;
        maxspeed = 10;
    };
   Agent::Agent(sf::Vector2f pos, bool debugMode){
        currentPos = pos;
        debugOn = debugMode;
        mass = 10;
        maxspeed = 100;
    };
    void Agent::draw(sf::RenderWindow &window){
        sf::CircleShape agent(mass);
        agent.setOrigin({mass, mass});
        agent.setOutlineThickness(2);
        agent.setPosition(currentPos);
        agent.setFillColor(sf::Color::Transparent);
        window.draw(agent);
        if (debugOn){
            sf::Color lineColor(255, 255, 255);
            sf::Vector2f endPoint = currentPos + velocity;
            std::array<sf::Vertex, 2> line = {
                sf::Vertex{sf::Vector2f(currentPos), lineColor},
                sf::Vertex{sf::Vector2f(endPoint), lineColor}};
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }
        
    };
    void Agent::locomotion(float deltaTime){
        currentPos += velocity * deltaTime;
    };
    void Agent::setDesired(sf::Vector2i desired){
        desiredPos = static_cast<sf::Vector2f>(desired);
    };
    void Agent::setVelocity(){
        sf::Vector2f difference = desiredPos - currentPos;
        float distance = difference.length();
        if (distance < 1.f){
            velocity = {0, 0};
            return;
        }
        velocity = difference.normalized();
        if (distance > maxspeed) {
            velocity *= maxspeed;
        } else {
            float decelerationFactor = distance / maxspeed;
            velocity *= maxspeed * decelerationFactor;
        }
    }
}