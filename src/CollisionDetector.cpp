#include <SFML/Graphics.hpp>
#include "../include/agents/CollisionDetector.hpp"
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/agents/Agents.hpp"
#include "../include/agents/AgentController.hpp"
#include <vector>
#include <iostream>

namespace AutonomousCity {
    CollisionDetector::CollisionDetector(CityGrid *cityGrid, sf::RenderWindow *renderWindow, TextureManager &manager) 
    : grid(cityGrid), window(renderWindow), textureManager(manager){};
    bool CollisionDetector::checkBoundary(Agent& agent){
        /**
         *Determine if we're heading off the render window  
         */
        bool willhitBoundary = false;
        int boundary = 20;
        sf::Vector2u windowDimensions = window->getSize();
        auto [forward, left, right] = getDirectionalPoints(&agent);
        if (forward.x < boundary || forward.x > windowDimensions.x - boundary || forward.y < boundary || forward.y > windowDimensions.y - boundary){  
            willhitBoundary = true;
        };
        return willhitBoundary;
    };
    bool CollisionDetector::agentCollision(Agent* agent, std::unordered_set<AutonomousCity::Agent *> &occupants){
        //no need to continue if no other agents - the passed in agent has been removed already
        if (occupants.size() < 1){
            return false;
        };
        float angleOffset = 0.523599f;//also used for detection later
        const sf::Texture &texture = textureManager.getTexture(agent->getTexturePath());
        sf::Vector2f size = static_cast<sf::Vector2f>(texture.getSize());
        float multiplier = calcLookAheadMultipler(agent->getCurrentSpeed(), size.x);
        float angle = agent->getAngle();
        sf::Vector2f currentPos = agent->getCurrentPos();
        
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
                
                if (dotProduct >= cosThreshold && distance < multiplier){
                    drawCollisionBox(agent);
                    return true;
                }
            };
        };
        return false;
    };
    std::array<bool, 3> CollisionDetector::pathsBlocked(Agent& agent){

        auto [forward, left, right] = getDirectionalPoints(&agent);
        
        sf::Vector2i forwardGridPos = grid->getGridPos(forward);
        sf::Vector2i leftGridPos = grid->getGridPos(left);
        sf::Vector2i rightGridPos = grid->getGridPos(right);
        //check if we're off grid first although shouldn't be
        bool forwardBlocked = offGrid(forwardGridPos);
        bool leftBlocked = offGrid(leftGridPos);
        bool rightBlocked = offGrid(rightGridPos);
        sf::Color forwardLine(255, 255, 255);
        sf::Color leftLine(255, 255, 255);
        sf::Color rightLine(255, 255, 255);
        //Red line = offgrid
        //Magenta line = no road
        if (forwardBlocked){
            forwardLine = sf::Color::Red;
        }
        if (forwardBlocked == false){
            forwardBlocked = notRoadCheck(forwardGridPos);
            if (forwardBlocked){
                forwardLine = sf::Color::Magenta;
            }
        };
        if (leftBlocked){
            leftLine = sf::Color::Red;
        }
        if (leftBlocked == false){
            leftBlocked = notRoadCheck(leftGridPos);
            if (leftBlocked){
                leftLine = sf::Color::Magenta;
            }
        };
        if (rightBlocked){
            rightLine = sf::Color::Red;
        }
        if (rightBlocked == false){
            rightBlocked = notRoadCheck(rightGridPos);
            if (rightBlocked){
                rightLine = sf::Color::Magenta;
            }
        };
        if (debugOn){
            sf::Vector2f currentPos = agent.getCurrentPos();
            drawLine(currentPos, forward, forwardLine);
            drawLine(currentPos, left, leftLine);
            drawLine(currentPos, right, rightLine);
        };
        return {
            forwardBlocked,
            leftBlocked,
            rightBlocked
        };
    };
    bool CollisionDetector::offGrid(sf::Vector2i gridPos){
        unsigned int gridWidth = grid->getWidth();
        unsigned int gridHeight = grid->getHeight();
        if (gridPos.x >= static_cast<int>(gridWidth) || gridPos.y >= static_cast<int>(gridHeight) || gridPos.x < 0 || gridPos.y < 0){
            return true;
        }
        return false;
    };
    bool CollisionDetector::notRoadCheck(sf::Vector2i gridPos){
        if (grid->getTile(gridPos.x, gridPos.y).getType() != AutonomousCity::TileType::Road){
            return true;
        }
        return false;
    };
    bool CollisionDetector::laneDisciplineCheck(Agent* agent){
        sf::Vector2f sidePoint = getSidePoint(agent);
        if (debugOn){
            sf::Vector2f currentPos = agent->getCurrentPos();
            drawLine(currentPos, sidePoint, sf::Color::Green);
        }
        sf::Vector2i gridPos = grid->getGridPos(sidePoint);
        return notRoadCheck(gridPos);
    };
    sf::Vector2f CollisionDetector::getSidePoint(Agent* agent){
        float sideAngle = 0.785398f; //90 degrees in radians
        const sf::Texture &texture = textureManager.getTexture(agent->getTexturePath());
        sf::Vector2f size = static_cast<sf::Vector2f>(texture.getSize());
        float angle = agent->getAngle();
        angle += sideAngle;
        sf::Vector2f currentPos = agent->getCurrentPos();
        sf::Vector2f sidePos({std::cos(angle) * size.x * 2, std::sin(angle) * size.x * 2});
        return sidePos += currentPos;
    };
    std::array<sf::Vector2f, 3> CollisionDetector::getDirectionalPoints(Agent* agent){
        /**
         * These 3 directions will be used quite often for boundary & obsticle checking
         */
        float angleOffset = 0.523599f;//also used for detection later
        const sf::Texture &texture = textureManager.getTexture(agent->getTexturePath());
        sf::Vector2f size = static_cast<sf::Vector2f>(texture.getSize());
        float multiplier = calcLookAheadMultipler(agent->getCurrentSpeed(), size.x);
        float angle = agent->getAngle();
        sf::Vector2f currentPos = agent->getCurrentPos();
        sf::Vector2f forward({std::cos(angle) * multiplier, std::sin(angle) * multiplier});
        sf::Vector2f left({std::cos(angle - angleOffset) * multiplier, std::sin(angle - angleOffset) * multiplier});
        sf::Vector2f right ({std::cos(angle + angleOffset) * multiplier, std::sin(angle + angleOffset) * multiplier});
        forward += currentPos;
        left += currentPos;
        right += currentPos;
        return {
            forward,
            left,
            right,
        };
    };
    void CollisionDetector::drawLine(sf::Vector2f start, sf::Vector2f end, sf::Color lineColor){
         std::array<sf::Vertex, 2> line = {
            sf::Vertex{sf::Vector2f(start), lineColor},
            sf::Vertex{sf::Vector2f(end), lineColor}
         };
         window->draw(line.data(), line.size(), sf::PrimitiveType::Lines);
    };
    void CollisionDetector::drawCollisionBox(Agent* agent){
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
    float CollisionDetector::calcLookAheadMultipler(float currentSpeed, float size){
        /**
         * For collision detection, we want to be looking ahead at least the size amount, further depending on speed
         */
        float lookAheadAmount = 0.02f;

        return (currentSpeed * lookAheadAmount) + size * 2; //need double the size distance so agents don't overlap
    };
    void CollisionDetector::setDebug(bool debug){
        debugOn = debug;
    };
};