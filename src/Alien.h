#ifndef ALIEN_H
#define ALIEN_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "BulletManager.h"
#include "Utils.h"

class Alien final : public sf::Drawable
{
    sf::Sprite sprite;
    float speed;
    float step_down;

    public:
        enum class Type : int
        {
            A = 40,
            B = 20,
            C = 10
        };

        enum class State
        {
            Alive,
            Exploding,
            Dead
        };

        enum class Direction
        {
            Left,
            Right,
            Down
        };

        const Type alien_type;
        State state = State::Alive;

        Alien(const sf::Texture &texture,
              const float speed,
              const float step_down,
              const float scale,
              const sf::Vector2f &pos,
              const Type alien_type) : sprite(texture), speed(speed), step_down(step_down), alien_type(alien_type)
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

        void move(const std::int32_t delta_time, const Direction direction)
        {
            switch (direction)
            {
                case Direction::Left:
                    move_left(delta_time);
                    break;
                case Direction::Right:
                    move_right(delta_time);
                    break;
                case Direction::Down:
                    move_down(delta_time);
                    break;

                default:
                    std::cerr << "No such direction\n";
                    break;
            }
        }

        void shoot(BulletManager &bullet_manager) const
        {
            bullet_manager.addBullet(sprite.getPosition(), Bullet::Type::Enemy);
        }

        [[nodiscard]] sf::Vector2f getPosition() const
        {
            return sprite.getPosition();
        }

        bool checkCollision(const Bullet &bullet) const
        {
            auto upper_left = bullet.getUpperLeft();
            auto upper_right = bullet.getUpperRight();

            // Move the pixels by one to account for contains() not considering points lying on the edge
            --upper_left.x;
            ++upper_left.y;
            ++upper_right.x;
            ++upper_right.y;

            return sprite.getGlobalBounds().contains(upper_left) || sprite.getGlobalBounds().contains(upper_right);
        }

        [[nodiscard]] int getScore() const
        {
            return static_cast<int>(alien_type);
        }

        void setTexture(const sf::Texture &texture)
        {
            sprite.setTexture(texture);
        }

        [[nodiscard]] bool isAlive() const
        {
            return state == State::Alive;
        }

        [[nodiscard]] bool isDead() const
        {
            return state == State::Dead;
        }

    private:
        void move_left(const std::int32_t delta_time)
        {
            const float distance = -speed * delta_time;
            sprite.move({distance, 0.0f});
        }

        void move_right(const std::int32_t delta_time)
        {
            const float distance = speed * delta_time;
            sprite.move({distance, 0.0f});
        }

        void move_down(const std::int32_t delta_time)
        {
            const float distance = step_down * delta_time;
            sprite.move({0.0f, distance});
        }
};

#endif //ALIEN_H
