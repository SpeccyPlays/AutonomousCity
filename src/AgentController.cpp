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
            if (currentCell.occupants.size() > 1 && agent.getCurrentSpeed() > 10){
                agent.slowDown();
            };
            if (checkBoundary(agent)){
                agent.slowDown();
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
        float yPos = 0.f;
        for (Agent& agent : agents){
            const sf::Texture &texture = textureManager.getTexture(agent.getTexturePath());
            sf::Sprite sprite(texture);
            sf::Vector2f size = static_cast<sf::Vector2f>(texture.getSize());
            sf::Vector2f origin({size.x /2, size.y /2});
            sprite.setOrigin(origin);
            sprite.setPosition(agent.getCurrentPos());
            sprite.setRotation(sf::radians(agent.getAngle()));
            if (debugOn){
                drawLine(agent.getCurrentPos(), agent.getCurrentPos() + agent.getVelocity());
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
            };
            yPos += 20.f;
            if (yPos > height){
                yPos = 0;
            }
            window->draw(sprite);
        };
    };
    bool AgentController::checkBoundary(Agent &agent){
        bool willhitBoundary = false;
        int boundary = 20;
        sf::Vector2f nextPos = agent.getCurrentPos() + agent.getVelocity();
        if (nextPos.x < boundary || nextPos.x > width - boundary || nextPos.y < boundary || nextPos.y > height - boundary){
            float steeringAmount = 0.1 - (agent.getCurrentSpeed() * 0.001);
            agent.addSteering(steeringAmount);     
            willhitBoundary = true;
        };
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