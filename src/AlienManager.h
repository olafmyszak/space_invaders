#ifndef ALIENMANAGER_H
#define ALIENMANAGER_H

#include <algorithm>
#include <functional>
#include <random>
#include <SFML/Graphics.hpp>
#include <utility>

#include "Alien.h"

class AlienManager final : public sf::Drawable
{
    static constexpr int Rows = 5;
    static constexpr int Cols = 10;

    std::vector<std::vector<Alien> > aliens{Rows};
    std::vector<Alien*> exploding_aliens{};

    const sf::Vector2f min_pos;
    const sf::Vector2f max_pos;
    const float alien_speed;
    const int original_move_interval;
    int move_interval;
    std::int32_t move_timer = 0;
    const float alien_step_down;
    const float alien_scale;
    int alive_alien_count = Rows * Cols;
    int texture_step = 0;
    bool all_aliens_dead = false;

    enum class Direction { Left, Right, Down };
    Direction curr_direction = Direction::Right;

    // Random engine for alien shooting
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<std::mt19937::result_type> dist100{1, 100};
    static constexpr int alien_shot_chance = 10;

    public:
        struct TwoTextures
        {
            sf::Texture a;
            sf::Texture b;

            TwoTextures(const std::filesystem::path &file1, const std::filesystem::path &file2) : a(file1), b(file2)
            {
            }
        };

        AlienManager(const std::vector<TwoTextures> &alien_textures,
                     sf::Texture explosion_texture,
                     const sf::Vector2f &min_pos,
                     const sf::Vector2f &max_pos,
                     const float alien_speed,
                     const int time_step,
                     const float alien_step_down,
                     const float alien_scale) : min_pos(min_pos), max_pos(max_pos), alien_speed(alien_speed),
                                                original_move_interval(time_step), move_interval(time_step),
                                                alien_step_down(alien_step_down), alien_scale(alien_scale),
                                                alien_textures(alien_textures),
                                                explosion_texture(std::move(explosion_texture))

        {
            max_tex_size = std::max_element(alien_textures.begin(),
                                            alien_textures.end(),
                                            [](const TwoTextures &a, const TwoTextures &b)
                                            {
                                                return a.a.getSize().x - b.a.getSize().x;
                                            })->a.getSize();

            initAliens();
        }

        void draw(sf::RenderTarget &target, const sf::RenderStates states) const override
        {
            for (auto &&row : aliens)
            {
                for (auto &&alien : row)
                {
                    if (!alien.isDead())
                    {
                        alien.draw(target, states);
                    }
                }
            }
        }

        void update(const std::int32_t delta_time, BulletManager &bullet_manager)
        {
            move_timer += delta_time;

            if (move_timer >= move_interval)
            {
                move(delta_time);
                shoot(bullet_manager);

                for (Alien * exploding_alien : exploding_aliens)
                {
                    exploding_alien->state = Alien::State::Dead;
                }
                exploding_aliens.clear();

                move_timer -= move_interval;
            }
        }

        // Returns true if enough time has passed for aliens to move
        void move(const std::int32_t delta_time)
        {
            const auto maybeAlien = curr_direction == Direction::Left ? findMostLeftAlien() : findMostRightAlien();

            if (!maybeAlien)
            {
                // std::cerr << "Cannot find the most " << (curr_direction == Direction::Left ? "left" : "right") <<
                    // " alien, the vector is empty\n";
                all_aliens_dead = true;
                return;
            }

            const Alien &edge_alien = maybeAlien->get();

            const bool hit_boundary = curr_direction == Direction::Left
                                          ? edge_alien.getPosition().x - max_tex_size.x / 2.0f <= min_pos.x
                                          : edge_alien.getPosition().x + max_tex_size.x / 2.0f >= max_pos.x;

            if (hit_boundary)
            {
                curr_direction = curr_direction == Direction::Left ? Direction::Right : Direction::Left;
                moveAll(delta_time, Direction::Down);
            }
            else
            {
                moveAll(delta_time, curr_direction);
            }

            texture_step = (texture_step + 1) % 2;

            changeTextures();
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
                        bullet_manager.addBullet(pos, Bullet::Type::Enemy);
                    }
                }
            }
        }

        // Returns alien's score value if an alien was hit, 0 otherwise
        // If alien is hit, sets its state to Alien::State::Exploding and changes texture to explosion
        [[nodiscard]] int handleCollision(const Bullet &bullet)
        {
            for (int row = 0; row < Rows; ++row)
            {
                for (int col = 0; col < Cols; ++col)
                {
                    if (Alien &curr_alien = aliens[row][col]; curr_alien.isAlive())
                    {
                        if (curr_alien.checkCollision(bullet))
                        {
                            curr_alien.state = Alien::State::Exploding;
                            curr_alien.setTexture(explosion_texture);
                            exploding_aliens.emplace_back(&curr_alien);
                            --alive_alien_count;

                            // scaled_percentage = min_percentage + current_count / max_count * (max_percentage - min_percentage)
                            const float percentage = 0.50f + static_cast<float>(alive_alien_count) / (Rows * Cols) *
                                0.50f;
                            move_interval = percentage * original_move_interval;

                            return curr_alien.getScore();
                        }
                    }
                }
            }

            return 0;
        }

        void restart()
        {
            initAliens();
            move_interval = original_move_interval;
            alive_alien_count = Rows * Cols;
            all_aliens_dead = false;
        }

        [[nodiscard]] bool allAliensDead() const
        {
            return all_aliens_dead;
        }

    private:
        const std::vector<TwoTextures> alien_textures;
        const sf::Texture explosion_texture;
        sf::Vector2u max_tex_size;

        [[nodiscard]] Alien createAlien(const Alien::Type alien_type, const sf::Vector2f &pos) const
        {
            const TwoTextures &two_textures = alien_textures.at(alienTypeToIndex(alien_type));
            const sf::Texture &tex = two_textures.a;
            return Alien{tex, alien_speed, alien_step_down, alien_scale, pos, alien_type};
        }

        static constexpr int alienTypeToIndex(const Alien::Type type)
        {
            switch (type)
            {
                case Alien::Type::A: return 0;
                case Alien::Type::B: return 1;
                case Alien::Type::C: return 2;
            }
        }

        void moveAll(const std::int32_t delta_time, const Direction direction)
        {
            for (auto &&row : aliens)
            {
                for (auto &&alien : row)
                {
                    if (alien.isAlive())
                    {
                        switch (direction)
                        {
                            case Direction::Left:
                                alien.move_left(delta_time);
                                break;
                            case Direction::Right:
                                alien.move_right(delta_time);
                                break;
                            case Direction::Down:
                                alien.move_down(delta_time);
                                break;

                            default:
                                std::cerr << "No such direction\n";
                                return;
                        }
                    }
                }
            }
        }

        void changeTextures()
        {
            const TwoTextures &two_tex1 = alien_textures.at(0);
            const sf::Texture &tex1 = texture_step == 0 ? two_tex1.a : two_tex1.b;

            // 1 row of As, 2 rows of Bs, 2 rows of Cs
            for (int col = 0; col < Cols; ++col)
            {
                if (aliens[0][col].isAlive())
                {
                    aliens[0][col].setTexture(tex1);
                }
            }

            const TwoTextures &two_tex2 = alien_textures.at(1);
            const sf::Texture &tex2 = texture_step == 0 ? two_tex2.a : two_tex2.b;
            for (int row = 1; row < 3; ++row)
            {
                for (int col = 0; col < Cols; ++col)
                {
                    if (aliens[row][col].isAlive())
                    {
                        aliens[row][col].setTexture(tex2);
                    }
                }
            }

            const TwoTextures &two_tex3 = alien_textures.at(2);
            const sf::Texture &tex3 = texture_step == 0 ? two_tex3.a : two_tex3.b;
            for (int row = 3; row < Rows; ++row)
            {
                for (int col = 0; col < Cols; ++col)
                {
                    if (aliens[row][col].isAlive())
                    {
                        aliens[row][col].setTexture(tex3);
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
                    if (aliens[row][col].isAlive())
                    {
                        return aliens[row][col];
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
                    if (aliens[row][col].isAlive())
                    {
                        return aliens[row][col];
                    }
                }
            }

            return std::nullopt;
        }

        [[nodiscard]] std::optional<std::reference_wrapper<const Alien> > findHighestInColumn(const int col) const
        {
            for (int row = 0; row < Rows; ++row)
            {
                if (aliens[row][col].isAlive())
                {
                    return aliens[row][col];
                }
            }

            return std::nullopt;
        }

        void initAliens()
        {
            for (auto &&vector : aliens)
            {
                vector.clear();
            }

            float curr_x = min_pos.x;
            float curr_y = min_pos.y;

            const float gap_x = max_tex_size.x * alien_scale * 1.6f;
            const float gap_y = max_tex_size.y * alien_scale * 1.5f;

            // 1 row of As, 2 rows of Bs, 2 rows of Cs
            for (int col = 0; col < Cols; ++col)
            {
                aliens.at(0).emplace_back(createAlien(Alien::Type::A, {curr_x, curr_y}));
                curr_x += gap_x;
            }
            curr_y += gap_y;

            for (int row = 1; row < 3; ++row)
            {
                curr_x = min_pos.x;
                for (int col = 0; col < Cols; ++col)
                {
                    aliens.at(row).emplace_back(createAlien(Alien::Type::B, {curr_x, curr_y}));
                    curr_x += gap_x;
                }
                curr_y += gap_y;
            }

            for (int row = 3; row < Rows; ++row)
            {
                curr_x = min_pos.x;
                for (int col = 0; col < Cols; ++col)
                {
                    aliens.at(row).emplace_back(createAlien(Alien::Type::C, {curr_x, curr_y}));
                    curr_x += gap_x;
                }
                curr_y += gap_y;
            }
        }
};

#endif //ALIENMANAGER_H
