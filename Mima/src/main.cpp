
#include "Mima/Parser.h"

#include <iostream>
#include <chrono>

int main(int argc, char* argv[])
{
    std::cout << std::endl;
    if (argc != 2) {
        std::cout << "Add your Input Program File Path as a Command line Argument!" << std::endl;
        return 1;
    }

    std::cout << "Now Running Program " << argv[1] << std::endl;

    auto instructions = InstructionParser().parse(argv[1]);

    if (!instructions.has_value())
        return 1;

    Mima mima(*instructions);

    auto start = std::chrono::high_resolution_clock::now();

    while (mima.canStep())
        mima.step();

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << std::endl << "Elapsed time: " << elapsed.count() << " s" << std::endl;
}
