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

    static constexpr float player_bullet_speed = 15.0f;
    static constexpr float enemy_bullet_speed = 9.0f;
    static constexpr sf::Vector2f bullet_scale = {5.0f, 12.5f};
    BulletManager bullet_manager{
        "../../assets/images/bullet.png", 0, window_y, player_bullet_speed, enemy_bullet_speed, bullet_scale
    };

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

    const std::vector<std::filesystem::path> paths = {
        "../../assets/images/alien3.png", "../../assets/images/alien2.png", "../../assets/images/alien1.png"
    };
    AlienManager alien_manager{
        paths, {window_x, window_y}, {0.1f * window_x, 0.1f * window_y}, {0.9f * window_x, 0.7f * window_y},
        alien_speed, alien_step_down, alien_scale
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

                if (clock.getElapsedTime() >= alien_time_step)
                {
                    alien_manager.move();
                    alien_manager.shoot(bullet_manager);
                    clock.restart();
                }

                bullet_manager.move();

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
                    bullet_manager.erasePlayerBullet();
                }
            }

            for (size_t i = 0, e = bullet_manager.alien_bullets.size(); i < e; ++i)
            {
                if (spaceship.handleCollision(bullet_manager.alien_bullets[i]))
                {
                    bullet_manager.eraseAlienBullet(i);

                    if (spaceship.isDead())
                    {
                        // TODO
                    }
                }
            }
        }
};

#endif //GAMEMANAGER_H
