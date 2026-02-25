#pragma once

#include <general.hpp>
#include <console/sfml.hpp>
#include "map.hpp"

#include <time.h>

class MapControl
{
    using AssignedCoords = std::vector<std::vector<Coords>>; 
public:
    Map const& GetMap() { return map; }
    void InitMap(int country_count, int width, int height);
    void Generate(General const& general)
    {
        srand(time(NULL));

        country_count = general.GetGameState().CountryCount();
        int territory_size = general.GetGameState().TerritorySize();
        map = Map(territory_size, territory_size);

        AssignedCoords assigned_coords = ClaimFirstCells(general);
        FloodFill(assigned_coords);
    }

    AssignedCoords ClaimFirstCells(General const& general)
    {
        AssignedCoords assigned_coords(country_count, std::vector<Coords>());
        TRegionArray const& regions = general.GetGameState().GetRegionArray();

        for(int i = 0; i < country_count; ++i)
        {
            TCoords const& coords = regions.GetAssignedCoords(i).At(0);
            assigned_coords.at(i).emplace_back(coords.row, coords.column);
            map.At(coords.row, coords.column).country = i;

            coords.Print(std::cout);
            std::cout << "\n";
        }

        return assigned_coords;
    }

    void FloodFill(AssignedCoords& assigned_coords)
    {
        std::vector<int> coord_id_to_expand(country_count, 0);
        std::vector<bool> out_of_options(country_count, false);
        int countries_done_count = 0;

        while(countries_done_count < country_count)
        {
            for(int i = 0; i < country_count; ++i)
            {
                if(out_of_options[i]) continue;

                bool was_found = false;

                do
                {
                    Coords coords;
                    was_found = map.FindEmptyNeighbour(assigned_coords[i][coord_id_to_expand[i]], coords);

                    if(was_found)
                    {
                        assigned_coords[i].push_back(coords);
                        map.At(coords.row, coords.column).country = i;
                    }
                    else
                    {
                        ++coord_id_to_expand[i];
                    }
                }
                while(!was_found && coord_id_to_expand[i] < assigned_coords[i].size());

                if(!was_found)
                {
                    out_of_options[i] = true;
                    ++countries_done_count;
                }
            }

            //ShowAssignedTable(assigned_coords, general.GetGameState().TerritorySize(), 100ms);
        }

        //ShowAssignedMap(assigned_coords, general.GetGameState().TerritorySize());

        for(int i = 0; i < country_count; ++i)
        {
            std::cout << i << " - " << assigned_coords[i].size() << "\n"; 
        }
    }
private:
    Map map;
    int country_count = 0;
};