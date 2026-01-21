#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <general.hpp>
#include <fwd.hpp>

#include <chrono>
#include <thread>

using namespace std::chrono;

namespace
{
    #define CELL_SIZE 8
    #define TITLE "General Map"

    inline sf::Color CellIdToLinearColor(uint8_t id, uint8_t total_count)
    {
        static const uint32_t COLOR_MAX = 255 * 255 * 255;
        uint32_t step = COLOR_MAX / total_count;
        uint32_t color = id * step;
        color <<= 8;
        color |= 0b11111111;

        return sf::Color(color);
    }

    inline sf::Color CellIdToColor(uint8_t id)
    {
        static constexpr sf::Color id_to_color[]
        {
            sf::Color(255,   0,   0), // Red
            sf::Color(  0,   0, 128), // Navy Blue
            sf::Color(  0, 128,   0), // Green
            sf::Color(  0,   0,   0), // Black
            sf::Color(  0, 128, 128), // Teal
            sf::Color(  0, 255, 255), // Cyan
            sf::Color(128,   0, 128), // Purple
            sf::Color(255, 255,   0), // Yellow
            sf::Color(128, 128,   0), // Olive
            sf::Color(128, 128, 128), // Gray
            sf::Color(  0, 128, 255), // Sky Blue
            sf::Color(255, 128, 255), // Pink
            sf::Color(128, 255, 128), // Light Green
            sf::Color(128, 128, 255), // Lavender Blue
            sf::Color(255, 128,  64), // Orange
            sf::Color(128, 128,  92), // Olive Green
            sf::Color(225, 225, 225), // Light Gray
            sf::Color(255, 128, 128), // Salmon
            sf::Color(255, 255, 200), // Cream
            sf::Color( 64,   0, 128), // Indigo
        };

        if (id < 1 || id > std::size(id_to_color)) 
        {
            return sf::Color::White;
        }

        return id_to_color[id - 1];
    }

    inline void DrawCheckerPattern(sf::RenderWindow& window, sf::RectangleShape& cell)
    {
        const int CHECKER_SIZE = (std::max)(1, CELL_SIZE / 4);
        const int checkers_per_cell = (CELL_SIZE + CHECKER_SIZE - 1) / CHECKER_SIZE;

        for (int i = 0; i < checkers_per_cell; ++i) 
        {
            for (int j = 0; j < checkers_per_cell; ++j) 
            {
                if ((i + j) % 2 == 0) 
                {
                    sf::RectangleShape checker(sf::Vector2f(CHECKER_SIZE, CHECKER_SIZE));
                    checker.setPosition({
                        cell.getPosition().x + j * CHECKER_SIZE,
                        cell.getPosition().y + i * CHECKER_SIZE
                    });
                    
                    sf::Color checkerColor;
                    sf::Color color = cell.getFillColor();
                    float brightness = (color.r * 0.299f + color.g * 0.587f + color.b * 0.114f);
                    
                    if (brightness > 50.0f) {
                        checkerColor = sf::Color(
                            (std::max)(0, color.r - 100),
                            (std::max)(0, color.g - 100),
                            (std::max)(0, color.b - 100)
                        );
                    } 
                    else 
                    {
                        checkerColor = sf::Color(
                            (std::min)(255, color.r + 100),
                            (std::min)(255, color.g + 100),
                            (std::min)(255, color.b + 100)
                        );
                    }
                    
                    checker.setFillColor(checkerColor);
                    window.draw(checker);
                }
            }
        }
    }

    inline void DrawSquare(sf::RenderWindow& window, TSquare const& square, uint32_t row, uint32_t col)
    {
        sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        cell.setPosition({ 
            static_cast<float>(col * CELL_SIZE), 
            static_cast<float>(row * CELL_SIZE), 
        });

        sf::Color color = CellIdToColor(square.country_id);

        cell.setFillColor(color);
        window.draw(cell);

        if(square.mark_for_transfer == 1)
        {
            DrawCheckerPattern(window, cell);
        }
    }

    inline void DrawMap(sf::RenderWindow& window, TSquaresArray const& map)
    {
        for(uint32_t row = 0; row < map.Height(); ++row)
        {
            for(uint32_t col = 0; col < map.Width(); ++col)
            {
                DrawSquare(window, map.At(row, col), row, col);
            }
        }
    }

    inline void DrawAssignedCoords(sf::RenderWindow& window, AssignedCoords const& assigned_coords)
    {
        for(uint8_t i = 0; i < assigned_coords.size(); ++i)
        {
            auto const& country_coords = assigned_coords[i];

            for(auto const& coord: country_coords)
            {
                DrawSquare(window, TSquare{ .country_id = (uint8_t)(i + 1u) }, coord.row, coord.column);
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

inline void ShowAssignedMap(AssignedCoords const& assigned_coords, uint32_t size, std::chrono::milliseconds frame_time = 0ms)
{
    sf::RenderWindow window(sf::VideoMode({ CELL_SIZE * size, CELL_SIZE * size, }), TITLE);

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
                    window.close();
                }
            }
        }

        window.clear();

        DrawAssignedCoords(window, assigned_coords);

        window.display();

        if(frame_time > 0ms)
        {
            std::this_thread::sleep_for(frame_time);
            window.close();
        }
    }
}