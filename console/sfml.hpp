#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <general.hpp>

inline sf::Color CellIdToColor(uint8_t id, uint8_t total_count)
{
    static const uint32_t COLOR_MAX = 255 * 255 * 255;
    uint32_t step = COLOR_MAX / total_count;
    uint32_t color = id * step;
    color <<= 8;
    color |= 0b11111111;

    return sf::Color(color);
}

inline void ShowMap(General& general)
{
    static uint32_t const cell_size = 10;
    static sf::String const title = "General Map";

    auto const& map = general.GetGameState().GetSquaresArray();
    sf::RenderWindow window(sf::VideoMode({ cell_size * map.Width(), cell_size * map.Height(), }), title);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    general.Update();
                    window.create(sf::VideoMode({ cell_size * map.Width(), cell_size * map.Height() }), title);
                }
            }
            
        }

        window.clear();

        for(uint32_t row = 0; row < map.Height(); ++row)
        {
            for(uint32_t col = 0; col < map.Width(); ++col)
            {
                //std::cout << (int)cols[col].At(row).country_id;
                sf::RectangleShape cell(sf::Vector2f(cell_size, cell_size));
                cell.setPosition({ 
                    static_cast<float>(col * cell_size), 
                    static_cast<float>(row * cell_size), 
                });

                TSquare const& square = map.At(col, row);
                sf::Color color = CellIdToColor(square.country_id, 20);

                cell.setFillColor(color);
                window.draw(cell);
            }
        }

        window.display();
    }
}