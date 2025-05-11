#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/agents/Agents.hpp"
#include "../include/agents/AgentController.hpp"
#include <vector>
#include <iostream>

namespace AutonomousCity {

    AgentController::AgentController(int amountOfAgents, CityGrid *cityGrid, const unsigned int pxWidth, const unsigned int pxHeight, sf::RenderWindow *renderWindow, TextureManager &manager)
    : textureManager(manager){
        //agents.resize(amountOfAgents);
        grid = cityGrid;
        window = renderWindow;
        debugOn = true;
        width = pxWidth;
        height = pxHeight;
        for (int i = 0; i < amountOfAgents; i++){
            AutonomousCity::Agent agent(sf::Vector2f(width /2 , height / 2), window, width, height);
            agents.emplace_back(agent);
        }
    };
    void AgentController::run(sf::Vector2f desired, float deltaTime){
        for (Agent& agent : agents){
            if (!checkBoundary(agent)){
                agent.addWander();
            };
            agent.update(desired, deltaTime);//desired is not actually used
            sf::Vector2i startingGridPos = grid->getGridPos(agent.getDesired());
            AutonomousCity::Cell& currentCell = grid->getCell(startingGridPos);
            if (currentCell.occupants.size() > 0){
                agent.slowDown();
            };
            agent.locomotion(deltaTime);
            sf::Vector2i endingGridPos = grid->getGridPos(agent.getCurrentPos());
            //agent.draw();
            if (startingGridPos != endingGridPos){
                //we've changed cell so update
                AutonomousCity::Cell& newCell = grid->getCell(endingGridPos);
                if (newCell.occupants.size() > 1){
                    agent.slowDown();
                }
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
            const sf::Texture &texture = textureManager.getTexture(agent.getTexturePath());
            sf::Sprite sprite(texture);
            sf::Vector2f size = static_cast<sf::Vector2f>(texture.getSize());
            sf::Vector2f origin({size.x /2, size.y /2});
            sprite.setOrigin(origin);
            sprite.setPosition(agent.getCurrentPos());
            sprite.setRotation(sf::radians(agent.getAngle()));
            window->draw(sprite);
            if (debugOn){
                drawLine(agent.getCurrentPos(), agent.getCurrentPos() + agent.getVelocity());
            };
        };
    };
    bool AgentController::checkBoundary(Agent &agent){
        bool willhitBoundary = false;
        int boundary = 20;
        sf::Vector2f nextPos = agent.getCurrentPos() + agent.getVelocity();
        float steeringAmount = 0.05;
        if (nextPos.x < boundary){
            agent.addSteering(steeringAmount);     
            willhitBoundary = true;
        }
        else if (nextPos.x > width - boundary){
            agent.addSteering(steeringAmount);
            willhitBoundary = true;
        }
        if (nextPos.y < boundary){     
            agent.addSteering(steeringAmount);
            willhitBoundary = true;
        }
        else if (nextPos.y > height - boundary){
            agent.addSteering(steeringAmount);
            willhitBoundary = true;
        }
        return willhitBoundary;
    };
    void AgentController::drawLine(sf::Vector2f start, sf::Vector2f end){
         sf::Color lineColor(255, 255, 255);
         std::array<sf::Vertex, 2> line = {
            sf::Vertex{sf::Vector2f(start), lineColor},
            sf::Vertex{sf::Vector2f(end), lineColor}
         };
         window->draw(line.data(), line.size(), sf::PrimitiveType::Lines);
    }
}