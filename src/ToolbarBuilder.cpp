
#include "../include/menu/ToolbarBuilder.hpp"
#include "../include/menu/Toolbar.hpp"
#include "../include/CityGrid/CityGrid.hpp"
#include "../include/agents/AgentController.hpp"
#include <iostream>

namespace AutonomousCity {
    
    void ToolbarBuilder::buildToolbars(AutonomousCity::CityGrid* cityGrid, AutonomousCity::AgentController* agentController){
        buildFileMenu(cityGrid, agentController);        
    };
    void ToolbarBuilder::buildFileMenu(AutonomousCity::CityGrid* cityGrid, AutonomousCity::AgentController* agentController){
        AutonomousCity::Toolbar toolbar;
        if (!this->font.openFromFile("include/assets/arial.ttf")) {
            std::cerr << "Failed to load font" << std::endl;
        }

        sf::Vector2f pos(10, 10);
        int fontSize = 18;
        struct ButtonInfo { sf::String name; std::function<void()> action; };
        std::vector<ButtonInfo> buttons = {
            {"Open", [cityGrid]() { cityGrid->loadFromMenu(); }},
            {"Save", [cityGrid]() { cityGrid->saveFromMenu(); }},
            {"Debug", [agentController]() { agentController->toggleDebug(); }}
        };
        for (auto& b : buttons) {
            sf::Text tmp(this->font, b.name, fontSize);
            sf::FloatRect bounds = tmp.getLocalBounds();
            sf::Vector2f buttonSize(bounds.size.x + 20, bounds.size.y + 10);
            toolbar.addButton(b.name, this->font, pos, buttonSize, b.action);
            pos.x += buttonSize.x;
        }
        addToolbar(toolbar);
    };
    void ToolbarBuilder::addToolbar(Toolbar toolbar){
        toolbars.emplace_back(toolbar);
    };
    bool ToolbarBuilder::handleClick(sf::Vector2f mousePos){
        for (auto& bar : toolbars){
            int clicked = bar.handleClick(mousePos);
            if (clicked != -1){
                bar.buttons[clicked].trigger();
                return true;
            }
        };
        return false;
    };
    void ToolbarBuilder::draw(sf::RenderWindow& window){
        for (auto& bar : toolbars){
            
            for (auto &btn : bar.buttons){
                sf::RectangleShape outline(btn.shape);
                outline.setFillColor(sf::Color::Transparent);
                outline.setOutlineThickness(1);
                outline.setOutlineColor(sf::Color::Red);
                window.draw(outline);
            }
            bar.draw(window);
        }
    };
}