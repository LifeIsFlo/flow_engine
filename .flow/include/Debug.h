#pragma once
#include <iostream>
#include <fstream>
#include "ConsoleColor.h"
#include <string>

class Debug
{
public:
    static void WriteLine(const std::string &msg)
    {
        std::ofstream log("../");
        log << msg << std::endl; // flushes automatically
    }
    static void WriteLine(const std::string &msg, const ConsoleColor::ColorEnum &color)
    {
        std::ofstream log("../logs/debug.txt");
        log << ConsoleColor::GetColor(color) + msg + ConsoleColor::GetReset() << std::endl; // flushes automatically
    }

    static void Write(const std::string &msg)
    {
        std::ofstream log("../logs/debug.txt");
        log << msg;
    }
    static void Write(const std::string &msg, const ConsoleColor::ColorEnum &color)
    {
        std::ofstream log("../logs/debug.txt");
        log << ConsoleColor::GetColor(color) + msg;
    }
};