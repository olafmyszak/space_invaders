#ifndef ALIENMANAGER_H
#define ALIENMANAGER_H

#include <array>
#include <algorithm>
#include <functional>
#include <random>
#include <SFML/Graphics.hpp>

#include "Alien.h"

class AlienManager final : public sf::Drawable
{
    static constexpr int Rows = 5;
    static constexpr int Cols = 10;

    std::vector<sf::Texture> textures;

    const sf::Vector2f window_size;
    const sf::Vector2f min_pos;
    const sf::Vector2f max_pos;
    const float alien_speed;
    const int original_time_step;
    int time_step;
    const float alien_step_down;
    const float alien_scale;
    int alive_alien_count = Rows * Cols;

    enum class Direction { Left, Right };
    Direction curr_direction = Direction::Right;

    // Random engine for alien shooting
    std::mt19937 rng{};
    std::uniform_int_distribution<std::mt19937::result_type> dist100{1, 100};
    static constexpr int alien_shot_chance = 10;

    public:
        std::array<std::array<std::optional<Alien>, Cols>, Rows> aliens{};

        AlienManager(const std::vector<std::filesystem::path> &filenames,
                     const sf::Vector2f &window_size,
                     const sf::Vector2f &min_pos,
                     const sf::Vector2f &max_pos,
                     const float alien_speed,
                     const int time_step,
                     const float alien_step_down,
                     const float alien_scale) : window_size(window_size), min_pos(min_pos), max_pos(max_pos),
                                                alien_speed(alien_speed), original_time_step(time_step),
                                                time_step(time_step), alien_step_down(alien_step_down),
                                                alien_scale(alien_scale)

        {
            // Load all textures
            for (auto &&filename : filenames)
            {
                textures.emplace_back(filename);
            }

            initAliens();

            // Seed with hardware entropy
            rng.seed(std::random_device{}());
        }

        void draw(sf::RenderTarget &target, const sf::RenderStates states) const override
        {
            for (auto &&row : aliens)
            {
                for (auto &&alien : row)
                {
                    if (alien)
                    {
                        alien->draw(target, states);
                    }
                }
            }
        }

        void move()
        {
            const auto maybeAlien = curr_direction == Direction::Left ? findMostLeftAlien() : findMostRightAlien();

            if (!maybeAlien)
            {
                std::cout << "Cannot find the most " << (curr_direction == Direction::Left ? "left" : "right") <<
                    " alien, the array is empty\n";
                return;
            }

            const Alien &edge_alien = maybeAlien->get();

            const float tex_width = textures.at(alienTypeToIndex(edge_alien.alien_type)).getSize().x * alien_scale;
            const float alien_edge_pos = curr_direction == Direction::Left
                                             ? edge_alien.getPosition().x - tex_width
                                             : edge_alien.getPosition().x + tex_width;

            const bool hit_boundary = curr_direction == Direction::Left
                                          ? alien_edge_pos <= 0
                                          : alien_edge_pos >= window_size.x;

            if (hit_boundary)
            {
                curr_direction = curr_direction == Direction::Left ? Direction::Right : Direction::Left;
                moveAllDown();
            }
            else
            {
                curr_direction == Direction::Left ? moveAllLeft() : moveAllRight();
            }
        }

        void shoot(BulletManager &bullet_manager)
        {
            // Each column has a random chance to shoot one bullet
            for (int col = 0; col < Cols; ++col)
            {
                if (dist100(rng) <= alien_shot_chance)
                {
                    if (const std::optional<std::reference_wrapper<const Alien> > maybe_alien =
                        findHighestInColumn(col))
                    {
                        const sf::Vector2f &pos = maybe_alien->get().getPosition();
                        bullet_manager.addBullet(pos, Bullet::BulletType::Enemy);
                    }
                    else
                    {
                        // std::cout << "Empty column " << col << "\n";
                    }
                }
            }
        }

        // Returns alien's score value if an alien was hit, 0 otherwise
        [[nodiscard]] int handleCollision(const Bullet &bullet)
        {
            for (int row = 0; row < Rows; ++row)
            {
                for (int col = 0; col < Cols; ++col)
                {
                    if (aliens[row][col])
                    {
                        if (aliens[row][col]->checkCollision(bullet))
                        {
                            const int result = aliens[row][col]->getScore();
                            aliens[row][col].reset();
                            --alive_alien_count;

                            // scaled_percentage = min_percentage + current_count / max_count * (max_percentage - min_percentage)
                            const float percentage = 0.50f + static_cast<float>(alive_alien_count) / (Rows * Cols) * 0.50f;
                            std::cout<<percentage<< std::endl;

                            time_step = percentage * original_time_step;
                            std::cout<<time_step <<std::endl;

                            return result;
                        }
                    }
                }
            }

            return 0;
        }

        void restart()
        {
            initAliens();
            time_step = original_time_step;
        }

        [[nodiscard]] sf::Time getTimeStep() const
        {
            return sf::milliseconds(time_step);
        }

    private:
        [[nodiscard]] Alien createAlien(const Alien::AlienType alien_type, const sf::Vector2f &pos) const
        {
            return Alien{
                textures.at(alienTypeToIndex(alien_type)), alien_speed, alien_step_down, alien_scale, pos, alien_type
            };
        }

        static constexpr int alienTypeToIndex(const Alien::AlienType type)
        {
            switch (type)
            {
                case Alien::AlienType::A: return 0;
                case Alien::AlienType::B: return 1;
                case Alien::AlienType::C: return 2;
            }
        }

        void moveAllLeft()
        {
            for (auto &&row : aliens)
            {
                for (auto &&alien : row)
                {
                    if (alien)
                    {
                        alien->move_left();
                    }
                }
            }
        }

        void moveAllRight()
        {
            for (auto &&row : aliens)
            {
                for (auto &&alien : row)
                {
                    if (alien)
                    {
                        alien->move_right();
                    }
                }
            }
        }

        void moveAllDown()
        {
            for (auto &&row : aliens)
            {
                for (auto &&alien : row)
                {
                    if (alien)
                    {
                        alien->move_down();
                    }
                }
            }
        }

        [[nodiscard]] std::optional<std::reference_wrapper<const Alien> > findMostLeftAlien() const
        {
            for (int col = 0; col < Cols; ++col)
            {
                for (int row = 0; row < Rows; ++row)
                {
                    if (aliens[row][col].has_value())
                    {
                        return aliens[row][col].value();
                    }
                }
            }

            return std::nullopt;
        }

        [[nodiscard]] std::optional<std::reference_wrapper<const Alien> > findMostRightAlien() const
        {
            for (int col = Cols - 1; col >= 0; --col)
            {
                for (int row = 0; row < Rows; ++row)
                {
                    if (aliens[row][col].has_value())
                    {
                        return aliens[row][col].value();
                    }
                }
            }

            return std::nullopt;
        }

        [[nodiscard]] std::optional<std::reference_wrapper<const Alien> > findHighestInColumn(const int col) const
        {
            for (int row = 0; row < Rows; ++row)
            {
                if (aliens[row][col].has_value())
                {
                    return aliens[row][col].value();
                }
            }

            return std::nullopt;
        }

        void initAliens()
        {
            float curr_x = min_pos.x;
            float curr_y = min_pos.y;

            const sf::Vector2u max_size = std::max_element(textures.begin(),
                                                           textures.end(),
                                                           [](const sf::Texture &a, const sf::Texture &b)
                                                           {
                                                               return a.getSize().x - b.getSize().x;
                                                           })->getSize();

            const float gap_x = max_size.x * alien_scale * 1.2f;
            const float gap_y = max_size.y * alien_scale * 1.0f;

            // 1 row of As, 2 rows of Bs, 2 rows of Cs
            for (int col = 0; col < Cols; ++col)
            {
                // Emplace should destroy the contained value if it exists, so no need to clear this
                aliens[0][col].emplace(createAlien(Alien::AlienType::A, {curr_x, curr_y}));
                curr_x += gap_x;
            }
            curr_y += gap_y;

            for (int row = 1; row < 3; ++row)
            {
                curr_x = min_pos.x;
                for (int col = 0; col < Cols; ++col)
                {
                    aliens[row][col].emplace(createAlien(Alien::AlienType::B, {curr_x, curr_y}));
                    curr_x += gap_x;
                }
                curr_y += gap_y;
            }

            for (int row = 3; row < Rows; ++row)
            {
                curr_x = min_pos.x;
                for (int col = 0; col < Cols; ++col)
                {
                    aliens[row][col].emplace(createAlien(Alien::AlienType::C, {curr_x, curr_y}));
                    curr_x += gap_x;
                }
                curr_y += gap_y;
            }
        }
};

#endif //ALIENMANAGER_H
