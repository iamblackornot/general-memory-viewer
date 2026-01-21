#include "console/menu.hpp"

#include <windows.h>

int main() 
{
    SetConsoleOutputCP(CP_UTF8);
    return MenuLoop();
}