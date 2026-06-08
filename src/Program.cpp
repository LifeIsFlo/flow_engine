#include "Player.h"
#include "Debug.h"
#include "Console.h"
#include "ConsoleColor.h"

int main()
{
    Player player("flow");

    Debug::WriteLine("debug test");
    Console::WriteLine("console test", ConsoleColor::BLUE);

    Console::ReadLine();
    Console::ReadLine();
    Console::ReadLine();
    Console::ReadLine();
    Console::ReadLine();
    Console::ReadLine();
    Console::ReadLine();

    return 0;
}