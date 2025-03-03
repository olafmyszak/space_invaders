#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <SFML/Graphics.hpp>

#include "BulletManager.h"

class Spaceship final : public sf::Drawable
{
    const sf::Texture texture;
    sf::Sprite sprite;
    const float speed;
    int lives = 3;

    public:
        Spaceship(const std::filesystem::path &filename,
                  const float speed,
                  const float scale,
                  const sf::Vector2f &pos) : texture(filename), sprite(texture), speed(speed)

        {
            //Set origin to center
            sprite.setOrigin({
                static_cast<float>(texture.getSize().x) / 2.0f, static_cast<float>(texture.getSize().y) / 2.0f
            });

            sprite.setScale({scale, scale});

            sprite.setPosition(pos);
        }

        void draw(sf::RenderTarget &target, const sf::RenderStates states) const override
        {
            target.draw(sprite, states);
        }

        void move_left()
        {
            sprite.move({-speed, 0.0f});
        }

        void move_right()
        {
            // std::cout << "(" << sprite.getOrigin().x << ", " << sprite.getOrigin().y << ")\n";
            sprite.move({speed, 0.0f});
        }

        void shoot(BulletManager &bullet_manager) const
        {
            bullet_manager.addBullet(sprite.getPosition(), BulletType::Player);
        }

        [[nodiscard]] bool handleCollision(const Bullet &bullet)
        {
            auto upper_left = bullet.getUpperLeft();
            auto upper_right = bullet.getUpperRight();

            // Move the pixels by one to account for contains() considering points lying on the edge
            --upper_left.x;
            ++upper_left.y;
            ++upper_right.x;
            ++upper_right.y;

            const bool isHit = sprite.getGlobalBounds().contains(upper_left) || sprite.getGlobalBounds().contains(
                upper_right);

            if (isHit)
            {
                --lives;
            }

            return isHit;
        }

        [[nodiscard]] bool isDead() const
        {
            return lives <= 0;
        }
};

#endif //SPACESHIP_H
