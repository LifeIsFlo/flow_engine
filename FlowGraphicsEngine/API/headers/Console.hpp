#pragma once
#include "ConsoleColor.hpp"

namespace flowio
{
    class Console
    {
    public:
        static void WriteLine(const std::string &msg);
        static void WriteLine(const std::string &msg, const ConsoleColor::ColorEnum &color);

        static void Write(const std::string &msg);
        static void Write(const std::string &msg, const ConsoleColor::ColorEnum &color);

        static void Clear();

        static std::string ReadLine();
    };
}