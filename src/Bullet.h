#ifndef BULLET_H
#define BULLET_H

#include "BulletType.h"
#include <SFML/Graphics.hpp>

class Bullet final : public sf::Drawable
{
    sf::Sprite sprite;
    float speed;
    int size_y;
    BulletType bullet_type;

    public:
        explicit Bullet(const sf::Texture &texture,
                        const float speed,
                        const float scale,
                        const sf::Vector2f &pos,
                        const BulletType bullet_type) : sprite(texture), bullet_type(bullet_type)
        {
            // If it's a player bullet, change the direction of movement
            if (bullet_type == BulletType::Player)
            {
                this->speed = -speed;
            }

            //Set origin to center
            const auto textureSize = texture.getSize();
            sprite.setOrigin({static_cast<float>(textureSize.x) / 2.0f, static_cast<float>(textureSize.y) / 2.0f});

            sprite.setScale({scale, scale});

            sprite.setPosition(pos);

            size_y = static_cast<int>(textureSize.y);
        }

        void draw(sf::RenderTarget &target, const sf::RenderStates states) const override
        {
            target.draw(sprite, states);
        }

        void move()
        {
            sprite.move({0.0f, speed});
        }

        int getSizeY() const
        {
            return size_y;
        }

        sf::Vector2f getPosition() const
        {
            return sprite.getPosition();
        }

        BulletType getBulletType() const
        {
            return bullet_type;
        }
};

#endif //BULLET_H
