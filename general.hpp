#include <memory>
#include <string_view>

#include "inject.hpp"
#include "objects/types/t_game_state.hpp"

class General
{
public:
    static constexpr std::wstring_view PROCESS_NAME = L"general.exe";
    static constexpr uint32_t GAME_SETTINGS_ADDR = 0x00537CCC;

    General() = default;

    bool ReattachToProccess()
    {
        process_id = GetProcessIdByName(PROCESS_NAME.data());
    
        if (process_id == 0) 
        {
            std::cout << "process not found." << std::endl;
            return false;
        }
        
        if (!ReadProcessMemoryEx(process_id, reinterpret_cast<LPVOID>(GAME_SETTINGS_ADDR), &games_state_ptr, sizeof(games_state_ptr)))
        {
            std::cout << "failed to read games_state_ptr" << std::endl;
            return false;
        }

        return true;
    }

    TGameState const& GetGameState() const
    {
        return game_state;
    }

    bool Update()
    {
        if(!ReattachToProccess()) { return false; }

        if(!game_state.Update(process_id, games_state_ptr)) { return false; }

        return true;
    }

private:    
    DWORD process_id = 0;
    uint32_t games_state_ptr = 0;
    TGameState game_state;
};
