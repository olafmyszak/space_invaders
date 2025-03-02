#ifndef ALIENMANAGER_H
#define ALIENMANAGER_H

#include <array>
#include <functional>
#include <SFML/Graphics.hpp>

#include "Alien.h"

template<int Rows, int Cols>
class AlienManager final : public sf::Drawable
{
    const sf::Texture texture;
    float tex_width;
    float tex_height;

    const sf::Vector2f window_size;
    const sf::Vector2f min_pos;
    const sf::Vector2f max_pos;
    const float alien_speed;
    const float alien_step_down;
    const float alien_scale;
    const int max_nr_aliens_row;
    const int max_nr_aliens_col;
    float offset_x;
    float offset_y;

    enum class Direction { Left, Right };
    Direction curr_direction = Direction::Right;

    public:
        std::array<std::array<std::optional<Alien>, Cols>, Rows> aliens;

        AlienManager(const std::filesystem::path &filename,
                     const sf::Vector2f &window_size,
                     const sf::Vector2f &min_pos,
                     const sf::Vector2f &max_pos,
                     const float alien_speed,
                     const float alien_step_down,
                     const float alien_scale,
                     const int max_nr_aliens_row,
                     const int max_nr_aliens_col) : texture(filename), window_size(window_size), min_pos(min_pos),
                                                    max_pos(max_pos), alien_speed(alien_speed),
                                                    alien_step_down(alien_step_down), alien_scale(alien_scale),
                                                    max_nr_aliens_row(max_nr_aliens_row),
                                                    max_nr_aliens_col(max_nr_aliens_col)
        {
            tex_width = texture.getSize().x * alien_scale;
            tex_height = texture.getSize().y * alien_scale;

            // const float gap_x = (max_pos.x - min_pos.x - max_nr_aliens_col * tex_width) / (max_nr_aliens_row - 1);
            // const float gap_y = (max_pos.y - min_pos.y - max_nr_aliens_row * tex_height) / (max_nr_aliens_col - 1);

            constexpr float gap_x = 50.0f;
            constexpr float gap_y = 20.0f;

            offset_x = gap_x + tex_width;
            offset_y = gap_y + tex_height;

            float curr_y = min_pos.y;

            for (int row = 0; row < max_nr_aliens_row; ++row)
            {
                float curr_x = min_pos.x;
                for (int col = 0; col < max_nr_aliens_col; ++col)
                {
                    aliens[row][col] = createAlien({curr_x, curr_y});

                    curr_x += offset_x;
                }

                curr_y += offset_y;
            }
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

        [[nodiscard]] bool handleCollision(const Bullet &bullet)
        {
            for (int row = 0; row < Rows; ++row)
            {
                for (int col = 0; col < Cols; ++col)
                {
                    if (aliens[row][col])
                    {
                        if (aliens[row][col]->checkCollision(bullet))
                        {
                            aliens[row][col].reset();
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        void killAlien(int row, int col)
        {
            aliens[row][col].reset();
        }

    private:
        [[nodiscard]] Alien createAlien(const sf::Vector2f &pos) const
        {
            return Alien{texture, alien_speed, alien_step_down, alien_scale, pos};
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
};

#endif //ALIENMANAGER_H
