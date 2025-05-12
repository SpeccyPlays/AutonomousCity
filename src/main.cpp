#include <SFML/Graphics.hpp>
#include "CityGrid/CityGrid.hpp"
#include "CityGrid/Tile.hpp"
#include "../include/agents/AgentController.hpp"
#include <iostream>
int main()
{
    sf::Clock deltaClock;
    constexpr float tileSize = 32.f;
    constexpr unsigned int windowWidth = 1280;
    constexpr unsigned int windowHeight = 720;
    constexpr unsigned int gridWidth = (int)windowWidth/tileSize;
    constexpr unsigned int gridHeight = (int)windowHeight/tileSize;
    std::string cityDataPath = "data/city.json";
    const std::string roadPath = "include/assets/road.png";
    std::filesystem::create_directories("data");
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Autonomous City");
    
    AutonomousCity::TextureManager textureManager;
    AutonomousCity::CityGrid city(gridWidth, gridHeight, textureManager, tileSize);
    AutonomousCity::AgentController agents(20, &city, windowWidth, windowHeight, &window, textureManager);

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
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            int x = (int) pos.x / tileSize;
            int y = (int) pos.y / tileSize;
        
            AutonomousCity::Tile newTile(
                AutonomousCity::TileType::Road,
                AutonomousCity::TileState::Empty,
                roadPath,
                &textureManager,
                tileSize
            );
            city.setTile(x, y, newTile);
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