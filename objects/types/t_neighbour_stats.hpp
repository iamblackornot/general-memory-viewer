#pragma once

#include <console/out.hpp>
#include <objects/array.hpp>

struct NeighbourStat
{
    uint8_t neighbour_cell_count;
    uint8_t unknown;

    void Print(std::ostream& out) const
    {
        out << std::format("[{}, {}]", this->neighbour_cell_count, this->unknown);
    }

    bool operator==(NeighbourStat const& lhs) const
    {
        return std::tie(lhs.neighbour_cell_count, lhs.unknown) == std::tie(this->neighbour_cell_count, this->unknown);
    }

    bool operator!=(NeighbourStat const& lhs) const
    {
        return std::tie(lhs.neighbour_cell_count, lhs.unknown) != std::tie(this->neighbour_cell_count, this->unknown);
    }
};


class TNeighbourStats: public ObservableArray<NeighbourStat>
{
public:
    void PrintDiff() const
    {
        auto const& print_callback = [&](std::ostream& output, NeighbourStat const& stat) 
        {
            stat.Print(output);
        };

        PrintArrayDiff<NeighbourStat>(std::cout, *this, print_callback);
    }
};