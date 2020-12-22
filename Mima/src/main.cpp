
#include "Mima/Parser.h"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    std::string input;
    std::string filePath = "example/Example.mima";

    while (input != "q")
    {
        std::cout << "Input your Program File ('q' to exit, 'r' to relaunch): ";
        std::cin >> input;

        if (input != "r")
            filePath = input;
        if (input == "q")
            break;

        auto instructions = InstructionParser().parse(filePath);

        if (!instructions.has_value())
            continue;

        Mima mima(*instructions);

        // TODO for future, 
        // - you could enable the user to go through step by step
        // - you could delay each step, so that you can analize the runtime

        auto start = std::chrono::high_resolution_clock::now();

        while (mima.canStep()) {
            mima.step();
            //std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    }

    std::cout << "Have a nice Day!" << std::endl;
}
