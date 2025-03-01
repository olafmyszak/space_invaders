#ifndef ALIEN_H
#define ALIEN_H

#include <SFML/Graphics.hpp>

#include "BulletManager.h"

class Alien final : public sf::Drawable
{
    sf::Sprite sprite;
    float speed;

    public:
        Alien(const sf::Texture &texture,
              const float speed,
              const float scale,
              const sf::Vector2f &pos) : sprite(texture), speed(speed)
        {
            //Set origin to center
            sprite.setOrigin({
                static_cast<float>(texture.getSize().x) / 2.0f, static_cast<float>(texture.getSize().y) / 2.0f
            });

            sprite.setScale({scale, scale});

            sprite.setPosition(pos);
        }

        // explicit Alien(Alien &&other) noexcept : sprite(other.sprite), speed(other.speed)
        // {
        // }

        // Alien &operator=(Alien other)
        // {
        //     std::swap(sprite, other.sprite);
        //     std::swap(speed, other.speed);
        //     return *this;
        // }

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
            sprite.move({speed, 0.0f});
        }

        void shoot(BulletManager &bullet_manager) const
        {
            bullet_manager.addBullet(sprite.getPosition(), BulletType::Enemy);
        }

        [[nodiscard]] sf::Vector2f getPosition() const
        {
            return sprite.getPosition();
        }
};

#endif //ALIEN_H
