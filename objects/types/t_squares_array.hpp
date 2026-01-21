#pragma once

#include <cstdint>

#include "console/out.hpp"
#include "objects/layout.hpp"
#include "objects/array.hpp"

struct TSquare
{
    static MemoryLayout const& GetMemoryLayout()
    {
        static MemoryLayout layout
        {
            LayoutRegion(0u, 1u, RegionType::INTEGER, "country_id"),
            LayoutRegion(1u, 1u, RegionType::INTEGER, "same_neighbour_count"),
            LayoutRegion(2u, 1u, RegionType::INTEGER, "unknown_2"),
            LayoutRegion(3u, 1u, RegionType::INTEGER, "mark_for_transfer"),

            LayoutRegion(4u, 1u, RegionType::INTEGER, "unknown_4"),
            LayoutRegion(5u, 1u, RegionType::INTEGER, "unknown_5"),
            LayoutRegion(6u, 1u, RegionType::INTEGER, "unknown_6"),
            LayoutRegion(7u, 1u, RegionType::INTEGER, "unknown_7"),         
        };

        return layout;
    }

    static uint32_t GetMemorySize()
    {
        MemoryLayout const& layout = TSquare::GetMemoryLayout();

        if(layout.empty()) { return 0; }

        auto const& last_region = layout.back();
        return last_region.offset + last_region.size;
    }

    uint8_t country_id;
    uint8_t same_neighbour_count;
    uint8_t unknown_1;
    uint8_t mark_for_transfer;

    uint8_t unknown[4];
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

        for(uint32_t col = 0; col < size && res; ++col)
        {
            res &= cols[col].Update(process_id, col_ptrs.At(col), size);
        }

        return res;
    }

    TSquare const& At(uint32_t row, uint32_t col) const
    {
        return cols[col].At(row);
    }

    TSquare const& AtPrev(uint32_t row, uint32_t col) const
    {
        return cols[col].AtPrev(row);
    }

    uint32_t Width() const
    {
        return cols.size();
    }

    uint32_t Height() const
    {
        if(cols.empty()) return 0u;
            
        return cols[0].Size();
    }

    void PrintMap() const
    {
        std::cout << "\n";
        // std::cout << std::setw(2) << std::setfill('0');

        for(uint32_t row = 0; row < size; ++row)
        {
            for(uint32_t col = 0; col < size; ++col)
            {
                //std::cout << "[" << std::setw(2) << std::setfill('0') << (int)cols[col].At(row).country_id << "]";
                PrintColoredSquare(std::cout, cols[col].At(row));
            }

            std::cout << "\n";
        }
    }

    void PrintNeighbourCountMap() const
    {
        std::cout << "\n";

        for(uint32_t row = 0; row < size; ++row)
        {
            for(uint32_t col = 0; col < size; ++col)
            {
                std::cout << (int)cols[col].At(row).same_neighbour_count;
            }

            std::cout << "\n";
        }
    }

    void PrintColumns() const
    {
        PrintArrayDiff(std::cout, RegionType::POINTER, col_ptrs);
    }

    void PrintSquareData(uint32_t row, uint32_t col) const
    {
        TSquare const& square = cols[col].At(row);
        TSquare const& square_prev = cols[col].AtPrev(row);
        uint32_t address = col_ptrs.At(col) + row * TSquare::GetMemorySize();

        PrintMemoryLayoutDiff(
            std::cout,
            TSquare::GetMemoryLayout(),
            reinterpret_cast<uint8_t const*>(&square),
            reinterpret_cast<uint8_t const*>(&square_prev),
            address
        );
    }
private:
    inline void PrintColoredSquare(std::ostream& out, TSquare const& square) const
    {
        static constexpr const std::string_view id_to_terminal_color_256[]
        {
            BG_RED_256,
            BG_NAVY_BLUE_256,
            BG_GREEN_256,
            BG_BLACK_256,
            BG_TEAL_256,
            BG_CYAN_256,
            BG_PURPLE_256,
            BG_YELLOW_256,
            BG_OLIVE_256,
            BG_GRAY_256,
            BG_SKY_BLUE_256,
            BG_PINK_256,
            BG_LIGHT_GREEN_256,
            BG_LAVENDER_256,
            BG_ORANGE_256,
            BG_OLIVE_GREEN_256,
            BG_LIGHT_GRAY_256,
            BG_SALMON_256,
            BG_CREAM_256,
            BG_INDIGO_256,
        };

        out << id_to_terminal_color_256[square.country_id - 1] << (square.mark_for_transfer ? "▓▓" : "  ") << COLOR_RESET;
    }

private:
    ObservableArray<uint32_t> col_ptrs;
    std::vector<ObservableArray<TSquare>> cols;
    uint32_t size = 0;
};