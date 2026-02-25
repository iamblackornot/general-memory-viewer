#pragma once

#include <general.hpp>
#include <console/sfml.hpp>
#include "map.hpp"

#include <time.h>

class MapControl
{
    using AssignedCoords = std::vector<std::vector<Coords>>; 
public:
    Map const& GetMap() const { return map; } 
    Grid<int> const& GetSameNeigbourMap() const { return same_neighbour_map; }

    void InitMap(int country_count, int width, int height);
    void Generate(General const& general)
    {
        srand(time(NULL));

        country_count = general.GetGameState().CountryCount();
        int territory_size = general.GetGameState().TerritorySize();

        map = Map(territory_size, territory_size);
        same_neighbour_map = Grid<int>(territory_size, territory_size);
        same_neighbour_map.Fill(0);

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
                        ClaimCell(coords.row, coords.column, i);
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

    void ClaimCell(int row, int column, int new_country)
    {
        ClaimCell(Coords(row, column), new_country);
    }

    void ClaimCell(Coords coords, int new_country)
    {
        int prev_country = map.At(coords).country;

        if(new_country != prev_country)
        {
            map.At(coords).country = new_country;
            UpdateSameNeighbourMap(coords, new_country, prev_country);
        }
    }

    void UpdateSameNeighbourMap(Coords coords, int new_country, int prev_country)
    {
        std::vector<Coords> coords_to_update;

        if(Coords neighbour_coords = map.GetUpNeighbourCoords(coords); neighbour_coords != coords) 
        { 
            coords_to_update.push_back(neighbour_coords); 
        }

        if(Coords neighbour_coords = map.GetRightNeighbourCoords(coords); neighbour_coords != coords) 
        { 
            coords_to_update.push_back(neighbour_coords); 
        }

        if(Coords neighbour_coords = map.GetDownNeighbourCoords(coords); neighbour_coords != coords) 
        { 
            coords_to_update.push_back(neighbour_coords); 
        }

        if(Coords neighbour_coords = map.GetLeftNeighbourCoords(coords); neighbour_coords != coords) 
        { 
            coords_to_update.push_back(neighbour_coords); 
        }
        
        int same_neighbour_count = 0;

        for(const Coords coords : coords_to_update)
        {
            int neighbour_country = map.At(coords).country;

            if(neighbour_country == new_country)
            {
                ++same_neighbour_map.At(coords);
                ++same_neighbour_count;
            }
            else if(neighbour_country == prev_country)
            {
                same_neighbour_map.At(coords) = (std::max)(0, same_neighbour_map.At(coords) - 1);
            }
        }

        same_neighbour_map.At(coords) = same_neighbour_count;
    }

private:
    Map map;
    Grid<int> same_neighbour_map;
    int country_count = 0;
};