#pragma once

#include "Data.h"

#include <vector>

struct instruction;

class Mima {
public:
    value Akku;
    value Ir;
    // value Iar; TODO idk what that does
    value M[2 ^ 20];

    std::vector<instruction*> instructions;

public:
    Mima(const std::vector<instruction*>& instructions) : instructions(instructions) {}

    void step();
    bool canStep();
};
