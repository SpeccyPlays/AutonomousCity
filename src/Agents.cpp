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
        maxspeed = 10;
    };
    void Agent::draw(sf::RenderWindow &window){
        sf::CircleShape agent(mass);
        agent.setOutlineThickness(2);
        agent.setPosition(currentPos);
        agent.setFillColor(sf::Color::Transparent);
        window.draw(agent);
    }
}