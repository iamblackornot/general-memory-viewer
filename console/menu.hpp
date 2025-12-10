#pragma once

#include <cstdint>
#include <general.hpp>
#include <console/sfml.hpp>

void PrintMenu()
{
    std::cout << "1. Print TGameState" << std::endl;
    std::cout << "2. Draw Game Map" << std::endl;
    std::cout << "3. Print Game Map" << std::endl;
    std::cout << "4. Print TSQuaresArray column ptrs" << std::endl;
    std::cout << "5. Print Neighbour Count Map" << std::endl;
    std::cout << "6. Print Region" << std::endl;
    std::cout << "7. Print Region Last Assigned Coords" << std::endl;
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
                general.GetGameState().GetRegionArray().GetAssignedCoords(id - 1).PrintDiff();
            }
                
        }
    }

    return EXIT_SUCCESS;
}