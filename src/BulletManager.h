#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Bullet.h"

class BulletManager final : public sf::Drawable
{
    const sf::Texture texture;

    const int min_height;
    const int max_height;
    const float player_bullet_speed;
    const float enemy_bullet_speed;
    const sf::Vector2f &bullet_scale;

    public:
        std::vector<Bullet> alien_bullets{};
        std::optional<Bullet> player_bullet{};

        explicit BulletManager(const std::filesystem::path &filename,
                               const int min_height,
                               const int max_height,
                               const float bullet_speed,
                               const float enemy_bullet_speed,
                               const sf::Vector2f &bullet_scale) : texture(filename), min_height(min_height),
                                                                   max_height(max_height),
                                                                   player_bullet_speed(bullet_speed),
                                                                   enemy_bullet_speed(enemy_bullet_speed),
                                                                   bullet_scale(bullet_scale)
        {
        }

        void move()
        {
            for (std::size_t i = 0, e = alien_bullets.size(); i < e; ++i)
            {
                alien_bullets[i].move();
                // std::cout<<"("<<bullets[i].getPosition().x << ", " << bullets[i].getPosition().y << ")\n";

                if (isOutOfBounds(alien_bullets[i]))
                {
                    alien_bullets.erase(alien_bullets.begin() + i);
                }
            }

            if (player_bullet)
            {
                player_bullet->move();
                if (isOutOfBounds(player_bullet.value()))
                {
                    player_bullet.reset();
                }
            }
        }

        void draw(sf::RenderTarget &target, const sf::RenderStates states) const override
        {
            for (auto &&bullet : alien_bullets)
            {
                bullet.draw(target, states);
            }

            if (player_bullet)
            {
                player_bullet->draw(target, states);
            }
        }

        void addBullet(const sf::Vector2f &pos, const BulletType bullet_type)
        {
            switch (bullet_type)
            {
                case BulletType::Player:
                    if (!player_bullet)
                    {
                        player_bullet.emplace(createBullet(pos, player_bullet_speed, bullet_type));
                    }
                    break;

                case BulletType::Enemy:
                    if (canEntityShoot())
                    {
                        alien_bullets.emplace_back(createBullet(pos, enemy_bullet_speed, bullet_type));
                    }
                    break;
            }
        }

        void erasePlayerBullet()
        {
            player_bullet.reset();
        }

        void eraseAlienBullet(const int index)
        {
            alien_bullets.erase(alien_bullets.begin() + index);
        }

    private:
        static constexpr int max_bullets_allowed = 10;

        bool canEntityShoot() const
        {
            return alien_bullets.size() < max_bullets_allowed;
        }

        [[nodiscard]] bool isOutOfBounds(const Bullet &bullet) const
        {
            const int pos_y = static_cast<int>(bullet.getPosition().y);

            return pos_y > max_height || pos_y < min_height;
        }

        [[nodiscard]] Bullet createBullet(const sf::Vector2f &pos,
                                          const float speed,
                                          const BulletType bullet_type) const
        {
            return Bullet(texture, speed, bullet_scale, pos, bullet_type);
        }
};

#endif //BULLETMANAGER_H
