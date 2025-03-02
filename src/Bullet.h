#ifndef BULLET_H
#define BULLET_H

#include "BulletType.h"
#include <SFML/Graphics.hpp>

class Bullet final : public sf::Drawable
{
    float speed;
    BulletType bullet_type;
    float left_x;
    float right_x;

    public:
        sf::Sprite sprite;

        explicit Bullet(const sf::Texture &texture,
                        const float speed,
                        const sf::Vector2f &scale,
                        const sf::Vector2f &pos,
                        const BulletType bullet_type) : bullet_type(bullet_type), sprite(texture)
        {
            // If it's a player bullet, change the direction of movement
            if (bullet_type == BulletType::Player)
            {
                this->speed = -speed;
            }

            //Set origin to center
            const auto textureSize = texture.getSize();
            sprite.setOrigin({static_cast<float>(textureSize.x) / 2.0f, static_cast<float>(textureSize.y) / 2.0f});

            sprite.setScale(scale);

            sprite.setPosition(pos);

            left_x = sprite.getOrigin().x - static_cast<float>(textureSize.x)  / 2.0f + pos.x;
            right_x = sprite.getOrigin().x + static_cast<float>(textureSize.x)  / 2.0f + pos.x;
        }

        void draw(sf::RenderTarget &target, const sf::RenderStates states) const override
        {
            target.draw(sprite, states);
        }

        void move()
        {
            sprite.move({0.0f, speed});
        }

        sf::Vector2f getPosition() const
        {
            return sprite.getPosition();
        }

        sf::Vector2f getUpperLeft() const
        {
            return {left_x, sprite.getPosition().y};
        }

        sf::Vector2f getUpperRight() const
        {
            return {right_x, sprite.getPosition().y};
        }

        BulletType getBulletType() const
        {
            return bullet_type;
        }
};

#endif //BULLET_H
