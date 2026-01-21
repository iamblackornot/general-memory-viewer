#pragma once

#include <cstdint>
#include <format>
#include <fstream>

#include <general.hpp>
#include <console/sfml.hpp>
#include <impl/generate_map.hpp>

void PrintMenu()
{
    std::cout << "1. Print TGameState" << std::endl;
    std::cout << "2. Draw Game Map" << std::endl;
    std::cout << "3. Print Game Map" << std::endl;
    std::cout << "4. Print TSQuaresArray column ptrs" << std::endl;
    std::cout << "5. Print Neighbour Count Map" << std::endl;
    std::cout << "6. Print Region" << std::endl;
    std::cout << "7. Print to File Region Assigned Coords" << std::endl;
    std::cout << "8. Generate Map" << std::endl;
    std::cout << "9. Print Neighbour Stats" << std::endl;
    std::cout << "10. Print Square Data" << std::endl;
}

int MenuLoop()
{
    std::string input;
    General general;

    while(input != "q")
    {
        PrintMenu();

        std::cin >> input;

        if(input == "q") continue;

        if(general.Update())
        {
            if(input == "1")
                general.GetGameState().PrintGameState();
            else if(input == "2")
                ShowMap(general);
            else if(input == "3")
                general.GetGameState().GetSquaresArray().PrintMap();
            else if(input == "4")
                general.GetGameState().GetSquaresArray().PrintColumns();
            else if(input == "5")
                general.GetGameState().GetSquaresArray().PrintNeighbourCountMap();
            else if(input == "6")
            {
                std::cin >> input;
                uint32_t id = std::stoul(input);
                general.GetGameState().GetRegionArray().PrintRegionDiff(id - 1);
            }
            else if(input == "7")
            {
                std::cin >> input;
                uint32_t id = std::stoul(input);
                std::string filename = std::format("coords_{}", id);
                std::ofstream output(filename);

                general.GetGameState().GetRegionArray().GetAssignedCoords(id - 1).Print(output);

                std::cout << "printed coords data to [" << filename << "]\n";
            }
            else if(input == "8")
            {
                GenerateMap(general);
            }
            else if(input == "9")
            {
                uint32_t count = general.GetGameState().CountryCount();

                for(uint32_t i = 0; i < count; ++i)
                {
                    general.GetGameState().GetRegionArray().GetNeighbourStats(i).PrintDiff();
                }
            }
            else if(input == "10")
            {
                std::cin >> input;
                uint32_t col = std::stoul(input);

                std::cin >> input;
                uint32_t row = std::stoul(input);

                general.GetGameState().GetSquaresArray().PrintSquareData(row, col);
            }
                
        }
    }

    return EXIT_SUCCESS;
}