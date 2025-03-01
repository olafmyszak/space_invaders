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

    // std::vector<std::vector<Alien> > aliens;
    std::array<std::array<std::optional<Alien>, Cols>, Rows> aliens;

    const sf::Vector2f window_size;
    const sf::Vector2f min_pos;
    const sf::Vector2f max_pos;
    const float alien_speed;
    const float alien_scale;
    const int max_nr_aliens_row;
    const int max_nr_aliens_col;
    float offset_x;
    float offset_y;

    enum class Direction { Left, Right };
    Direction curr_direction = Direction::Right;

    public:
        AlienManager(const std::filesystem::path &filename,
                     const sf::Vector2f &window_size,
                     const sf::Vector2f &min_pos,
                     const sf::Vector2f &max_pos,
                     const float alien_speed,
                     const float alien_scale,
                     const int max_nr_aliens_row,
                     const int max_nr_aliens_col) : texture(filename), window_size(window_size), min_pos(min_pos),
                                                    max_pos(max_pos), alien_speed(alien_speed),
                                                    alien_scale(alien_scale), max_nr_aliens_row(max_nr_aliens_row),
                                                    max_nr_aliens_col(max_nr_aliens_col)
        {
            tex_width = texture.getSize().x * alien_scale;
            tex_height = texture.getSize().y * alien_scale;

            // const float gap_x = (max_pos.x - min_pos.x - max_nr_aliens_col * tex_width) / (max_nr_aliens_row - 1);
            // const float gap_y = (max_pos.y - min_pos.y - max_nr_aliens_row * tex_height) / (max_nr_aliens_col - 1);

            const float gap_x = 50.0f;
            const float gap_y = 20.0f;

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
            if (curr_direction == Direction::Left)
            {
                if (const auto maybeAlien = findMostLeftAlien())
                {
                    if (const Alien &left_most = maybeAlien->get(); (left_most.getPosition().x - tex_width) > 0)
                    {
                        // std::cout<<"left " << left_most.getPosition().x<< '\n';
                        moveAllLeft();
                    }
                    else
                    {
                        curr_direction = Direction::Right;
                    }
                }
                else
                {
                    std::cout << "Cannot find the most left alien, the array is empty\n";
                }
            }
            else
            {
                if (const auto maybeAlien = findMostRightAlien())
                {
                    if (const Alien &right_most = maybeAlien->get(); (right_most.getPosition().x + tex_width) <
                        window_size.x)
                    {
                        // std::cout<<"right " << right_most.getPosition().x<< '\n';
                        moveAllRight();
                    }
                    else
                    {
                        curr_direction = Direction::Left;
                    }
                }
                else
                {
                    std::cout << "Cannot find the most right alien, the array is empty\n";
                }
            }
        }

    private:
        [[nodiscard]] Alien createAlien(const sf::Vector2f &pos) const
        {
            return Alien{texture, alien_speed, alien_scale, pos};
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
