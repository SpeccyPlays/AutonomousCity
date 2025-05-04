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
        sf::CircleShape agent(mass/ 2);
        agent.setOrigin({mass /2, mass / 2});
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
            window.draw(text);
        };
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
        if ((int)distance < 1){
            velocity = {0, 0};
            return;
        }
        velocity = difference.normalized();
        if ((int)distance > maxspeed) {
            velocity *= maxspeed;
        } else {
            slowDown(distance);
        }
    };
    void Agent::slowDown(float distance){
        float decelerationFactor = distance / maxspeed;
        velocity *= maxspeed * decelerationFactor;
    };
}