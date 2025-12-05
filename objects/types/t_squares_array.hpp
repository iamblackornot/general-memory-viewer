#pragma once

#include <cstdint>

#include "console/out.hpp"
#include "objects/layout.hpp"
#include "objects/array.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

struct TSquare
{
    uint8_t country_id;
    uint8_t neighbour_count;
    uint8_t unknown[6];
};

struct TSquaresArray
{
    uint32_t GetMemorySize()
    {
        return size * static_cast<uint32_t>(sizeof(uint32_t));
    }

    bool Update(DWORD process_id, uint32_t address, uint32_t new_size)
    {
        if(new_size != size)
        {
            size = new_size;
            cols.resize(size);
        }

        bool res = col_ptrs.Update(process_id, address, size);

        for(size_t col = 0; col < size && res; ++col)
        {
            res &= cols[col].Update(process_id, col_ptrs.At(col), size);
        }

        return res;
    }

    TSquare const& At(size_t row, size_t col)
    {
        return cols[row].At(col);
    }

    TSquare const& AtPrev(size_t row, size_t col)
    {
        return cols[row].AtPrev(col);
    }

    void PrintMap() const
    {
        std::cout << "\n";
        // std::cout << std::setw(2) << std::setfill('0');

        for(size_t row = 0; row < size; ++row)
        {
            for(size_t col = 0; col < size; ++col)
            {
                std::cout << "[" << std::setw(2) << std::setfill('0') << (int)cols[col].At(row).country_id << "]";
            }

            std::cout << "\n";
        }
    }

    void PrintNeighbourCountMap() const
    {
        std::cout << "\n";

        for(size_t row = 0; row < size; ++row)
        {
            for(size_t col = 0; col < size; ++col)
            {
                std::cout << (int)cols[col].At(row).neighbour_count;
            }

            std::cout << "\n";
        }
    }

    void DrawMap() const
    {
        float const cell_size = 10.f;
        sf::RenderWindow window(sf::VideoMode({(int)cell_size * size, (int)cell_size * size}), "SFML works!");

        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                    window.close();
            }

            window.clear();

            for(size_t row = 0; row < size; ++row)
            {
                for(size_t col = 0; col < size; ++col)
                {
                    //std::cout << (int)cols[col].At(row).country_id;
                    sf::RectangleShape cell(sf::Vector2f(cell_size, cell_size));
                    cell.setPosition({ col * cell_size, row * cell_size });

                    TSquare const& square = cols[col].At(row);
                    sf::Color color = CellIdToColor(square.country_id, 20);

                    // if(square.country_id == 1)
                    // {
                    //     std::cout << "row = " << row << ", col = " << col << "\n";
                    //     //color = sf::Color::White;
                    // }

                    // if(square.neighbour_count < 4)
                    // {
                    //     color.a = 125;
                    // }

                    cell.setFillColor(color);
                    window.draw(cell);
                }
            }

            window.display();
        }
    }

    void PrintColumns() const
    {
        PrintArrayDiff(std::cout, RegionType::POINTER, col_ptrs);
    }

private:
    sf::Color CellIdToColor(uint8_t id, uint8_t total_count) const
    {
        static const uint32_t COLOR_MAX = 255 * 255 * 255;
        uint32_t step = COLOR_MAX / total_count;
        uint32_t color = id * step;
        color <<= 8;
        color |= 0b11111111;

        return sf::Color(color);
    }
private:
    ObservableArray<uint32_t> col_ptrs;
    std::vector<ObservableArray<TSquare>> cols;
    uint32_t size = 0;
};