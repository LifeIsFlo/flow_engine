#include <string>
#include <iostream>
#include "Console.hpp"

void flowio::Console::WriteLine(const std::string &msg)
{
    std::cout << msg << std::endl;
}

void flowio::Console::WriteLine(const std::string &msg, const ConsoleColor::ColorEnum &color)
{
    std::cout << ConsoleColor::GetColor(color) + msg + ConsoleColor::GetReset() << std::endl;
}

void flowio::Console::Write(const std::string &msg)
{
    std::cout << msg;
}
void flowio::Console::Write(const std::string &msg, const ConsoleColor::ColorEnum &color)
{
    std::cout << ConsoleColor::GetColor(color) + msg + ConsoleColor::GetReset();
}

void flowio::Console::Clear()
{
    std::cout << "\033[H\033[2J" << std::flush;
}

std::string flowio::Console::ReadLine()
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}