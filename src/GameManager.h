#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>

#include "AlienManager.h"
#include "Spaceship.h"

class GameManager
{
    static constexpr int window_x = 1920;
    static constexpr int window_y = 1080;
    static constexpr int framerate_limit = 60;

    sf::RenderWindow window{
        sf::VideoMode({window_x, window_y}), "Space Invaders", sf::Style::Titlebar | sf::Style::Close
    };

    static constexpr float bullet_speed = 10.0f;
    static constexpr float bullet_scale = 10.0f;
    BulletManager bullet_manager{"../../assets/images/bullet.png", 0, window_y, bullet_speed, bullet_scale};

    static constexpr float spaceship_speed = 5.0f;
    static constexpr float spaceship_scale = 0.15f;
    static constexpr sf::Vector2f spaceship_pos = {window_x / 2.0f, window_y - 0.1f * window_y};
    Spaceship spaceship{"../../assets/images/spaceship.png", spaceship_speed, spaceship_scale, spaceship_pos};

    static constexpr sf::Time alien_time_step = sf::milliseconds(1000);
    static constexpr float alien_speed = 40.0f;
    static constexpr float alien_step_down = 20.0f;
    static constexpr float alien_scale = 0.15f;
    static constexpr int aliens_row = 5;
    static constexpr int aliens_col = 10;
    AlienManager<aliens_row, aliens_col> alien_manager{
        "../../assets/images/alien1.png", {window_x, window_y}, {0.1f * window_x, 0.1f * window_y},
        {0.9f * window_x, 0.7f * window_y}, alien_speed, alien_step_down, alien_scale, aliens_row, aliens_col
    };

    public:
        GameManager()
        {
            window.setFramerateLimit(framerate_limit);
        }

        void run()
        {
            sf::Clock clock;
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
                    spaceship.shoot(bullet_manager);
                }

                bullet_manager.move();

                if (clock.getElapsedTime() >= alien_time_step)
                {
                    alien_manager.move();
                    clock.restart();
                }

                handleCollisions();

                // Clear screen
                window.clear();

                // Draw the sprites
                window.draw(spaceship);
                window.draw(bullet_manager);
                window.draw(alien_manager);

                // Update the window
                window.display();
            }
        }

    private:
        void handleCollisions()
        {
            if (const auto &player_bullet = bullet_manager.player_bullet)
            {
                if (alien_manager.handleCollision(player_bullet.value()))
                {
                    bullet_manager.resetPlayerBullet();
                }
            }
        }
};

#endif //GAMEMANAGER_H
