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
    const sf::Vector2f original_pos;
    const float min_x;
    const float max_x;

    float half_tex_size;

    public:
        Spaceship(const std::filesystem::path &filename,
                  const float speed,
                  const float scale,
                  const sf::Vector2f &pos,
                  const float min_x,
                  const float max_x): texture(filename), sprite(texture), speed(speed), original_pos(pos), min_x(min_x),
                                      max_x(max_x)

        {
            //Set origin to center
            sprite.setOrigin({
                static_cast<float>(texture.getSize().x) / 2.0f, static_cast<float>(texture.getSize().y) / 2.0f
            });

            sprite.setScale({scale, scale});

            sprite.setPosition(pos);

            half_tex_size = texture.getSize().x * scale / 2.0f;
        }

        void draw(sf::RenderTarget &target, const sf::RenderStates states) const override
        {
            target.draw(sprite, states);
        }

        void move_left(const std::int32_t delta_time)
        {
            if (sprite.getPosition().x - half_tex_size >= min_x)
            {
                const float distance = -speed * delta_time;
                sprite.move({distance, 0.0f});
            }
        }

        void move_right(const std::int32_t delta_time)
        {
            if (sprite.getPosition().x + half_tex_size <= max_x)
            {
                const float distance = speed * delta_time;
                sprite.move({distance, 0.0f});
            }
        }

        void shoot(BulletManager &bullet_manager) const
        {
            bullet_manager.addBullet(sprite.getPosition(), Bullet::BulletType::Player);
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

        [[nodiscard]] int getLives() const
        {
            return lives;
        }

        void restart()
        {
            lives = 3;
            sprite.setPosition(original_pos);
        }
};

#endif //SPACESHIP_H
