#pragma once

#include <cstdint>

#include "console/out.hpp"
#include "objects/layout.hpp"
#include "objects/observable.hpp"

struct TCoords : public ObservableObject
{
    static MemoryLayout const& GetMemoryLayout()
    {
        static MemoryLayout layout
        {
            LayoutRegion(0u, 1u, RegionType::INTEGER, "column"),
            LayoutRegion(1u, 1u, RegionType::INTEGER, "row"),         
        };

        return layout;
    }

    static SIZE_T GetMemorySize()
    {
        MemoryLayout const& layout = TCoords::GetMemoryLayout();

        if(layout.empty()) { return 0; }

        auto const& last_region = layout.back();
        return last_region.offset + last_region.size;
    }

    bool Update(DWORD process_id, uint32_t address)
    {
        return ObservableObject::Update(process_id, address, TCoords::GetMemorySize());
    }

    void PrintDiff() const
    {
        PrintMemoryLayoutDiff(std::cout, GetMemoryLayout(), GetActiveBufferPtr(), GetPrevBufferPtr(), GetAddress());
    }
};