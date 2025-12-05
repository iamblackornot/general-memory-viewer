#pragma once

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>

#include "style.hpp"
#include "objects/layout.hpp"
#include "objects/array.hpp"

#define PRINT_VALUE_WITDH 10

// inline void PrintInt(std::ostream& output, uint8_t const* ptr, size_t size) 
// {
//     output << " " << COLOR_BRIGHT_GREEN << std::setw(PRINT_VALUE_WITDH);

//     if      (size == 1) output << +(*ptr);
//     else if (size == 2) output << *reinterpret_cast<uint16_t const*>(ptr);
//     else if (size == 4) output << *reinterpret_cast<uint32_t const*>(ptr);
//     else if (size == 8) output << *reinterpret_cast<uint64_t const*>(ptr);
//     else                output << "wrong int size";

//     output << " " << COLOR_RESET;
// }
template<typename TScalarType>
inline void PrintInt(std::ostream& output, TScalarType scalar_value) 
{
    output << " " << COLOR_BRIGHT_GREEN << std::setw(PRINT_VALUE_WITDH) << scalar_value << " " << COLOR_RESET;    
}

inline void PrintInt(std::ostream& output, uint8_t const* ptr, size_t size) 
{
    if      (size == 1) PrintInt(output, +(*ptr));
    else if (size == 2) PrintInt(output, *reinterpret_cast<uint16_t const*>(ptr));
    else if (size == 4) PrintInt(output, *reinterpret_cast<uint32_t const*>(ptr));
    else if (size == 8) PrintInt(output, *reinterpret_cast<uint64_t const*>(ptr));
    else                output << "wrong int size";
}

inline void PrintPtr32(std::ostream& output, uint32_t const ptr) 
{
    output 
        << std::uppercase << COLOR_BRIGHT_GREEN << " 0x" << std::hex << std::setw(PRINT_VALUE_WITDH - 2) << std::setfill('0') 
        << ptr << " " << std::dec << std::setfill(' ') << COLOR_RESET;
}

inline void PrintPtr32(std::ostream& output, uint8_t const* ptr) 
{
    PrintPtr32(output, *reinterpret_cast<uint32_t const*>(ptr));
}

inline void PrintValue(std::ostream& output, LayoutRegion const& region, uint8_t const* base_ptr)
{
    if(region.type == RegionType::INTEGER)
        PrintInt(output, base_ptr + region.offset, region.size);
    else if(region.type == RegionType::POINTER)
        PrintPtr32(output, base_ptr + region.offset);
}

template<typename TValueType>
inline void PrintValue(std::ostream& output, RegionType value_type, TValueType value)
{
    if(value_type == RegionType::INTEGER)
        PrintInt(output, value);
    else if(value_type == RegionType::POINTER)
        PrintPtr32(output, value);
}

inline void PrintMemoryAddressHeader(std::ostream& output, uint32_t const address) 
{
    output << "    ----------------\n";
    output << "    | " << COLOR_YELLOW << std::setw(3);
    
    PrintPtr32(output, address);
    
    output << COLOR_RESET << " |" << "\n";
    output << "    ----------------\n";
}

inline void PrintLayoutRegion(std::ostream& output, LayoutRegion const& region, uint8_t const* base_ptr) 
{
    output << COLOR_BLUE << std::setw(3) << region.offset << COLOR_RESET << " | ";

    PrintValue(output, region, base_ptr);

    output << COLOR_RESET << " | " << region.name << "\n";
}

inline void PrintMemoryLayout(std::ostream& output, MemoryLayout const& layout, uint8_t const* base_ptr, uint32_t address) 
{
    PrintMemoryAddressHeader(output, address);

    size_t total_offset = 0;

    for(LayoutRegion const& region : layout)
    {
        PrintLayoutRegion(output, region, base_ptr);
        total_offset += region.size;

        if((total_offset & 0b11) == 0) { output << "\n"; }
    }
}

inline void PrintLayoutRegionDiff(std::ostream& output, LayoutRegion const& region, uint8_t const* updated_ptr, uint8_t const* prev_ptr) 
{
    output << COLOR_BLUE << std::setw(3) << region.offset << COLOR_RESET << " | ";

    bool changed = std::memcmp(updated_ptr + region.offset, prev_ptr + region.offset, region.size);

    if(changed) { output << BG_BLUE; }

    PrintValue(output, region, updated_ptr);

    output << COLOR_RESET << " | ";

    PrintValue(output, region, prev_ptr);

    output << COLOR_RESET << " | " << region.name << "\n";
}

inline void PrintMemoryLayoutDiff(
    std::ostream& output, 
    MemoryLayout const& layout, 
    uint8_t const* updated_ptr, 
    uint8_t const* prev_ptr,
    uint32_t address) 
{
    PrintMemoryAddressHeader(output, address);

    size_t total_offset = 0;

    for(LayoutRegion const& region : layout)
    {
        PrintLayoutRegionDiff(output, region, updated_ptr, prev_ptr);
        total_offset += region.size;

        if((total_offset & 0b11) == 0) 
        { 
            output << "\n";
        }
    }
}

template<typename T>
inline void PrintArrayDiff(std::ostream& output, RegionType value_type, ObservableArray<T> const& array) 
{
    PrintMemoryAddressHeader(output, array.GetAddress());

    for(size_t i = 0; i < array.Size(); ++i)
    {
        output << COLOR_BLUE << std::setw(3) << i << COLOR_RESET << " | ";

        T value = array.At(i);
        T prev_value = array.AtPrev(i);

        if(value != prev_value) { output << BG_BLUE; }

        PrintValue(output, value_type, value);

        output << COLOR_RESET << " | ";

        PrintValue(output, value_type, prev_value);

        output << COLOR_RESET << "\n";
    }
}
