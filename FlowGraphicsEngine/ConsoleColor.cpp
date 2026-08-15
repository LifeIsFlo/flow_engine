#include <string>
#include "ConsoleColor.hpp"

std::string flowio::ConsoleColor::GetColor(const ColorEnum &color)
{
    switch (color)
    {
    case RED:
        return ANSI_COLOR_RED;
    case GREEN:
        return ANSI_COLOR_GREEN;
    case YELLOW:
        return ANSI_COLOR_YELLOW;
    case BLUE:
        return ANSI_COLOR_BLUE;
    case MAGENTA:
        return ANSI_COLOR_MAGENTA;
    case CYAN:
        return ANSI_COLOR_CYAN;
    case RESET:
        return GetReset();
    }

    return GetReset();
}

std::string flowio::ConsoleColor::GetReset()
{
    return ANSI_COLOR_RESET;
}