#include <SFML/Graphics.hpp>
#include "CityGrid/CityGrid.hpp"

int main()
{
    constexpr float tileSize = 32.f;
    constexpr unsigned int windowWidth = 800;
    constexpr unsigned int windowHeight = 600;
    constexpr unsigned int gridWidth = (int)windowWidth/tileSize;
    constexpr unsigned int gridHeight = (int)windowHeight/tileSize;
    // create the window
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "My window");

    AutonomousCity::TextureManager textureManager;
    AutonomousCity::CityGrid city(gridWidth, gridHeight, textureManager);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        /** Preparation for adding tiles
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            int x = pos.x / 32;
            int y = pos.y / 32;
        
            AnonymousCity::Tile newTile(
                AnonymousCity::TileType::Road,
                AnonymousCity::TileState::Normal,
                "assets/road.png"
            );
            city.setTile(x, y, newTile);
        }*/

        // clear the window with black color
        window.clear(sf::Color::Black);
        city.draw(window, tileSize);
        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
    }
}