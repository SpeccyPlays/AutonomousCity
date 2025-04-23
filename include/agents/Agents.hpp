#pragma once

#include <SFML/Graphics.hpp>

namespace AutonomousCity {
    class Agent {
        public:
        Agent(sf::Vector2f pos, const std::string& texturePath);
        sf::Vector2f getCurrentPos() const;
        sf::Vector2f getVelocity() const;
        sf::Vector2f getAcceleration() const;

        sf::Vector2f setCurrentPos(sf::Vector2f pos);
        sf::Vector2f setVelocity(sf::Vector2f velocity);
        sf::Vector2f setAcceleration(sf::Vector2f acceleration);

        private:
        sf::Vector2f currentPos;
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        unsigned int mass;
    };
}