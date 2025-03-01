#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <SFML/Graphics.hpp>

#include "BulletManager.h"

class Spaceship final : public sf::Drawable
{
    const sf::Texture texture;
    sf::Sprite sprite;
    const float speed;

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
};

#endif //SPACESHIP_H
