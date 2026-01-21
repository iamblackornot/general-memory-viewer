#pragma once

#include <cstdint>

#include "console/out.hpp"
#include "objects/layout.hpp"
#include "objects/observable.hpp"

#include "objects/types/t_squares_array.hpp"
#include "objects/types/t_region.hpp"

struct TGameState : public ObservableObject
{
    static MemoryLayout const& GetMemoryLayout()
    {
        static MemoryLayout layout
        {
            LayoutRegion(0u, 4u, RegionType::POINTER, "vtable_ptr"),
            
            LayoutRegion(4u, 1u, RegionType::INTEGER, "width"),
            LayoutRegion(5u, 1u, RegionType::INTEGER, "height"),
            LayoutRegion(6u, 1u, RegionType::INTEGER, "is_cylinder_world"),
            LayoutRegion(7u, 1u, RegionType::INTEGER, "unknown_1"),
            
            LayoutRegion(8u, 4u, RegionType::POINTER, "squares_ptr"),
            
            LayoutRegion(12u, 1u, RegionType::INTEGER, "country_count"),
            LayoutRegion(13u, 1u, RegionType::INTEGER, "unknown_2"),
            LayoutRegion(14u, 1u, RegionType::INTEGER, "unknown_3"),
            LayoutRegion(15u, 1u, RegionType::INTEGER, "unknown_4"),
            
            LayoutRegion(16u, 4u, RegionType::POINTER, "region_array_ptr"),
            
            LayoutRegion(20u, 2u, RegionType::INTEGER, "grid_size"),
            LayoutRegion(22u, 2u, RegionType::INTEGER, "assigned cell count"),
            
            LayoutRegion(24u, 2u, RegionType::INTEGER, "squares_per_country"),
            LayoutRegion(26u, 2u, RegionType::INTEGER, "squares_per_country_adj"),
            
            LayoutRegion(28u, 4u, RegionType::POINTER, "word_array_ptr"),
            LayoutRegion(32u, 4u, RegionType::INTEGER, "unknown_5"),
            
            LayoutRegion(36u, 1u, RegionType::INTEGER, "smth_2"),
            LayoutRegion(37u, 1u, RegionType::INTEGER, "unknown_6"),
            LayoutRegion(38u, 1u, RegionType::INTEGER, "unknown_7"),
            LayoutRegion(39u, 1u, RegionType::INTEGER, "unknown_8"),
            
            LayoutRegion(40u, 1u, RegionType::INTEGER, "smth_3"),
            LayoutRegion(41u, 1u, RegionType::INTEGER, "unknown_9"),
            LayoutRegion(42u, 1u, RegionType::INTEGER, "unknown_10"),
            LayoutRegion(43u, 1u, RegionType::INTEGER, "unknown_11"),
            
            LayoutRegion(44u, 4u, RegionType::INTEGER, "smth_4"),
            
            LayoutRegion(48u, 1u, RegionType::INTEGER, "hardcoded_1"),
            LayoutRegion(49u, 1u, RegionType::INTEGER, "unknown_12"),
            LayoutRegion(50u, 1u, RegionType::INTEGER, "unknown_13"),
            LayoutRegion(51u, 1u, RegionType::INTEGER, "unknown_14"),
            
            LayoutRegion(52u, 1u, RegionType::INTEGER, "max_attemps_to_place_first_mark"),
            LayoutRegion(53u, 1u, RegionType::INTEGER, "unknown_15"),
            LayoutRegion(54u, 1u, RegionType::INTEGER, "unknown_16"),
            LayoutRegion(55u, 1u, RegionType::INTEGER, "unknown_17"),
            
            LayoutRegion(56u, 1u, RegionType::INTEGER, "hardcoded_2"),
            LayoutRegion(57u, 1u, RegionType::INTEGER, "unknown_18"),
            LayoutRegion(58u, 1u, RegionType::INTEGER, "unknown_19"),
            LayoutRegion(59u, 1u, RegionType::INTEGER, "unknown_20"),
            
            LayoutRegion(60u, 1u, RegionType::INTEGER, "hardcoded_3"),
            LayoutRegion(61u, 1u, RegionType::INTEGER, "unknown_21"),
            LayoutRegion(62u, 1u, RegionType::INTEGER, "unknown_22"),
            LayoutRegion(63u, 1u, RegionType::INTEGER, "unknown_23"),
            
            LayoutRegion(64u, 4u, RegionType::POINTER, "boolen_array_ptr"),
            LayoutRegion(68u, 4u, RegionType::POINTER, "word_array_2_ptr")
        };

        return layout;
    }

    static SIZE_T GetMemorySize()
    {
        MemoryLayout const& layout = TGameState::GetMemoryLayout();

        if(layout.empty()) { return 0; }

        auto const& last_region = layout.back();
        return last_region.offset + last_region.size;
    }

    uint32_t TerritorySize() const
    {
        return GetValue<uint8_t>(4u);
    }

    uint32_t CountryCount() const
    {
        return GetValue<uint8_t>(12u);
    }


    bool Update(DWORD process_id, uint32_t address)
    {
        if(!ObservableObject::Update(process_id, address, TGameState::GetMemorySize())) { return false; }

        if(!region_array.Update(process_id, GetValue<uint32_t>(16u), CountryCount())) { return false; }

        return squares_array.Update(process_id, GetValue<uint32_t>(8u), TerritorySize());
    }

    void PrintGameState() const
    {
        PrintMemoryLayout(std::cout, GetMemoryLayout(), GetActiveBufferPtr(), GetAddress());
    }    
    
    void PrintGameStateDiff()
    {
        PrintMemoryLayoutDiff(std::cout, GetMemoryLayout(), GetActiveBufferPtr(), GetPrevBufferPtr(), GetAddress());
    }

    TSquaresArray const& GetSquaresArray() const
    {
        return squares_array;
    }

    TRegionArray const& GetRegionArray() const
    {
        return region_array;
    }

private:
    TSquaresArray squares_array;
    TRegionArray region_array;
};