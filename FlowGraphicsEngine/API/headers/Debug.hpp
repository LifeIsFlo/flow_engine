#pragma once
#include "ConsoleColor.hpp"

namespace flowio
{
    class Debug
    {
    public:
        static std::ofstream& GetLog();
        static void WriteLine(const std::string &msg);
        static void WriteLine(const std::string &msg, const ConsoleColor::ColorEnum &color);
        static void Write(const std::string &msg);
        static void Write(const std::string &msg, const ConsoleColor::ColorEnum &color);
    };
}