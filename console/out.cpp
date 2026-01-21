#include "out.hpp"

#include <objects/types/t_squares_array.hpp>

#define DEFAULT_SQUARE "  "
#define MARKED_SQUARE "▓▓"
#define CONQUERED_SQUARE "▪▪"

void PrintColoredSquare(std::ostream& out, TSquare const& square)
{
    out << CountryIdToBGColor(square.country_id - 1) << (square.mark_for_transfer ? MARKED_SQUARE : DEFAULT_SQUARE) << COLOR_RESET;
}

void PrintColoredSquareDiff(std::ostream& out, TSquare const& square, TSquare const& prev)
{
    if(square.country_id != prev.country_id && prev.country_id > 0)
    {
        out << CountryIdToBGColor(square.country_id - 1) << CountryIdToFGColor(prev.country_id - 1) << CONQUERED_SQUARE << COLOR_RESET;
    }
    else
    {
        out << CountryIdToBGColor(square.country_id - 1) << DEFAULT_SQUARE << COLOR_RESET;
    }
}
