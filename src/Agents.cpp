#include "../include/agents/Agents.hpp"
#include <SFML/Graphics.hpp>
#include "../include/CityGrid/CityGrid.hpp"
#include <random>


namespace AutonomousCity {

    Agent::Agent(sf::Vector2f pos, sf::RenderWindow *renderWindow, const unsigned int &width, const unsigned int &height, const std::string& texturePath, bool debugMode)
    :windowWidth(width), windowHeight(height)
    {
        currentPos = pos;
        debugOn = debugMode;
        mass = 10;
        maxspeed = 10;
        wanderingDistance = 20;
        steeringForce = 0.05;
        agentState = AgentState::WANDERING;
        window = renderWindow;
        velocity = {0.f, 0.f};
    };
   Agent::Agent(sf::Vector2f pos, sf::RenderWindow *renderWindow, const unsigned int &width, const unsigned int &height, bool debugMode)
    :windowWidth(width), windowHeight(height)
   {
        currentPos = pos;
        debugOn = debugMode;
        mass = 10;
        maxspeed = 100;
        wanderingDistance = 20;
        steeringForce = 0.05;
        agentState = AgentState::WANDERING;
        window = renderWindow;
        velocity = {0.f, 0.f};
    };
    void Agent::update(sf::Vector2i desired){
        switch(agentState){
            case AgentState::WANDERING:
                wandering();
                break;
            default:
                setDesired(desired);
                break;
        }
        checkBoundary();
        setVelocity(maxspeed);
    }
    void Agent::wandering(){
        sf::Vector2f tempDesired = currentPos + (velocity * maxspeed);
        //all this to generate a random angle and distance
        std::random_device randomAngle;
        std::uniform_real_distribution<float> angleRange(0.0f, 360.0f);
        sf::Angle angle = sf::degrees(angleRange(randomAngle));
        float radians = angle.asRadians();
        std::random_device randomDistance;
        std::uniform_real_distribution<float> dist(maxspeed / 2, maxspeed);
        float distance = dist(randomDistance);
        if (debugOn){
            sf::Color lineColor(255, 255, 255);
            sf::Vector2f endPoint = tempDesired + sf::Vector2f({distance, distance});
            std::array<sf::Vertex, 2> line = {
                sf::Vertex{sf::Vector2f(currentPos), lineColor},
                sf::Vertex{sf::Vector2f(endPoint), lineColor}};
            window->draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }
        tempDesired.x += distance * cos(radians);
        tempDesired.y += distance * sin(radians);
        setDesired(static_cast<sf::Vector2i>(tempDesired));
    }
    bool Agent::checkBoundary(){
        bool willhitBoundary = false;
        float boundary = maxspeed / 2;
        sf::Vector2f nextPos = currentPos + velocity;
        if (nextPos.x < boundary ){
            velocity += sf::Vector2f(steeringForce * maxspeed, 0.f);
            setDesired(static_cast<sf::Vector2i>(currentPos + velocity));
            willhitBoundary = true;
        }
        else if (nextPos.x > windowWidth - boundary){
            velocity += sf::Vector2f(-steeringForce * maxspeed, 0.f);
            setDesired(static_cast<sf::Vector2i>(currentPos + velocity));
            willhitBoundary = true;
        }
        if (nextPos.y < boundary){        
            velocity += sf::Vector2f(0.f, steeringForce * maxspeed);
            setDesired(static_cast<sf::Vector2i>(currentPos + velocity));
            willhitBoundary = true;
        }
        else if (nextPos.y > windowHeight - boundary){
            velocity += sf::Vector2f(0.f, -steeringForce * maxspeed);
            setDesired(static_cast<sf::Vector2i>(currentPos + velocity));
            willhitBoundary = true;
        }
        return willhitBoundary;
    }
    void Agent::draw(){
        sf::CircleShape agent(mass/ 2);
        agent.setOrigin({mass /2, mass / 2});
        agent.setOutlineThickness(2);
        agent.setPosition(currentPos);
        agent.setFillColor(sf::Color::Transparent);
        window->draw(agent);
        if (debugOn){
            sf::Color lineColor(255, 255, 255);
            sf::Vector2f endPoint = currentPos + velocity;
            std::array<sf::Vertex, 2> line = {
                sf::Vertex{sf::Vector2f(currentPos), lineColor},
                sf::Vertex{sf::Vector2f(endPoint), lineColor}};
            window->draw(line.data(), line.size(), sf::PrimitiveType::Lines);
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
            window->draw(text);
        };
    };
    void Agent::locomotion(float deltaTime){
        currentPos += velocity * deltaTime;
    };
    void Agent::setDesired(sf::Vector2i desired){
        desiredPos = static_cast<sf::Vector2f>(desired);
    };
    void Agent::addVelocity(sf::Vector2f toAdd){
        velocity += toAdd;
    }
    sf::Vector2f Agent::limitVector(sf::Vector2f vectorToLimit, float limitValue){
        return vectorToLimit.normalized() * limitValue;
    }
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
    void Agent::setVelocity(float speedLimit){
        sf::Vector2f difference = desiredPos - currentPos;
        float distance = difference.length();
        if ((int)distance < 1){
            velocity = {0, 0};
            return;
        }
        velocity = difference.normalized();
        if ((int)distance > speedLimit) {
            velocity *= speedLimit;
        } else {
            slowDown(distance);
        }
    };
    void Agent::slowDown(float distance){
        float decelerationFactor = distance / maxspeed;
        velocity *= maxspeed * decelerationFactor;
    };
}