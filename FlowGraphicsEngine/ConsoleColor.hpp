#pragma once
#include <string>

namespace flowio
{
    class ConsoleColor
    {
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_YELLOW "\033[33m"
#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_CYAN "\033[36m"

#define ANSI_COLOR_BRIGHT "\033[1m"
#define ANSI_COLOR_RESET "\033[0m"
    public:
        enum ColorEnum : uint8_t
        {
            RED,
            GREEN,
            YELLOW,
            BLUE,
            MAGENTA,
            CYAN,
            RESET
        };
        static std::string GetColor(const ColorEnum &color);
        static std::string GetReset();
    };
}