#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

namespace AutonomousCity {
    struct Button {
        sf::RectangleShape shape;
        sf::Text label;
        bool isSelected;
        std::function<void()> onClick;
        Button(const std::string& text, const sf::Font& font, sf::Vector2f pos, sf::Vector2f size, std::function<void()> callback = nullptr)
            : label(font, text, 18),
              isSelected(false),
              onClick(callback)
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
        void trigger() {
            if (onClick) onClick();
        }
    };
}
