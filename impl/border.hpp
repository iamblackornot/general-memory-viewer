#pragma once

#include <list>
#include <unordered_map>

#include "const.hpp"
#include "grid.hpp"

class Border
{
public:
    using CoordsIter = std::list<Coords>::iterator;
    using CoordsConstIter = std::list<Coords>::const_iterator;

    bool Add(Coords coords)
    {
        if(index.contains(coords)) 
        {
            return false;
        }

        CoordsIter iter = list.insert(list.end(), coords);
        index[coords] = iter;

        return true;
    }

    bool Remove(Coords coords)
    {
        if(!index.contains(coords)) 
        {
            return false;
        }

        list.erase(index.at(coords));
        index.erase(coords);

        return true;
    }

    CoordsConstIter begin() const
    {
        return list.begin();
    }

    CoordsConstIter end() const
    {
        return list.end();
    }
private:
    std::list<Coords> list;
    std::unordered_map<Coords, CoordsIter> index;
};

class TransferCandidates
{
public:
    void Add(Coords coords, int foreign_neighbour_count)
    {
        if(index.contains(coords) && index.at(coords).foreign_neighbour_count != foreign_neighbour_count) 
        {
            Remove(coords);
        }

        if(foreign_neighbour_count > 0)
        {
            std::list<Coords>& list = groupped_by_foreign_neighbour[GetGroupIndex(foreign_neighbour_count)];
            CoordsIter iter = list.insert(list.end(), coords);
            index.insert({ coords, IndexEntry(foreign_neighbour_count, iter) });
        }
    }

    bool Remove(Coords coords)
    {
        if(!index.contains(coords)) 
        {
            return false;
        }

        IndexEntry const& entry = index.at(coords);
        groupped_by_foreign_neighbour[GetGroupIndex(entry.foreign_neighbour_count)].erase(entry.iterator);
        index.erase(coords);

        return true;
    }

    bool PopNext(Coords& coords)
    {
        for(int i = 0; i < DIRECTIONS_COUNT; ++i)
        {
            auto& queue = groupped_by_foreign_neighbour[i];

            if(!queue.empty())
            {
                coords = *queue.begin();
                queue.pop_front();

                return true;
            }
        }

        return false;
    }
private:
    using CoordsIter = std::list<Coords>::iterator;
    struct IndexEntry
    {
        int foreign_neighbour_count;
        CoordsIter iterator;

        IndexEntry(int foreign_neighbour_count, CoordsIter iterator)
            : foreign_neighbour_count(foreign_neighbour_count),
              iterator(iterator) {}
    };
    int GetGroupIndex(int foreign_neighbour_count)
    {
        return DIRECTIONS_COUNT - foreign_neighbour_count;
    }
private:
    std::list<Coords> groupped_by_foreign_neighbour[DIRECTIONS_COUNT];
    std::unordered_map<Coords, IndexEntry> index;
};