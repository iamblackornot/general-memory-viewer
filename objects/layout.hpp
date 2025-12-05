#pragma once

#include <iostream>
#include <string>
#include <vector>

enum class RegionType : int
{
    INTEGER = 0,
    POINTER = 1,
};

class LayoutRegion
{
public:
    uint32_t offset;
    uint32_t size;
    RegionType type;
    std::string name;

    LayoutRegion(uint32_t offset, uint32_t size, RegionType type, std::string name):
        offset(offset), size(size), type(type), name(name) {}

private:

};

typedef std::vector<LayoutRegion> MemoryLayout;
