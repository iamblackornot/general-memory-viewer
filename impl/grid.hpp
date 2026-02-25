#pragma once

#include <vector>

template <typename T>
class Grid 
{
public:
    Grid() : Grid(0, 0) {}
    Grid(int width, int height)
        : width(width), data(width * height) {}

    int Width() const { return static_cast<int>(width); } const
    int Height() const 
    {
        if(width == 0)
            return 0;

        return static_cast<int>(data.size() / width); 
    }

    T& At(int row, int column)
    { 
        return data.at(row * width + column); 
    }

    T const& At(int row, int column) const
    { 
        return data.at(row * width + column); 
    }
private:
    std::vector<T> data;
    int width;
};