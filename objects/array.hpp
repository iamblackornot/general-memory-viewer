#pragma once

#include "objects/observable.hpp"

template<typename T>
struct ObservableArray : public ObservableObject
{
    uint32_t GetMemorySize()
    {
        return size * (sizeof(T));
    }

    uint32_t Size() const
    {
        return size;
    }

    bool Update(DWORD process_id, uint32_t address, uint32_t new_size)
    {
        size = new_size;

        return ObservableObject::Update(process_id, address, GetMemorySize());
    }

    T const& At (uint32_t index) const
    {
        return GetValue<T>(index * sizeof(T));
    }

    T const& AtPrev (uint32_t index) const
    {
        return GetValuePrev<T>(index * sizeof(T));
    }
private:
    uint32_t size = 0;
};
