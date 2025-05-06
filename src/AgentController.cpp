#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/agents/Agents.hpp"
#include "../include/agents/AgentController.hpp"
#include <vector>

namespace AutonomousCity {
    
    AgentController::AgentController(int amountOfAgents, CityGrid *cityGrid, sf::RenderWindow *renderWindow){
        agents.resize(amountOfAgents);
        grid = cityGrid;
        window = renderWindow;
        debugOn = false;
    };
    void AgentController::setDebug(bool debug){
        debugOn = debug;
    };
    bool AgentController::getDebug() const {
        return debugOn;
    };
    void AgentController::update(){
        for (Agent agent : agents){
            //do some stuff
        };
    };
    void AgentController::locomotion(){
        //Move the ants
        for (Agent agent : agents){
            //do some stuff
        };
    }
    void AgentController::draw(){
        for (Agent agent : agents){
            //do some stuff
            sf::CircleShape obj(5);
            obj.setOrigin({5, 5});
            obj.setOutlineThickness(2);
            obj.setPosition(agent.getCurrentPos());
            obj.setFillColor(sf::Color::Transparent);
            window->draw(obj);
            if (debugOn){
                sf::Color lineColor(255, 255, 255);
                sf::Vector2f endPoint = agent.getCurrentPos() + agent.getVelocity();
                std::array<sf::Vertex, 2> line = {
                    sf::Vertex{sf::Vector2f(agent.getCurrentPos()), lineColor},
                    sf::Vertex{sf::Vector2f(endPoint), lineColor}};
                window->draw(line.data(), line.size(), sf::PrimitiveType::Lines);
            };
        };
    };
}