#pragma once

#include "Data.h"

#include <vector>

struct instruction;

class Mima {
public:
    value Akku;
    value* M;
    int IrAddress = 1048500;

    std::vector<instruction*> instructions;

public:
    Mima(const std::vector<instruction*>& instructions) : instructions(instructions) {
        M = new value[1 << 20];
    }

    ~Mima()
    {
        delete[] M;
    }

    void step();
    bool canStep();
};
