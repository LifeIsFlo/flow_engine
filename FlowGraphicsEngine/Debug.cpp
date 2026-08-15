#include <string>
#include <fstream>
#include <filesystem>
#include "Debug.hpp"


std::ofstream& flowio::Debug::GetLog()
{
    static std::ofstream log(
        (std::filesystem::current_path() / "logs" / "debug.txt"),
        std::ios::app);

    return log;
}
void flowio::Debug::WriteLine(const std::string &msg)
{
    Write(msg + "\n");
}
void flowio::Debug::WriteLine(const std::string &msg, const ConsoleColor::ColorEnum &color)
{
    Write(ConsoleColor::GetColor(color) + msg + ConsoleColor::GetReset() + "\n");
}
void flowio::Debug::Write(const std::string &msg)
{
    GetLog() << msg;
    GetLog().flush();
}
void flowio::Debug::Write(const std::string &msg, const ConsoleColor::ColorEnum &color)
{
    Write(ConsoleColor::GetColor(color) + msg + ConsoleColor::GetReset());
}