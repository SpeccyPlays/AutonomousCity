#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/agents/Agents.hpp"
#include "../include/agents/AgentController.hpp"
#include <vector>
#include <iostream>

namespace AutonomousCity {
    
    AgentController::AgentController(int amountOfAgents, CityGrid *cityGrid, const unsigned int &width, const unsigned int &height, sf::RenderWindow *renderWindow){
        //agents.resize(amountOfAgents);
        grid = cityGrid;
        window = renderWindow;
        debugOn = false;
        //need to use begin and end so i is correct type for vector ilitorator 
        for (int i = 0; i < amountOfAgents; i++){
            AutonomousCity::Agent agent(sf::Vector2f(width /2 , height / 2), window, width, height, true);
            agents.emplace_back(agent);
        }
    };
    void AgentController::run(sf::Vector2f desired, float deltaTime){
        for (Agent& agent : agents){
            //do some stuff
            sf::Vector2i startingGridPos = grid->getGridPos(agent.getCurrentPos());
            agent.update(desired);//desired is not actually used
            agent.locomotion(deltaTime);
            sf::Vector2i endingGridPos = grid->getGridPos(agent.getCurrentPos());
            agent.draw();
            if (startingGridPos != endingGridPos){
                //we've changed cell so update
                if (!grid->removeAgent(&agent, startingGridPos) || !grid->addAgent(&agent, endingGridPos)){
                    std::cerr << "Agent probably off the grid" << std::endl;
                }
            }
        };
    }
    void AgentController::setDebug(bool debug){
        debugOn = debug;
    };
    bool AgentController::getDebug() const {
        return debugOn;
    };
    void AgentController::update(){
        for (Agent& agent : agents){
            //do some stuff
        };
    };
    void AgentController::locomotion(){
        //Move the ants
        for (Agent& agent : agents){
            //do some stuff
        };
    }
    void AgentController::draw(){
        for (Agent& agent : agents){
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