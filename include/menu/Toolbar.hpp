#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Button.hpp"

namespace AutonomousCity {
    class Toolbar {
    public:
        std::vector<Button> buttons;

        void addButton(const std::string& text, const sf::Font& font, sf::Vector2f pos, sf::Vector2f size, std::function<void()> callback = nullptr);
        void draw(sf::RenderWindow& window);
        int handleClick(sf::Vector2f mousePos);
    };
}

