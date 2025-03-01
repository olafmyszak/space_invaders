#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include <algorithm>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Bullet.h"

class BulletManager final : public sf::Drawable
{
    const sf::Texture texture;

    std::vector<Bullet> bullets{};

    const int min_height;
    const int max_height;
    const float bullet_speed;
    const float bullet_scale;

    public:
        explicit BulletManager(const std::filesystem::path &filename,
                               const int min_height,
                               const int max_height,
                               const float bullet_speed,
                               const float bullet_scale) : texture(filename), min_height(min_height),
                                                           max_height(max_height), bullet_speed(bullet_speed),
                                                           bullet_scale(bullet_scale)
        {
        }

        void move()
        {
            for (std::size_t i = 0, e = bullets.size(); i < e; ++i)
            {
                bullets[i].move();
                // std::cout<<"("<<bullets[i].getPosition().x << ", " << bullets[i].getPosition().y << ")\n";

                if (isOutOfBounds(bullets[i]))
                {
                    bullets.erase(bullets.begin() + static_cast<long long>(i));
                }
            }
        }

        void draw(sf::RenderTarget &target, const sf::RenderStates states) const override
        {
            for (auto &&bullet : bullets)
            {
                bullet.draw(target, states);
            }
        }

        void addBullet(const sf::Vector2f &pos, const BulletType bullet_type)
        {
            if (canEntityShoot(bullet_type))
            {
                bullets.emplace_back(createBullet(pos, bullet_type));
            }
        }

    private:
        static constexpr int maxBulletsAllowed(const BulletType bullet_type)
        {
            switch (bullet_type)
            {
                case BulletType::Player:
                    return 1;
                case BulletType::Enemy:
                    return 10;
                default:
                    std::cout << "Invalid bullet type\n";
                    return -1;
            }
        }

        bool canEntityShoot(const BulletType bullet_type)
        {
            return std::count_if(bullets.begin(),
                                 bullets.end(),
                                 [bullet_type](const Bullet &bullet)
                                 {
                                     return bullet.getBulletType() == bullet_type;
                                 }) < maxBulletsAllowed(bullet_type);
        }

        [[nodiscard]] bool isOutOfBounds(const Bullet &bullet) const
        {
            const int pos_y = static_cast<int>(bullet.getPosition().y);

            return pos_y > max_height || pos_y < min_height;
        }

        [[nodiscard]] Bullet createBullet(const sf::Vector2f &pos, const BulletType bullet_type) const
        {
            return Bullet(texture, bullet_speed, bullet_scale, pos, bullet_type);
        }
};

#endif //BULLETMANAGER_H
