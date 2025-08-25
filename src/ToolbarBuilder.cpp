#include "../include/menu/ToolbarBuilder.hpp"
#include "../include/menu/Toolbar.hpp"
#include <iostream>

namespace AutonomousCity {
    
    void ToolbarBuilder::buildToolbars(){
        buildFileMenu();        
    };

    void ToolbarBuilder::buildFileMenu(){
        AutonomousCity::Toolbar toolbar;
        if (!this->font.openFromFile("include/assets/arial.ttf")) {
            std::cerr << "Failed to load font" << std::endl;
        }
        sf::Vector2f pos(10, 10);
        //long winded but nevermind
        //add open button
        std::string open = "Open";
        int fontSize = 18;
        sf::Vector2f buttonSize(open.length() * fontSize, 32);
        toolbar.addButton(open, this->font, pos, buttonSize);
        //add Save button
        std::string save = "Save";
        pos.x += buttonSize.x;
        buttonSize.x = save.length() * fontSize;
        toolbar.addButton(save, this->font, pos, buttonSize);
        //add debug button
        std::string debug = "Debug";
        pos.x += buttonSize.x;
        buttonSize.x = debug.length() * fontSize;
        toolbar.addButton(debug, this->font, pos, buttonSize);
        addToolbar(toolbar);
    };
    void ToolbarBuilder::addToolbar(Toolbar toolbar){
        toolbars.emplace_back(toolbar);
    };
    bool ToolbarBuilder::handleClick(sf::Vector2f mousePos){
        for (auto& bar : toolbars){
            if (bar.handleClick(mousePos)) return true;
        };
        return false;
    };
    void ToolbarBuilder::draw(sf::RenderWindow& window){
        for (auto& bar : toolbars){
            bar.draw(window);
        }
    };
}