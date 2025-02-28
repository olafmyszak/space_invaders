#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <SFML/Graphics.hpp>

#include "Bullet.h"
#include "BulletFactory.h"

class Spaceship final : public sf::Drawable, sf::Transformable
{
    const sf::Texture texture;
    sf::Sprite sprite;
    const float speed;
    const BulletFactory &bullet_factory;

    public:
    std::optional<Bullet> bullet;

        explicit Spaceship(const std::filesystem::path &filename,
                           const float speed,
                           const float scale,
                           const sf::Vector2f &pos,
                           const BulletFactory &bullet_factory) : texture(filename), sprite(texture), speed(speed),
                                                                  bullet_factory(bullet_factory)
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
            sprite.move({speed, 0.0f});
        }

        void shoot()
        {
            if (!bullet.has_value())
            {
                bullet = bullet_factory.createPlayerBullet(5.0f, 5.0f, sprite.getPosition());
            }
        }
};

#endif //SPACESHIP_H
