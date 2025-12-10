#pragma once

#include <cstdint>
#include <format>

#include "console/out.hpp"
#include "objects/layout.hpp"
#include "objects/observable.hpp"

struct TCoords
{
    uint8_t column;
    uint8_t row;

    void Print(std::ostream& output) const
    {
        output << std::format("({}, {})", column, row);
    }

    bool operator==(TCoords const& lhs) const
    {
        return std::tie(lhs.column, lhs.row) == std::tie(this->column, this->row);
    }

    bool operator!=(TCoords const& lhs) const
    {
        return std::tie(lhs.column, lhs.row) != std::tie(this->column, this->row);
    }
};

struct TCoordsArray
{
    void PrintDiff() const
    {
        auto const& print_callback = [&](std::ostream& output, TCoords const& coords) 
        {
            coords.Print(output);
        };

        PrintArrayDiff<TCoords>(std::cout, coords, print_callback);
    }

    uint32_t GetMemorySize()
    {
        return Size() * static_cast<uint32_t>(sizeof(uint32_t));
    }

    bool Update(DWORD process_id, uint32_t address, uint32_t new_size)
    {
        return coords.Update(process_id, address, new_size);
    }

    TCoords const& At(uint32_t index) const
    {
        return coords.At(index);
    }

    TCoords const& AtPrev(uint32_t index) const
    {
        return coords.AtPrev(index);
    }

    uint32_t Size() const
    {
        return coords.Size();
    }

private:
    ObservableArray<TCoords> coords;
};