#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <general.hpp>

namespace
{
    #define CELL_SIZE 8
    #define TITLE "General Map"

    inline sf::Color CellIdToColor(uint8_t id, uint8_t total_count)
    {
        static const uint32_t COLOR_MAX = 255 * 255 * 255;
        uint32_t step = COLOR_MAX / total_count;
        uint32_t color = id * step;
        color <<= 8;
        color |= 0b11111111;

        return sf::Color(color);
    }

    inline void DrawSquare(sf::RenderWindow& window, TSquare const& square, uint32_t col, uint32_t row)
    {
        sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setPosition({ 
            static_cast<float>(col * CELL_SIZE), 
            static_cast<float>(row * CELL_SIZE), 
        });

        sf::Color color = CellIdToColor(square.country_id, 20);

        cell.setFillColor(color);
        window.draw(cell);
    }

    inline void DrawMap(sf::RenderWindow& window, TSquaresArray const& map)
    {
        for(uint32_t row = 0; row < map.Height(); ++row)
        {
            for(uint32_t col = 0; col < map.Width(); ++col)
            {
                DrawSquare(window, map.At(col, row), row, col);
            }
        }
    }
}

inline void ShowMap(General& general)
{
    auto const& map = general.GetGameState().GetSquaresArray();
    sf::RenderWindow window(sf::VideoMode({ CELL_SIZE * map.Width(), CELL_SIZE * map.Height(), }), TITLE);

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
                    window.create(sf::VideoMode({ CELL_SIZE * map.Width(), CELL_SIZE * map.Height() }), TITLE);
                }
            }
        }

        window.clear();

        DrawMap(window, map);

        window.display();
    }
}