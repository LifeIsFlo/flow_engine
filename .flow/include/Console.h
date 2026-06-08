#pragma once
#include "Debug.h"
#include "ConsoleColor.h"
#include <iostream>
#include <string>

class Console
{
public:
    static void WriteLine(const std::string &msg)
    {
        std::cout << msg << std::endl; // flushes automatically
    }
    static void WriteLine(const std::string &msg, const ConsoleColor::ColorEnum &color)
    {
        std::cout << ConsoleColor::GetColor(color) + msg + ConsoleColor::GetReset() << std::endl; // flushes automatically
    }

    static void Write(const std::string &msg)
    {
        std::cout << msg;
    }
    static void Write(const std::string &msg, const ConsoleColor::ColorEnum &color)
    {
        std::cout << ConsoleColor::GetColor(color) + msg + ConsoleColor::GetReset();
    }

    static std::string ReadLine()
    {
        std::string input;
        std::getline(std::cin, input);
        Debug::WriteLine("User input: " + input);
        return input;
    }
};