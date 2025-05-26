#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/agents/Agents.hpp"
#include "../include/agents/AgentController.hpp"
#include <vector>
#include <iostream>

namespace AutonomousCity {

    AgentController::AgentController(int amountOfAgents, CityGrid *cityGrid, const unsigned int pxWidth, const unsigned int pxHeight, sf::RenderWindow *renderWindow, TextureManager &manager)
    : textureManager(manager), window(renderWindow), collisionDetector(cityGrid, renderWindow, manager){
        //agents.resize(amountOfAgents);
        grid = cityGrid;
        debugOn = true;
        collisionDetector.setDebug(true);
        for (int i = 0; i < amountOfAgents; i++){
            AutonomousCity::Agent agent(sf::Vector2f(pxWidth /2 , pxHeight / 2), window);
            agents.emplace_back(agent);
            //add agent to the grid
            Agent& addedAgent = agents.back();//do this or it uses local copy instead of the one in the agents vector
            sf::Vector2i startingGridPos = grid->getGridPos(addedAgent.getCurrentPos());
            grid->addAgent(&addedAgent, startingGridPos);
        }
    };
    void AgentController::run(sf::Vector2f desired, float deltaTime){
        for (Agent& agent : agents){
            sf::Vector2i startingGridPos = grid->getGridPos(agent.getCurrentPos());
            AutonomousCity::Cell& currentCell = grid->getCell(startingGridPos);
            if (!grid->removeAgent(&agent, startingGridPos)){
                std::cerr << "Agent probably off the grid" << std::endl;
            };
            if (collisionDetector.checkBoundary(agent)){
                float steeringAmount = 0.1 - (agent.getCurrentSpeed() * 0.001);
                agent.addSteering(steeringAmount);  
                agent.slowDown();
            };
            if (collisionDetector.agentCollision(&agent, currentCell.occupants)){
                //remove current speed in future once all agents don't start in the middle
               if (agent.getCurrentSpeed() > 5){
                    agent.slowDown();
                }
            };
            auto [forwardBlocked, leftBlocked, rightBlocked] = collisionDetector.pathsBlocked(agent);
            if (forwardBlocked){
                float steeringAmount = 0.1 - (agent.getCurrentSpeed() * 0.001);
                if (!leftBlocked && rightBlocked){
                    agent.addSteering(-steeringAmount);
                }
                else if (!rightBlocked && leftBlocked){
                    agent.addSteering(steeringAmount);
                }
                else if (!leftBlocked && !rightBlocked) {
                    if (rand() % 2 == 0) {
                        agent.addSteering(-steeringAmount);
                    }
                    else {
                        agent.addSteering(steeringAmount);
                    }
                }
                else {
                    //Probably completely blocked so try to u-turn out
                    agent.addSteering(steeringAmount * 2);
                    if (agent.getCurrentSpeed() > 5){
                        agent.slowDown();
                    }
                }
            } else if (collisionDetector.laneDisciplineCheck(&agent)){
                //checks if we're going off the right hand side of road and moves to left
                //allows more wander this way
                float steeringBias = 0.002f;
                agent.addSteering(-steeringBias);
            } else {
                agent.addWander();
            };
            
            agent.update(desired, deltaTime);//desired is not actually used
            agent.locomotion(deltaTime);
            sf::Vector2i endingGridPos = grid->getGridPos(agent.getCurrentPos());
            if (!grid->addAgent(&agent, endingGridPos)){
                std::cerr << "Agent probably off the grid" << std::endl;
            };
        };
    };
    void AgentController::setDebug(bool debug){
        debugOn = debug;
        collisionDetector.setDebug(debug);
    };
    bool AgentController::getDebug() const {
        return debugOn;
    };
    void AgentController::draw(){
        float yPos = 0.f;
        for (Agent& agent : agents){
            const sf::Texture &texture = textureManager.getTexture(agent.getTexturePath());
            sf::Sprite sprite(texture);
            sf::Vector2f size = static_cast<sf::Vector2f>(texture.getSize());
            sf::Vector2f origin({size.x /2, size.y /2});
            sprite.setOrigin(origin);
            sprite.setPosition(agent.getCurrentPos());
            sprite.setRotation(sf::radians(agent.getAngle()));
            /*if (debugOn){
                sf::Font font("include/assets/arial.ttf"); 
                sf::Text text(font);
                float x = 0.f;
                float y = yPos;
                text.setPosition({x, y});
                text.setCharacterSize(18);
                text.setFillColor(sf::Color::Red);
                std::stringstream ss;
                sf::Vector2i startingGridPos = grid->getGridPos(agent.getCurrentPos());
                sf::Vector2i endingGridPos = grid->getGridPos(agent.getDesired());
                int startCount = grid->getCell(startingGridPos).occupants.size();
                int desiredCount = grid->getCell(endingGridPos).occupants.size();
                ss << "Start: (" << startingGridPos.x << ", " << startingGridPos.y << ") Occupants: " << startCount << " Desired grid pos: (" << endingGridPos.x << ", " << endingGridPos.y << ") Occupants: " << desiredCount;
                text.setString(ss.str());
                window->draw(text);
            };*/
            yPos += 20.f;
            if (yPos > window->getSize().y){
                yPos = 0;
            }
            window->draw(sprite);
        };
    };
};