#pragma once

#include "layout.hpp"
#include "inject.hpp"
#include "console/out.hpp"

class ObservableObject
{
public:
    uint32_t GetAddress() const
    {
        return address;
    }

protected:
    ObservableObject() = default;

    template<typename T>
    T const& GetValue(size_t offset) const
    {
        return *(reinterpret_cast<T const*>(GetActiveBufferPtr() + offset));
    }

    template<typename T>
    T const& GetValuePrev(size_t offset) const
    {
        return *(reinterpret_cast<T const*>(GetPrevBufferPtr() + offset));
    }

    bool Update(DWORD process_id, uint32_t address, SIZE_T new_size)
    {
        this->address = address;

        if(new_size != size) {
            size = new_size;
            buffer[0].resize(size, 0);
            buffer[1].resize(size, 0);
        }

        last_updated_buf ^= 1;
        return ReadProcessMemoryEx(process_id, reinterpret_cast<LPVOID>(address), GetMutableBufferPtr(), size);
    }

    uint8_t const* GetActiveBufferPtr() const
    {
        return buffer[last_updated_buf].data();
    }

    uint8_t const* GetPrevBufferPtr() const
    {
        return buffer[last_updated_buf ^ 1].data();
    }

private:
    uint8_t* GetMutableBufferPtr()
    {
        return buffer[last_updated_buf].data();
    }

private:
    std::vector<uint8_t> buffer[2];
    uint8_t last_updated_buf = 0;
    uint32_t address;
    size_t size = 0;
};
