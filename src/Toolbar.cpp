#include "../include/menu/Toolbar.hpp"

namespace AutonomousCity {

    void Toolbar::addButton(const std::string& text, const sf::Font& font, sf::Vector2f pos, sf::Vector2f size) {
        buttons.emplace_back(text, font, pos, size);
    }

    void Toolbar::draw(sf::RenderWindow& window) {
        for (auto& b : buttons) {
            window.draw(b.shape);
            window.draw(b.label);
        }
    }

    bool Toolbar::handleClick(sf::Vector2f mousePos) {
        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i].contains(mousePos)) return true;
        }
        return false;
    }
}
