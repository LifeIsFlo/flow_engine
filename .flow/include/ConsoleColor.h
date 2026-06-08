#pragma once
#include <string>
class ConsoleColor
{

private:
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_YELLOW "\033[33m"
#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_CYAN "\033[36m"

#define ANSI_COLOR_BRIGHT "\033[1m"
#define ANSI_COLOR_RESET "\033[0m"
public:
    ConsoleColor();
    ~ConsoleColor();
    enum ColorEnum
    {
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        RESET
    };
    static std::string GetColor(const ColorEnum &color)
    {
        switch (color)
        {
        case RED:
            return ANSI_COLOR_RED;
            break;
        case GREEN:
            return ANSI_COLOR_GREEN;
            break;
        case YELLOW:
            return ANSI_COLOR_YELLOW;
            break;
        case BLUE:
            return ANSI_COLOR_BLUE;
            break;
        case MAGENTA:
            return ANSI_COLOR_MAGENTA;
            break;
        case CYAN:
            return ANSI_COLOR_CYAN;
            break;
        case RESET:
            return GetReset();
            break;
        default:
            return GetReset();
            break;
        }
    }

    static std::string GetReset()
    {
        return ANSI_COLOR_RESET;
    }
};
