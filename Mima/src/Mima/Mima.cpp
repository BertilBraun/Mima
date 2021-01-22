#include "Mima.h"
#include "Instructions.h"

void Mima::step() {
    
    M[IrAddress].v++;
    instructions[M[IrAddress].v - 1]->run(*this);
}

bool Mima::canStep() {
    return M[IrAddress].v < instructions.size();
}
