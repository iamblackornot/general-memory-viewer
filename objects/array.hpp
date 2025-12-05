#pragma once

#include "objects/observable.hpp"

template<typename T>
struct ObservableArray : public ObservableObject
{
    SIZE_T GetMemorySize()
    {
        return size * (sizeof(T));
    }

    size_t Size() const
    {
        return size;
    }

    bool Update(DWORD process_id, uint32_t address, uint32_t new_size)
    {
        size = new_size;

        return ObservableObject::Update(process_id, address, GetMemorySize());
    }

    T const& At (size_t index) const
    {
        return GetValue<T>(index * sizeof(T));
    }

    T const& AtPrev (size_t index) const
    {
        return GetValuePrev<T>(index * sizeof(T));
    }
private:
    size_t size = 0;
};
