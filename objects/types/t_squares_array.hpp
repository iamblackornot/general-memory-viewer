#pragma once

#include <cstdint>

#include "console/out.hpp"
#include "objects/layout.hpp"
#include "objects/array.hpp"

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

        for(uint32_t col = 0; col < size && res; ++col)
        {
            res &= cols[col].Update(process_id, col_ptrs.At(col), size);
        }

        return res;
    }

    TSquare const& At(uint32_t row, uint32_t col) const
    {
        return cols[row].At(col);
    }

    TSquare const& AtPrev(uint32_t row, uint32_t col) const
    {
        return cols[row].AtPrev(col);
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
                std::cout << "[" << std::setw(2) << std::setfill('0') << (int)cols[col].At(row).country_id << "]";
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
                std::cout << (int)cols[col].At(row).neighbour_count;
            }

            std::cout << "\n";
        }
    }

    void PrintColumns() const
    {
        PrintArrayDiff(std::cout, RegionType::POINTER, col_ptrs);
    }

private:
    ObservableArray<uint32_t> col_ptrs;
    std::vector<ObservableArray<TSquare>> cols;
    uint32_t size = 0;
};