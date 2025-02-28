#ifndef BULLETFACTORY_H
#define BULLETFACTORY_H

#include <filesystem>
#include <SFML/Graphics.hpp>

#include "Bullet.h"

class BulletFactory
{
    const sf::Texture texture;

    public:
        explicit BulletFactory(const std::filesystem::path &filename) : texture(filename)
        {
        }

        Bullet createPlayerBullet(const float speed, const float scale, const sf::Vector2f &pos) const
        {
            return Bullet(texture, speed, scale, pos, BulletType::Player);
        }

        Bullet createEnemyBullet(const float speed, const float scale, const sf::Vector2f &pos) const
        {
            return Bullet(texture, speed, scale, pos, BulletType::Enemy);
        }
};

#endif //BULLETFACTORY_H
