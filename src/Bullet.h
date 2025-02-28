#ifndef BULLET_H
#define BULLET_H
#include "BulletType.h"
#include <SFML/Graphics.hpp>

class Bullet final : public sf::Drawable
{
    sf::Sprite sprite;
    float speed;

    public:
        explicit Bullet(const sf::Texture &texture,
                        const float speed,
                        const float scale,
                        const sf::Vector2f &pos,
                        const BulletType bullet_type) : sprite(texture)
        {
            // If it's a player bullet, change the direction of movement
            if (bullet_type == BulletType::Player)
            {
                this->speed = -speed;
            }

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

        void move()
        {
            sprite.move({0.0f, speed});
        }
};

#endif //BULLET_H
