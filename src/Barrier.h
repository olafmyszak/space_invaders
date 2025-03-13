#ifndef BARRIER_H
#define BARRIER_H

#include <SFML/Graphics.hpp>

#include "Bullet.h"

class Barrier final : public sf::Drawable
{
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    const float scale;

    static constexpr float bullet_hit_radius = 50.0f;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<std::mt19937::result_type> dist100{1, 100};

    public:
        Barrier(const std::filesystem::path &path, const float scale, const sf::Vector2f &pos) : image(path),
            texture(path), sprite(texture), scale(scale)
        {
            //Set origin to center
            sprite.setOrigin({
                static_cast<float>(texture.getSize().x) / 2.0f, static_cast<float>(texture.getSize().y) / 2.0f
            });

            sprite.setScale({scale, scale});
            sprite.setPosition(pos);

            sprite.setOrigin({0.0, 0.0});
        }

        void draw(sf::RenderTarget &target, sf::RenderStates states) const override
        {
            target.draw(sprite, states);
        }

        bool handleCollision(const Bullet &bullet)
        {
            const auto pixel = sf::Vector2u((bullet.sprite.getPosition() - sprite.getPosition()) / scale);

            if (!imageContainsPixel(pixel))
            {
                return false;
            }

            // If pixel is transparent
            if (image.getPixel(pixel).a == 0)
            {
                return false;
            }

            const int range = bullet_hit_radius / scale;

            for (int x = -range / 2; x <= range / 2; ++x)
            {
                for (int y = -range / 2; y <= range / 2; ++y)
                {
                    if (dist100(rng) <= 50)
                    {
                        if (const sf::Vector2u curr = {pixel.x + x, pixel.y + y}; imageContainsPixel(curr))
                        {
                            image.setPixel(curr, sf::Color::Transparent);
                        }
                    }
                }
            }

            if (!texture.loadFromImage(image))
            {
                std::cerr << "Error loading barrier texture from image\n";
            }

            return true;
        }

        bool imageContainsPixel(const sf::Vector2u pixel) const
        {
            if (pixel.x >= image.getSize().x || pixel.y >= image.getSize().y)
            {
                return false;
            }

            return true;
        }
};

#endif //BARRIER_H
