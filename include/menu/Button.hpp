#pragma once
#include <SFML/Graphics.hpp>

namespace AutonomousCity {
    struct Button {
        sf::RectangleShape shape;
        sf::Text label;
        bool isSelected;

        Button(const std::string& text, const sf::Font& font, sf::Vector2f pos, sf::Vector2f size)
            : label(font, text, 18),
              isSelected(false)
        {
            shape.setSize(size);
            shape.setPosition(pos);
            shape.setFillColor(sf::Color::Transparent);

            label.setPosition(pos);
            label.setFont(font);
            label.setString(text);
            label.setFillColor(sf::Color::White);
        }

        bool contains(sf::Vector2f point) const {
            return shape.getGlobalBounds().contains(point);
        }
    };
}
