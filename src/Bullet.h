#ifndef BULLET_H
#define BULLET_H

// #include "BulletType.h"
#include <SFML/Graphics.hpp>

class Bullet final : public sf::Drawable
{
    float speed;
    float left_x;
    float right_x;

    public:
        enum class Type
        {
            Player,
            Enemy
        };

        sf::Sprite sprite;

        explicit Bullet(const sf::Texture &texture,
                        const float speed,
                        const sf::Vector2f &scale,
                        const sf::Vector2f &pos,
                        const Type bullet_type) : sprite(texture), bullet_type(bullet_type)
        {
            // If it's a player bullet, change the direction of movement
            if (bullet_type == Type::Player)
            {
                this->speed = -speed;
            }
            else
            {
                this->speed = speed;
            }

            //Set origin to center
            const auto textureSize = texture.getSize();
            sprite.setOrigin({static_cast<float>(textureSize.x) / 2.0f, static_cast<float>(textureSize.y) / 2.0f});

            sprite.setScale(scale);

            sprite.setPosition(pos);

            left_x = sprite.getOrigin().x - static_cast<float>(textureSize.x) / 2.0f + pos.x;
            right_x = sprite.getOrigin().x + static_cast<float>(textureSize.x) / 2.0f + pos.x;
        }

        void draw(sf::RenderTarget &target, const sf::RenderStates states) const override
        {
            target.draw(sprite, states);
        }

        void move(const std::int32_t delta_time)
        {
            const float distance = speed * delta_time;
            sprite.move({0.0f, distance});
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

        Type getBulletType() const
        {
            return bullet_type;
        }

    private:
        Type bullet_type;
};

#endif //BULLET_H
