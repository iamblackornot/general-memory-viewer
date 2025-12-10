#pragma once

#include <cstdint>

#include "console/out.hpp"
#include "objects/layout.hpp"
#include "objects/observable.hpp"
#include "objects/types/t_coords.hpp"

class TRegion
{
public:
    static MemoryLayout const& GetMemoryLayout()
    {
        static MemoryLayout layout
        {
            LayoutRegion(0u, 1u, RegionType::INTEGER, "assigned_cell_count"),
            LayoutRegion(1u, 1u, RegionType::INTEGER, "unknown_1"),
            LayoutRegion(2u, 1u, RegionType::INTEGER, "unknown_2"),
            LayoutRegion(3u, 1u, RegionType::INTEGER, "unknown_3"),

            LayoutRegion(4u, 1u, RegionType::INTEGER, "unknown_4"),
            LayoutRegion(5u, 1u, RegionType::INTEGER, "unknown_5"),
            LayoutRegion(6u, 1u, RegionType::INTEGER, "unknown_6"),
            LayoutRegion(7u, 1u, RegionType::INTEGER, "unknown_7"),
            
            LayoutRegion(8u, 4u, RegionType::POINTER, "assigned_coords_ptr"),            
            LayoutRegion(12u, 4u, RegionType::POINTER, "some_ptr"),            
        };

        return layout;
    }

    static uint32_t GetMemorySize()
    {
        MemoryLayout const& layout = TRegion::GetMemoryLayout();

        if(layout.empty()) { return 0; }

        auto const& last_region = layout.back();
        return last_region.offset + last_region.size;
    }

public:
    uint8_t assigned_cell_count;
    uint8_t unknown[7];
    uint32_t assigned_coords_ptr;
    uint32_t some_ptr;
};

class TRegionArray: public ObservableArray<TRegion>
{
public:
    bool Update(DWORD process_id, uint32_t address, uint32_t new_size)
    {
        if(size != new_size)
        {
            size = new_size;
            assigned_coords.resize(new_size);
        }

        bool res = ObservableArray::Update(process_id, address, new_size);

        for(uint32_t i = 0; i < size && res; ++i)
        {
            res &= assigned_coords[i].Update(process_id, At(i).assigned_coords_ptr, At(i).assigned_cell_count);
        }

        return res;
    }

    void PrintRegionDiff(uint32_t country_id) const
    {
        TRegion const& active = At(country_id);
        TRegion const& prev = AtPrev(country_id);

        PrintMemoryLayoutDiff(
            std::cout, 
            TRegion::GetMemoryLayout(), 
            reinterpret_cast<uint8_t const*>(&active),
            reinterpret_cast<uint8_t const*>(&prev), 
            GetAddress() + TRegion::GetMemorySize() * country_id
        );
    }


    TCoordsArray const& GetAssignedCoords(uint32_t country_id) const
    {
        return assigned_coords[country_id];
    }
    
private:
    uint32_t size = 0;
    std::vector<TCoordsArray> assigned_coords;
};