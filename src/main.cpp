#include <SFML/Graphics.hpp>
#include "Spaceship.h"

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Space Invaders");
    window.setFramerateLimit(60);

    const float window_x = static_cast<float>(window.getSize().x);
    const float window_y = static_cast<float>(window.getSize().y);

    // // Load textures
    // const sf::Texture alien1_texture("../../assets/images/alien1.png");
    // const sf::Texture alien2_texture("../../assets/images/alien2.png");
    // const sf::Texture alien3_texture("../../assets/images/alien3.png");
    // const sf::Texture spaceship_texture("../../assets/images/spaceship.png");
    // const sf::Texture bullet_texture("../../assets/images/bullet.png");

    constexpr float scale = 0.2f;
    const sf::Vector2f pos = {window_x / 2.0f, window_y - 0.1f * window_y};

    const BulletFactory bullet_factory{"../../assets/images/bullet.png"};
    Spaceship spaceship{"../../assets/images/spaceship.png", 5.0f, scale, pos, bullet_factory};

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
        {
            spaceship.move_left();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
        {
            spaceship.move_right();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
        {
            spaceship.shoot();
        }

        for (auto & bullet : spaceship.bullets)
        {
            bullet.move();
        }

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(spaceship);

        // Update the window
        window.display();
    }
}
