#pragma once

#include <vector>

#include "grid.hpp"

struct Coords
{
    int row;
    int column;

    Coords() = default;
    Coords( int row, int column) : row(row), column(column) {}

    bool operator==(Coords const& rhs) const
    {
        return std::tie(this->column, this->row) == std::tie(rhs.column, rhs.row); 
    }
};

struct Cell
{
    static constexpr int NOT_CLAIMED = -1;
    int country = NOT_CLAIMED;
};

class Map : public Grid<Cell>
{
public:
    static constexpr int DIRECTIONS_COUNT = 4;

    Map() : Map(0, 0) {}
    Map(int width, int height, bool is_cylindrical = false) 
        : Grid(width, height),
          is_cylindrical(is_cylindrical) {}

    Coords GetLeftNeighbourCoords(Coords coords) const
    {
        return GetOffsetCoords(coords, -1, 0);
    }

    Coords GetRightNeighbourCoords(Coords coords) const
    {
        return GetOffsetCoords(coords, 1, 0);
    }

    Coords GetUpNeighbourCoords(Coords coords) const
    {
        return GetOffsetCoords(coords, 0, 1);
    }

    Coords GetDownNeighbourCoords(Coords coords) const
    {
        return GetOffsetCoords(coords, 0, -1);
    }

    Coords GetOffsetCoords(Coords coords, int x_offset, int y_offset) const
    {
        Coords res = coords;

        res.column += x_offset;
        res.row    += y_offset;

        int map_width  = static_cast<int>(Width());
        int map_height = static_cast<int>(Height());

        if(!is_cylindrical)
        {
            if(res.column < 0 || res.column >= map_width)
                return coords;

            if(res.row < 0 || res.row >= map_height)
                return coords;

            return res;
        }

        if(x_offset != 0)
            res.column %= map_width;

        if(y_offset != 0)
            res.row %= map_height;

        if(res.row < 0 || res.row >= map_height)
            return coords;

        if(res.column < 0)
            res.column += map_width; 

        return res;
    }
    
    bool FindEmptyNeighbour(Coords location, Coords& out) const
    {
        int direction = rand() % DIRECTIONS_COUNT;
        int tries = 0;

        do
        {
            Coords coords;

            if(direction == 0)      // GO LEFT
            {
                coords = GetLeftNeighbourCoords(location);
            }
            else if(direction == 1) // GO UP
            {
               coords = GetUpNeighbourCoords(location);
            }
            else if(direction == 2) // GO RIGHT
            {
               coords = GetRightNeighbourCoords(location);
            }
            else                    // GO DOWN
            {
               coords = GetDownNeighbourCoords(location);
            }

            if(coords != location && At(coords.row, coords.column).country == Cell::NOT_CLAIMED)
            {
                out = coords;
                return true;
            }

            ++tries;
            direction = (direction + 1) % DIRECTIONS_COUNT;
        }
        while(tries < DIRECTIONS_COUNT);

        return false;
    }
private:
    bool is_cylindrical;
};