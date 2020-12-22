#include "Mima.h"
#include "Instructions.h"

void Mima::step() {
    Ir.v++;
    instructions[Ir.v - 1]->run(*this);
}

bool Mima::canStep() {
    return Ir.v < instructions.size();
}
