#include "sfml.hpp"

namespace
{
    inline void DrawCell(sf::RenderWindow& window, Cell const& cell, int row, int col)
    {
        sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        rect.setPosition({ 
            static_cast<float>(col * CELL_SIZE), 
            static_cast<float>(row * CELL_SIZE), 
        });

        sf::Color color = CellIdToColor(cell.country);

        rect.setFillColor(color);
        window.draw(rect);
    }

    inline void DrawMap(sf::RenderWindow& window, Map const& map)
    {
        for(int row = 0; row < map.Height(); ++row)
        {
            for(int col = 0; col < map.Width(); ++col)
            {
                DrawCell(window, map.At(row, col), row, col);
            }
        }
    }

    inline void DrawBorderMap(sf::RenderWindow& window, MapControl& map_control)
    {
        Map const& map = map_control.GetMap();
        std::vector<Border> const& borders = map_control.GetBorders();
        
        for(int country = 0; country < borders.size(); ++country)
        {
            for(const Coords coords : borders[country])
            {
                DrawCell(window, map.At(coords), coords.row, coords.column);
            }
        }
    }
}

void ShowMap(Map const& map)
{
    sf::RenderWindow window(sf::VideoMode({ 
        CELL_SIZE * static_cast<unsigned int>(map.Width()), 
        CELL_SIZE * static_cast<unsigned int>(map.Height()) 
    }), TITLE);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                window.close();
            }
        }

        window.clear();

        DrawMap(window, map);

        window.display();
    }
}

void ShowBorderMap(MapControl& map_control)
{
    Map const& map = map_control.GetMap();

    sf::RenderWindow window(sf::VideoMode({ 
        CELL_SIZE * static_cast<unsigned int>(map.Width()), 
        CELL_SIZE * static_cast<unsigned int>(map.Height()) 
    }), TITLE);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                window.close();
            }
        }

        window.clear();

        DrawBorderMap(window, map_control);

        window.display();
    }
}
