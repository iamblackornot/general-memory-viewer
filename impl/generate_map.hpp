#pragma once

#include <general.hpp>
#include <console/sfml.hpp>

#include <time.h>

namespace
{
    #define NEIGHBOUR_COUNT 4

    // inline TCoords GetOffsetCoords(TCoords const& location, uint32_t map_size, int x_offset, int y_offset)
    // {
    //     TCoords res;

    //     if(x_offset < 0)
    //     {
    //         res.column = static_cast<uint8_t>(
    //             location.column < - x_offset 
    //                 ? map_size + x_offset 
    //                 : location.column + x_offset
    //         );
    //     }
    //     else
    //     {
    //         res.column = static_cast<uint8_t>((location.column + x_offset) % map_size);
    //     }

    //     if(y_offset < 0)
    //     {
    //         res.row = static_cast<uint8_t>(
    //             location.row < - y_offset 
    //                 ? map_size + y_offset 
    //                 : location.row + y_offset
    //         );
    //     }
    //     else
    //     {
    //         res.row = static_cast<uint8_t>((location.row + y_offset) % map_size);
    //     }

    //     return res;
    // }

    inline TCoords GetOffsetCoords(TCoords const& location, uint32_t map_size, int x_offset, int y_offset)
    {
        TCoords res;

        if(x_offset < 0)
        {
            res.column = static_cast<uint8_t>(
                location.column < - x_offset 
                    ? location.column 
                    : location.column + x_offset
            );
        }
        else
        {
            res.column = static_cast<uint8_t>(
                map_size - location.column - 1 < x_offset 
                    ? location.column 
                    : location.column + x_offset
            );
        }

        if(y_offset < 0)
        {
            res.row = static_cast<uint8_t>(
                location.row < - y_offset 
                    ? location.row 
                    : location.row + y_offset
            );
        }
        else
        {
            res.row = static_cast<uint8_t>(
                map_size - location.row - 1 < y_offset 
                    ? location.row 
                    : location.row + y_offset
            );
        }

        return res;
    }

    inline bool FindEmptyNeighbour(Map const& map, TCoords const& location, TCoords& out)
    {
        int direction = rand() % NEIGHBOUR_COUNT;
        int tries = 0;

        do
        {
            TCoords coord;

            if(direction == 0)      // GO LEFT
            {
                coord = GetOffsetCoords(location, map.size(), -1, 0);
            }
            else if(direction == 1) // GO UP
            {
               coord = GetOffsetCoords(location, map.size(), 0, 1); 
            }
            else if(direction == 2) // GO RIGHT
            {
               coord = GetOffsetCoords(location, map.size(), 1, 0); 
            }
            else                    // GO DOWN
            {
               coord = GetOffsetCoords(location, map.size(), 0, -1); 
            }

            if(map.at(coord.row).at(coord.column).country_id == 0)
            {
                out = coord;
                return true;
            }

            ++tries;
            direction = (direction + 1) % NEIGHBOUR_COUNT;
        }
        while(tries < NEIGHBOUR_COUNT);

        return false;
    }
}

inline void GenerateMap(General const& general)
{
    uint32_t country_count = general.GetGameState().CountryCount();
    uint32_t territory_size = general.GetGameState().TerritorySize();
    TRegionArray const& regions = general.GetGameState().GetRegionArray();

    AssignedCoords assigned_coords(country_count, std::vector<TCoords>());
    std::vector<uint32_t> coord_id_to_expand(country_count, 0);
    std::vector<bool> out_of_options(country_count, false);
    uint32_t countries_done_count = 0;
    Map map(territory_size, std::vector(territory_size, TSquare{0}));
 
    for(uint32_t i = 0; i < country_count; ++i)
    {
        TCoords const& coord = regions.GetAssignedCoords(i).At(0);
        assigned_coords.at(i).push_back(coord);
        map.at(coord.row).at(coord.column).country_id = static_cast<uint8_t>(i + 1u);

        coord.Print(std::cout);
        std::cout << "\n";
    }

    ShowAssignedMap(assigned_coords, general.GetGameState().TerritorySize());
    srand(time(NULL));

    while(countries_done_count < country_count)
    {
        for(uint32_t country_id = 0; country_id < country_count; ++country_id)
        {
            if(out_of_options[country_id]) continue;

            bool was_found = false;

            do
            {
                TCoords coords;
                was_found = FindEmptyNeighbour(map, assigned_coords[country_id][coord_id_to_expand[country_id]], coords);

                if(was_found)
                {
                    assigned_coords[country_id].push_back(coords);
                    map[coords.row][coords.column].country_id = static_cast<uint8_t>(country_id + 1);
                }
                else
                {
                    ++coord_id_to_expand[country_id];
                }
            }
            while(!was_found && coord_id_to_expand[country_id] < assigned_coords[country_id].size());

            if(!was_found)
            {
                out_of_options[country_id] = true;
                ++countries_done_count;
            }
        }

        ShowAssignedMap(assigned_coords, general.GetGameState().TerritorySize(), 100ms);
    }

    ShowAssignedMap(assigned_coords, general.GetGameState().TerritorySize());

    for(uint32_t country_id = 0; country_id < country_count; ++country_id)
    {
        std::cout << country_id << " - " << assigned_coords[country_id].size() << "\n"; 
    }
}