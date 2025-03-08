#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <fstream>
#include <SFML/Graphics.hpp>
#include <string>

#include "AlienManager.h"
#include "Menu.h"
#include "Spaceship.h"

class GameManager
{
    static constexpr int window_x = 1920;
    static constexpr int window_y = 1080;
    static constexpr int framerate_limit = 144;

    sf::RenderWindow window{
        sf::VideoMode({window_x, window_y}), "Space Invaders", sf::Style::Titlebar | sf::Style::Close
    };

    const sf::Font font{"../../assets/fonts/arial.ttf"};
    Menu menu{font};

    sf::Text player_lives_text{font, "Lives: ", 36};
    sf::Text player_score_text{font, "Score: ", 36};
    sf::Text high_score_text{font, "High Score : ", 36};

    static constexpr float player_bullet_speed = 1.2f;
    static constexpr float enemy_bullet_speed = 0.5f;
    static constexpr sf::Vector2f bullet_scale = {5.0f, 12.5f};
    BulletManager bullet_manager{
        "../../assets/images/bullet.png", 0, window_y, player_bullet_speed, enemy_bullet_speed, bullet_scale
    };

    static constexpr float spaceship_speed = 0.8f;
    static constexpr float spaceship_scale = 4.0f;
    static constexpr sf::Vector2f spaceship_pos = {window_x / 2.0f, window_y - 0.1f * window_y};
    Spaceship spaceship{
        "../../assets/images/spaceship.png", spaceship_speed, spaceship_scale, spaceship_pos, 0.0f, window_x
    };

    static constexpr int alien_move_interval = 500;
    static constexpr float alien_speed = 5.0f;
    static constexpr float alien_step_down = 5.0f;
    static constexpr float alien_scale = 3.0f;
    static constexpr int aliens_row = 5;
    static constexpr int aliens_col = 10;
    const std::vector<AlienManager::TwoTextures> alien_textures = {
        {"../../assets/images/alien3a.png", "../../assets/images/alien3b.png"},
        {"../../assets/images/alien2a.png", "../../assets/images/alien2b.png"},
        {"../../assets/images/alien1a.png", "../../assets/images/alien1b.png"}
    };

    const sf::Texture explosion_texture{"../../assets/images/alienExplosion.png"};
    AlienManager alien_manager{
        alien_textures, explosion_texture, {0.05f * window_x, 0.1f * window_y}, {0.95f * window_x, 0.7f * window_y},
        alien_speed, alien_move_interval, alien_step_down, alien_scale
    };

    int score = 0;

    public:
        GameManager()
        {
            window.setFramerateLimit(framerate_limit);

            player_lives_text.setFillColor(sf::Color::Green);
            player_lives_text.setStyle(sf::Text::Bold);
            player_lives_text.setPosition({0.92f * window_x, 0.0f});

            player_score_text.setFillColor(sf::Color::Green);
            player_score_text.setStyle(sf::Text::Bold);
            player_score_text.setPosition({0.05f, 0.0f});

            high_score_text.setFillColor(sf::Color::Green);
            high_score_text.setStyle(sf::Text::Bold);
            high_score_text.setPosition({0.30f * window_x, 0.0f});
        }

        void run()
        {
            int high_score = 0;

            {
                std::ifstream high_score_file{std::filesystem::path{"../../assets/high_score.txt"}};

                if (!high_score_file)
                {
                    high_score_file.close();

                    std::cout << "Could not open high_score.txt\n";
                    std::cout << "Trying to create it...\n";

                    if (std::ofstream of{std::filesystem::path{"../../assets/high_score.txt"}}; !of)
                    {
                        std::cout << "Error creating high_score.txt! Exiting...\n";
                        return;
                    }

                    std::cout << "high_score.txt created\n";

                    high_score_file.open("../../assets/high_score.txt");
                }

                std::string high_score_input;
                std::getline(high_score_file, high_score_input);

                high_score = high_score_input.empty() ? 0 : std::stoi(high_score_input);
            }

            high_score_text.setString("High Score: " + std::to_string(high_score));

            sf::Clock clock;
            while (window.isOpen())
            {
                std::int32_t delta_time = clock.restart().asMilliseconds();

                // Process events
                while (const std::optional event = window.pollEvent())
                {
                    if (event->is<sf::Event::Closed>())
                    {
                        saveHighScore(high_score);
                        window.close();
                    }
                    else if (const auto *key_pressed = event->getIf<sf::Event::KeyPressed>())
                    {
                        if (key_pressed->scancode == sf::Keyboard::Scan::Escape)
                        {
                            clock.stop();
                            switch (menu.openMainMenu(window))
                            {
                                case Menu::MenuResult::Restart:
                                    restart();
                                    break;

                                case Menu::MenuResult::ClearHighScore:
                                    clearHighScore(&high_score);
                                    high_score_text.setString("High Score: " + std::to_string(high_score));
                                    break;

                                case Menu::MenuResult::Exit:
                                    saveHighScore(high_score);
                                    window.close();
                                    break;

                                default: break;
                            }

                            clock.start();
                        }
                        else if (key_pressed->scancode == sf::Keyboard::Scan::Space)
                        {
                            spaceship.shoot(bullet_manager);
                        }
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
                {
                    spaceship.move_left(delta_time);
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
                {
                    spaceship.move_right(delta_time);
                }

                alien_manager.update(delta_time, bullet_manager);
                // if (alien_manager.move(delta_time))
                // {
                //     // Only shoot on alien move
                //     alien_manager.shoot(bullet_manager);
                // }

                bullet_manager.move(delta_time);

                const bool was_player_hit = handleCollisions();

                // Clear screen
                window.clear();

                // Draw the sprites
                window.draw(spaceship);
                window.draw(bullet_manager);
                window.draw(alien_manager);

                // Draw text
                player_lives_text.setString("Lives: " + std::to_string(spaceship.getLives()));
                window.draw(player_lives_text);

                player_score_text.setString("Score: " + std::to_string(score));
                window.draw(player_score_text);

                window.draw(high_score_text);

                // Update the window
                window.display();

                if (was_player_hit)
                {
                    clock.stop();
                    sf::sleep(sf::seconds(1));
                    clock.start();
                }
            }
        }

    private:
        // Returns true if player was hit
        bool handleCollisions()
        {
            bool was_player_hit = false;
            if (const auto &player_bullet = bullet_manager.player_bullet)
            {
                if (const int value = alien_manager.handleCollision(player_bullet.value()); value != 0)
                {
                    bullet_manager.erasePlayerBullet();
                    score += value;
                }
            }

            for (size_t i = 0, e = bullet_manager.alien_bullets.size(); i < e; ++i)
            {
                if (spaceship.handleCollision(bullet_manager.alien_bullets[i]))
                {
                    bullet_manager.eraseAlienBullet(i);

                    was_player_hit = true;

                    if (spaceship.isDead())
                    {
                        // TODO Player death
                        switch (menu.openGameOverScreen(window))
                        {
                            case Menu::MenuResult::Restart:
                                restart();
                                break;

                            default:
                                window.close();
                                break;
                        }
                    }
                }
            }

            return was_player_hit;
        }

        void restart()
        {
            spaceship.restart();
            bullet_manager.restart();
            alien_manager.restart();
            score = 0;
        }

        void saveHighScore(const int high_score) const
        {
            if (score > high_score)
            {
                if (std::ofstream of{"../../assets/high_score.txt", std::ios::trunc}; !of)
                {
                    std::cout << "Error opening high_score.txt!\n";
                }
                else
                {
                    of << std::to_string(score);
                }
            }
        }

        static void clearHighScore(int *high_score)
        {
            *high_score = 0;

            if (std::ofstream of{"../../assets/high_score.txt", std::ios::trunc}; !of)
            {
                std::cout << "Error opening high_score.txt!\n";
            }
            else
            {
                of << std::to_string(0);
            }
        }
};

#endif //GAMEMANAGER_H
