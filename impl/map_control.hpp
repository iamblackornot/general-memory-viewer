#pragma once

#include <general.hpp>
#include <console/sfml.hpp>

#include "border.hpp"
#include "map.hpp"

#include <time.h>
#include <list>

class MapControl
{
    using AssignedCoords = std::vector<std::vector<Coords>>; 
public:
    Map const& GetMap() const { return map; } 
    Grid<int> const& GetSameNeigbourMap() const { return foreign_neighbour_map; }
    std::vector<Border> const& GetBorders() const { return borders; }

    void InitMap(int country_count, int width, int height);
    void Generate(General const& general)
    {
        srand(time(NULL));

        country_count = general.GetGameState().CountryCount();
        int territory_size = general.GetGameState().TerritorySize();

        map = Map(territory_size, territory_size);
        foreign_neighbour_map = Grid<int>(territory_size, territory_size);
        foreign_neighbour_map.Fill(0);

        borders.clear();
        borders.resize(country_count);
        country_sizes.clear();
        country_sizes.resize(country_count, 0);

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

            ClaimCell(coords.row, coords.column, i);

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
        }

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
            country_sizes[new_country] += 1;

            if(prev_country != Cell::NOT_CLAIMED)
            {
                country_sizes[prev_country] -= 1;
            }
            
            UpdateNeighbours(coords, new_country, prev_country);
        }
    }

    void UpdateNeighbours(Coords coords, int new_country, int prev_country)
    {      
        int foreign_neighbour_count = 0;

        for(const Coords neighbour_coords : map.GetAllNeighbourCoords(coords))
        {
            int neighbour_country = map.At(neighbour_coords).country;

            if(neighbour_country == Cell::NOT_CLAIMED)
            {
                continue;
            }

            if(neighbour_country == new_country)
            {
                if(prev_country != Cell::NOT_CLAIMED)
                {
                    if(foreign_neighbour_map.At(neighbour_coords) == 1)
                    {
                        borders.at(neighbour_country).Remove(neighbour_coords);
                    }

                    foreign_neighbour_map.At(neighbour_coords) = (std::max)(0, foreign_neighbour_map.At(neighbour_coords) - 1);
                }
            }
            else 
            {
                ++foreign_neighbour_count;
                foreign_neighbour_map.At(neighbour_coords) += 1;

                if(foreign_neighbour_map.At(neighbour_coords) == 1)
                {
                    borders.at(neighbour_country).Add(neighbour_coords);
                }              
            }
        }

        foreign_neighbour_map.At(coords) = foreign_neighbour_count;

        if(prev_country != Cell::NOT_CLAIMED)
        {
            borders.at(prev_country).Remove(coords);
        }

        if(foreign_neighbour_count > 0)
        {
            borders.at(new_country).Add(coords);
        }
    }

    int TransferCells(int country_from, int country_to, int count)
    {
        if(country_from == country_to)
        {
            return 0;
        }
        
        int transfered = 0;
        TransferCandidates candidates;

        for(auto const coords : borders.at(country_from))
        {
            if(map.HasNeighbour(coords, country_to))
            {
                candidates.Add(coords, foreign_neighbour_map.At(coords));
            }
        }

        Coords coords;
        while(transfered < count && candidates.PopNext(coords))
        {
            ClaimCell(coords, country_to);
            transfered += 1;

            for(Coords const neighbour_coords : map.GetAllNeighbourCoords(coords))
            {
                if(map.At(neighbour_coords).country == country_from)
                {
                    candidates.Add(neighbour_coords, foreign_neighbour_map.At(neighbour_coords));
                }
            }
        }
        
        return transfered;
    }

    int TransferCellsDebug(int country_from, int country_to, int count, std::function<void()> callback = [](){})
    {
        if(country_from == country_to)
        {
            return 0;
        }
        
        int transfered = 0;
        TransferCandidates candidates;

        for(auto const coords : borders.at(country_from))
        {
            if(map.HasNeighbour(coords, country_to))
            {
                candidates.Add(coords, foreign_neighbour_map.At(coords));
            }
        }

        Coords coords;
        while(transfered < count && candidates.PopNext(coords))
        {
            callback();

            ClaimCell(coords, country_to);
            transfered += 1;

            for(Coords const neighbour_coords : map.GetAllNeighbourCoords(coords))
            {
                if(map.At(neighbour_coords).country == country_from)
                {
                    candidates.Add(neighbour_coords, foreign_neighbour_map.At(neighbour_coords));
                }
            }
        }
        
        return transfered;
    }

private:
    Map map;
    Grid<int> foreign_neighbour_map;
    std::vector<Border> borders;
    std::vector<int> country_sizes;
    int country_count = 0;
};