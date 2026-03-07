#pragma once

#include <cstdint>
#include <format>
#include <fstream>

#include <general.hpp>
#include <console/sfml.hpp>
#include <impl/map_control.hpp>

void PrintMenu()
{
    std::cout << "1. Print TGameState" << std::endl;
    std::cout << "2. Draw Game Map" << std::endl;
    std::cout << "3. Print Game Map" << std::endl;
    std::cout << "4. Print TSQuaresArray column ptrs" << std::endl;
    std::cout << "5. Print Neighbour Count Map" << std::endl;
    std::cout << "6. Print Region" << std::endl;
    std::cout << "7. Print to File Region Assigned Coords" << std::endl;
    std::cout << "8. Print Region Sizes" << std::endl;
    std::cout << "9. Print Neighbour Stats" << std::endl;
    std::cout << "10. Print Square Data" << std::endl;
    std::cout << "11. Generate Map" << std::endl;
    std::cout << "12. Print SameNeighbourMap" << std::endl;
    std::cout << "13. Print BorderMap" << std::endl;
    std::cout << "14. Transfer Cells (from, to, count)" << std::endl;
    std::cout << "15. Claim Cell (row, col, country)" << std::endl;
    std::cout << "16. Transfer Cells (from, to, count) Visualize Iterations" << std::endl;
}

int MenuLoop()
{
    std::string input;
    General general;
    MapControl control;

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
                general.GetGameState().GetSquaresArray().PrintMapDiff();
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
                general.GetGameState().PrintRegionSizeTable();
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
            else if(input == "11")
            {
                control.Generate(general);
                ShowMap(control.GetMap());
            }
            else if(input == "12")
            {
                PrintSameNeighbourMap(control.GetSameNeigbourMap(), control.GetMap());
            }
            else if(input == "13")
            {
                ShowBorderMap(control);
            }
            else if(input == "14")
            {
                int from;
                int to;
                int count;

                std::cin >> from;
                std::cin >> to;
                std::cin >> count;

                int transfered = control.TransferCells(from, to, count);
                std::cout << "\ntransfered " << transfered << std::endl;
            }
            else if(input == "15")
            {
                int row;
                int col;
                int country;

                std::cin >> row;
                std::cin >> col;
                std::cin >> country;

                control.ClaimCell(row, col, country); 
            }
            else if(input == "16")
            {
                int from;
                int to;
                int count;

                std::cin >> from;
                std::cin >> to;
                std::cin >> count;

                int transfered = control.TransferCellsDebug(from, to, count, [&](){ ShowBorderMap(control); });
                std::cout << "\ntransfered " << transfered << std::endl;
            }
        }
    }

    return EXIT_SUCCESS;
}