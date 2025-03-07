#ifndef MENU_H
#define MENU_H

#include <filesystem>
#include <utility>
#include <vector>

#include <SFML/Graphics.hpp>

class Menu
{
    const sf::Font font;
    const std::array<const std::string, 4> menu_items = {"Resume", "Restart", "Clear High Score", "Exit"};
    std::vector<sf::Text> menu_texts{};
    static constexpr float spacing = 60.0f;
    static constexpr int char_size = 36;

    public:
        enum class MenuResult
        {
            Resume,
            Restart,
            ClearHighScore,
            Exit
        };

        explicit Menu(const std::filesystem::path &filename) : font(filename)
        {
        }

        explicit Menu(sf::Font font) : font(std::move(font))
        {
        }

        MenuResult openMainMenu(sf::RenderWindow &window)
        {
            const float window_center_x = window.getSize().x / 2.0f;
            float y_pos = 0.2f * window.getSize().y;

            menu_texts.clear();
            for (const auto &menu_item : menu_items)
            {
                y_pos += spacing;

                menu_texts.emplace_back(createCenteredText(font, menu_item, char_size, window_center_x, y_pos));
            }

            unsigned int selected = 0;
            while (window.isOpen())
            {
                // Process events
                while (const std::optional event = window.pollEvent())
                {
                    if (event->is<sf::Event::Closed>())
                    {
                        window.close();
                    }
                    else if (const auto *key_pressed = event->getIf<sf::Event::KeyPressed>())
                    {
                        if (key_pressed->scancode == sf::Keyboard::Scan::Escape)
                        {
                            // Close the menu
                            return MenuResult::Resume;
                        }

                        if (key_pressed->scancode == sf::Keyboard::Scan::Down)
                        {
                            selected = (selected + 1) % 4;
                        }
                        else if (key_pressed->scancode == sf::Keyboard::Scan::Up)
                        {
                            selected = (selected - 1) % 4;
                        }
                        else if (key_pressed->scancode == sf::Keyboard::Scan::Space || key_pressed->scancode ==
                            sf::Keyboard::Scan::Enter)
                        {
                            switch (selected)
                            {
                                case 0:
                                    return MenuResult::Resume;
                                case 1:
                                    return MenuResult::Restart;
                                case 2:
                                    return MenuResult::ClearHighScore;
                                case 3:
                                    return MenuResult::Exit;

                                default:
                                    std::cout << "No such menu item\n";
                                    break;
                            }
                        }
                    }
                }

                window.clear();

                for (size_t i = 0, e = menu_texts.size(); i < e; ++i)
                {
                    auto text = menu_texts[i];

                    if (i == selected)
                    {
                        text.setString(">" + text.getString());
                    }

                    window.draw(text);
                }

                window.display();
            }

            return MenuResult::Exit;
        }

        MenuResult openGameOverScreen(sf::RenderWindow &window)
        {
            const float window_center_x = window.getSize().x / 2.0f;
            float y_pos = 0.2f * window.getSize().y;

            menu_texts.clear();

            menu_texts.emplace_back(createCenteredText(font, "Restart?", char_size, window_center_x, y_pos));
            y_pos += spacing;
            menu_texts.emplace_back(createCenteredText(font, "Exit", char_size, window_center_x, y_pos));

            int selected = 0;
            while (window.isOpen())
            {
                // Process events
                while (const std::optional event = window.pollEvent())
                {
                    if (event->is<sf::Event::Closed>())
                    {
                        window.close();
                    }
                    else if (const auto *key_pressed = event->getIf<sf::Event::KeyPressed>())
                    {
                        if (key_pressed->scancode == sf::Keyboard::Scan::Down)
                        {
                            selected = (selected + 1) % 2;
                        }
                        else if (key_pressed->scancode == sf::Keyboard::Scan::Up)
                        {
                            selected = (selected - 1) % 2;
                        }
                        else if (key_pressed->scancode == sf::Keyboard::Scan::Space || key_pressed->scancode ==
                            sf::Keyboard::Scan::Enter)
                        {
                            switch (selected)
                            {
                                case 0:
                                    return MenuResult::Restart;
                                case 1:
                                    return MenuResult::Exit;

                                default:
                                    std::cout << "No such menu item\n";
                                    break;
                            }
                        }
                    }
                }

                window.clear();

                for (size_t i = 0, e = menu_texts.size(); i < e; ++i)
                {
                    auto text = menu_texts[i];

                    if (i == selected)
                    {
                        text.setString(">" + text.getString());
                    }
                    window.draw(text);
                }

                window.display();
            }

            return MenuResult::Exit;
        }

    private:
        static sf::Text createCenteredText(const sf::Font &font,
                                           const std::string &str,
                                           const unsigned int charSize,
                                           float x,
                                           float y)
        {
            sf::Text text{font, str, charSize};
            text.setFillColor(sf::Color::Green);
            text.setStyle(sf::Text::Bold);

            // Compute local bounds and adjust origin for proper centering
            const sf::FloatRect &localBounds = text.getLocalBounds();
            text.setOrigin({
                localBounds.position.x + localBounds.size.x / 2.0f, localBounds.position.y + localBounds.size.y / 2.0f
            });
            text.setPosition({x, y});
            return text;
        }
};

#endif //MENU_H
