#include <SFML/Graphics.hpp>
#include "CityGrid/CityGrid.hpp"
#include "CityGrid/Tile.hpp"
#include "../include/agents/AgentController.hpp"
#include "../include/menu/Toolbar.hpp"
#include <iostream>
int main()
{
    sf::Clock deltaClock;
    constexpr float tileSize = 32.f;
    constexpr unsigned int windowWidth = 1920;
    constexpr unsigned int windowHeight = 990;
    constexpr unsigned int gridWidth = (int)windowWidth/tileSize;
    constexpr unsigned int gridHeight = (int)windowHeight/tileSize;
    std::string cityDataPath = "data/city.json";
    const std::string roadPath = "include/assets/road.png";
    std::filesystem::create_directories("data");
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Autonomous City");
    
    AutonomousCity::TextureManager textureManager;
    AutonomousCity::CityGrid city(gridWidth, gridHeight, textureManager, tileSize);
    AutonomousCity::AgentController agents(50, &city, windowWidth, windowHeight, &window, textureManager);
    
    AutonomousCity::Toolbar toolbar;
    sf::Font font;
    if (!font.openFromFile("include/assets/arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
    sf::Vector2f toolbarStart(10, 10);
    std::vector<std::string> names = {"Open", "Save", "Debug"};
    sf::Vector2f pos = toolbarStart;

    for (size_t i = 0; i < names.size(); i++) {
        sf::Vector2f buttonSize(names[i].length() * 18, tileSize);
        sf::Vector2f gap(buttonSize.x + tileSize, 0);
        toolbar.addButton(names[i], font, pos + gap * (float)i, buttonSize);
    }

    if (city.loadFromFile(cityDataPath)){
        std::cout << "City loaded from: " << cityDataPath << '\n';
    }
    else {
        std::cerr << "Existing city not found at: " << cityDataPath << '\n';
    }

    // run the program as long as the window is open
    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        // Preparation for adding tiles
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

            if (toolbar.handleClick(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))){
                std::cout << "We've clicked the menu" << std::endl;
            }
            else {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                int x = (int) pos.x / tileSize;
                int y = (int) pos.y / tileSize;
            
                AutonomousCity::Tile newTile(
                    AutonomousCity::Tile::TileType::Road,
                    AutonomousCity::TileState::Empty,
                    roadPath,
                    &textureManager,
                    tileSize
                );
                city.setTile(x, y, newTile);
            }
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            int x = (int) pos.x / tileSize;
            int y = (int) pos.y / tileSize;
        
            AutonomousCity::Tile newTile(
                &textureManager
            );
            city.setTile(x, y, newTile);
        }
        // clear the window with black color
        window.clear(sf::Color::Black);
        city.draw(window);
        //draw agents last so they're on top of the grid
        agents.run(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)), deltaTime);
        agents.draw();
        toolbar.draw(window);
        // end the current frame
        window.display();
    };
    if (city.saveToFile(cityDataPath)){
        std::cout << "City saved to: " << cityDataPath << '\n';
    }
    else {
        std::cerr << "Error saving city at: " << cityDataPath << '\n';
    };
}