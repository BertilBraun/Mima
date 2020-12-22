#include "Mima.h"
#include "Instructions.h"

void Mima::step() {
    instructions[Ir.v]->run(*this);
    Ir.v++;
}

bool Mima::canStep() {
    return Ir.v < instructions.size();
}
