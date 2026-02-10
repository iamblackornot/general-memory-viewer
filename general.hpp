#pragma once

#include <memory>
#include <string_view>

#include "inject.hpp"
#include "objects/types/t_game_state.hpp"

class General
{
public:
    enum class GameVersion
    {
        V4_5h,
        V4_6,
        V4_7,
        Unknown,
    };
public:
    static constexpr std::wstring_view PROCESS_NAME = L"general.exe";

    General() = default;

    bool ReattachToProccess()
    {
        process_id = GetProcessIdByName(PROCESS_NAME.data());
    
        if (process_id == 0) 
        {
            std::cout << "process not found." << std::endl;
            return false;
        }

        GameVersion version = GetGameVersion();

        if(version == GameVersion::Unknown)
        {
            std::cout << "failed to read game version" << std::endl;
            return false;
        }

        uint32_t game_state_ptr_location = GetGameStatePointerLocation(version);
        
        if (!ReadProcessMemoryEx(process_id, reinterpret_cast<LPVOID>(game_state_ptr_location), &game_state_ptr, sizeof(game_state_ptr)))
        {
            std::cout << "failed to read game_state_ptr" << std::endl;
            return false;
        }

        return true;
    }
 
    GameVersion GetGameVersion() const
    {
        static constexpr std::string_view V4_5H_STRING = "gamelist;4.5h";
        static constexpr std::string_view  V4_6_STRING = "gamelist;4.6";
        static constexpr std::string_view  V4_7_STRING = "gamelist;4.7";

        static constexpr uint32_t V4_5H_STRING_ADDRESS = 0x005333FC;
        static constexpr uint32_t  V4_6_STRING_ADDRESS = 0x00555CEC;
        static constexpr uint32_t  V4_7_STRING_ADDRESS = 0x0056357C;

        char buffer[32];
        bool was_read = ReadProcessMemoryEx(process_id, reinterpret_cast<LPVOID>(V4_5H_STRING_ADDRESS), &buffer, sizeof(buffer));

        if(was_read && std::string_view(buffer) == V4_5H_STRING)
        {
            return GameVersion::V4_5h;
        }

        was_read = ReadProcessMemoryEx(process_id, reinterpret_cast<LPVOID>(V4_6_STRING_ADDRESS), &buffer, sizeof(buffer));

        if(was_read && std::string_view(buffer) == V4_6_STRING)
        {
            return GameVersion::V4_6;
        }

        was_read = ReadProcessMemoryEx(process_id, reinterpret_cast<LPVOID>(V4_7_STRING_ADDRESS), &buffer, sizeof(buffer));

        if(was_read && std::string_view(buffer) == V4_7_STRING)
        {
            return GameVersion::V4_7;
        }

        return GameVersion::Unknown;
    }

    uint32_t GetGameStatePointerLocation(GameVersion version)
    {
        static const std::unordered_map<GameVersion, uint32_t> version_to_location{
            { GameVersion::V4_5h, 0x00537CCC },
            { GameVersion::V4_6 , 0x005655C0 },
            { GameVersion::V4_7 , 0x00568554 },
        };

        return version_to_location.at(version);
    }

    TGameState const& GetGameState() const
    {
        return game_state;
    }

    bool Update()
    {
        if(!ReattachToProccess()) { return false; }

        if(!game_state.Update(process_id, game_state_ptr)) { return false; }

        return true;
    }

private:    
    DWORD process_id = 0;
    uint32_t game_state_ptr = 0;
    TGameState game_state;
};
