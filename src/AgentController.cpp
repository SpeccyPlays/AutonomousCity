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
            if (checkBoundary(agent)){
                agent.slowDown();
            } else {
                agent.addWander();
            };
            obsticleDetections(&agent, currentCell.occupants);
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
            if (yPos > height){
                yPos = 0;
            }
            window->draw(sprite);
        };
    };
    bool AgentController::checkBoundary(Agent &agent){
        bool willhitBoundary = false;
        int boundary = 20;

        auto [forward, left, right] = getDirectionalPoints(&agent);
        /*if (nextPos.x < boundary || nextPos.x > width - boundary || nextPos.y < boundary || nextPos.y > height - boundary){
            float steeringAmount = 0.1 - (agent.getCurrentSpeed() * 0.001);
            agent.addSteering(steeringAmount);     
            willhitBoundary = true;
        };*/
        if (forward.x < boundary || forward.x > width - boundary || forward.y < boundary || forward.y > height - boundary){
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
    };
    void AgentController::drawCollisionBox(Agent* agent){
        //get the texture size so we know how big to draw the box
        const sf::Texture &texture = textureManager.getTexture(agent->getTexturePath());
        sf::Sprite sprite(texture);
        sf::Vector2f size = static_cast<sf::Vector2f>(texture.getSize());
        sf::Vector2f origin({size.x /2, size.y /2});
        //work out where to start drawing
        float angle = agent->getAngle();
        sf::Vector2f distance({0, 0});
        //Calculate distance
        float x = std::cos(angle) * size.x; //the textures are facing to the right so x is height
        float y = std::sin(angle) * size.x;
        distance.x = x;
        distance.y = y;
        sf::Vector2f start = agent->getCurrentPos() + distance;
        //Set up the shape
        sf::Color lineColor(255,0,0);
        sf::RectangleShape rectangle({size.x, size.y});
        rectangle.setPosition(start);
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineThickness(2.f);
        rectangle.setOutlineColor(lineColor);
        rectangle.setRotation(sf::radians(agent->getAngle()));
        rectangle.setOrigin(origin);
        //draw
        window->draw(rectangle);
    };
    void AgentController::obsticleDetections(Agent* agent, std::unordered_set<AutonomousCity::Agent *> &occupants){
        //no need to continue if no other agents - the passed in agent has been removed already
        if (occupants.size() < 1){
            return;
        };
        unsigned int multiplier = 2;//we want to at least look our agent size at front but better if increased
        float angleOffset = 0.523599f;//also used for detection later
        const sf::Texture &texture = textureManager.getTexture(agent->getTexturePath());
        sf::Vector2f size = static_cast<sf::Vector2f>(texture.getSize());
        sf::Vector2f distance({0, 0});
        float angle = agent->getAngle();
        sf::Vector2f currentPos = agent->getCurrentPos();
        
        if (debugOn){
            //only calculate left and right positions if we need to
            auto [forward, left, right] = getDirectionalPoints(agent);
            drawLine(currentPos, forward);
            drawLine(currentPos, left);
            drawLine(currentPos, right);
        };
        //chatgpt did the maths for this part
        sf::Vector2f forwardDir = { std::cos(angle), std::sin(angle) };
        float cosThreshold = std::cos(angleOffset);
        for (auto occupant : occupants){
            if (agent != occupant){
                sf::Vector2f occupantPos = occupant->getCurrentPos();
                sf::Vector2f toOccupant = occupantPos - currentPos;//already have agent current position
                
                float distance = std::sqrt(toOccupant.x * toOccupant.x + toOccupant.y * toOccupant.y);
                //avoid dividing by 0
                if (distance == 0.f){
                     continue;
                }
                sf::Vector2f toOccupantDir = toOccupant / distance;

                float dotProduct = forwardDir.x * toOccupantDir.x + forwardDir.y * toOccupantDir.y;
                
                if (dotProduct >= cosThreshold && distance < size.x * multiplier){
                    drawCollisionBox(agent);
                    //remove current speed in future once all agents don't start in the middle
                    if (agent->getCurrentSpeed() > 5){
                        agent->slowDown();
                    }
                }
            };
        };
    };
    std::array<sf::Vector2f, 3> AgentController::getDirectionalPoints(Agent* agent){
        /**
         * These 3 directions will be used quite often for boundary & obsticle checking
         */
        unsigned int multiplier = 2;//we want to at least look our agent size at front but better if increased
        float angleOffset = 0.523599f;//also used for detection later
        const sf::Texture &texture = textureManager.getTexture(agent->getTexturePath());
        sf::Vector2f size = static_cast<sf::Vector2f>(texture.getSize());
        sf::Vector2f distance({0, 0});
        float angle = agent->getAngle();
        sf::Vector2f currentPos = agent->getCurrentPos();
        sf::Vector2f forward({std::cos(angle) * size.x * multiplier, std::sin(angle) * size.x * multiplier});
        sf::Vector2f left({std::cos(angle - angleOffset) * size.x * multiplier, std::sin(angle - angleOffset) * size.x * multiplier});
        sf::Vector2f right ({std::cos(angle + angleOffset) * size.x * multiplier, std::sin(angle + angleOffset) * size.x * multiplier});
        forward += currentPos;
        left += currentPos;
        right += currentPos;
        return {
            forward,
            left,
            right,
        };
    };
};