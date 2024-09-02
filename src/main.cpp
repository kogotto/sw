#include <iostream>
#include <fstream>

#include "IO/Commands/Commands.hpp"
#include "system/MainLoop.hpp"

int main(int argc, char** argv)
{
    using namespace sw;

    if (argc != 2) {
        throw std::runtime_error("Error: No file specified in command line argument");
    }

    std::ifstream file(argv[1]);
    if (!file) {
        throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
    }

    const auto commands = io::parseCommands(file);
    mainLoop(commands);

    return 0;
}
