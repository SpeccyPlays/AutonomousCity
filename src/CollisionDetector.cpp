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
    void CollisionDetector::drawLine(sf::Vector2f start, sf::Vector2f end){
         sf::Color lineColor(255, 255, 255);
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