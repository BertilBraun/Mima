
#include "Mima/Parser.h"

#include <string>
#include <iostream>
#include <emscripten/emscripten.h>

void Mima::step() {
    Ir.v++;
    instructions[Ir.v - 1]->run(*this);
}

bool Mima::canStep() {
    return M != nullptr && Ir.v < instructions.size();
}

void Mima::free() {
	delete[] M;
	M = nullptr;
	for (auto instr : instructions)
		delete instr;
	instructions.clear();
}

Mima mima;

// Example Code:  "LDC 1\nSTV 1\nINC:\nADD 1\nPRINTAKKU\nJMP INC";
	

extern "C" {

	void load(const char* code) {
		auto instructions = InstructionParser().parseCode(code);

		mima.free();

		if (instructions.empty()) {
			std::cout << "Failed to load!" << std::endl;
			return;
		}

		mima.load(instructions);
	}

	void step() {
		if (mima.canStep()) {
			mima.step();
		}
		else {
			std::cout << "Can't Step!" << std::endl;
		}
	}

	bool canStep() {
		return mima.isLoaded() && mima.canStep();
	}
}
