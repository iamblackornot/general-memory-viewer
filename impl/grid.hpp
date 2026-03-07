#pragma once

#include <vector>

struct Coords
{
    int row;
    int column;

    Coords() = default;
    Coords( int row, int column) : row(row), column(column) {}

    bool operator==(Coords const& rhs) const
    {
        return std::tie(this->column, this->row) == std::tie(rhs.column, rhs.row); 
    }
};

template<>
struct std::hash<Coords>
{
    size_t operator()(Coords const& coords) const noexcept
    {
        return static_cast<size_t>(coords.row)
             | static_cast<size_t>(coords.column) << sizeof(int); 
    }
};

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

    T& At(Coords coords)
    {
        return At(coords.row, coords.column);
    }

    T const& At(Coords coords) const
    {
        return At(coords.row, coords.column);
    }

    T& At(int row, int column)
    { 
        return data.at(row * width + column); 
    }

    T const& At(int row, int column) const
    { 
        return data.at(row * width + column); 
    }

    void Fill(T const& value) 
    { 
        std::fill(data.begin(), data.end(), value);
    }
private:
    std::vector<T> data;
    int width;
};