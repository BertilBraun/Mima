
#include "Mima/Parser.h"

#include <string>
#include <iostream>
#include <emscripten/emscripten.h>
#include <chrono>
#include <thread>

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
bool isRunning = false;

// Example Code:  "LDC 1\nSTV 1\nINC:\nADD 1\nPRINTAKKU\nJMP INC";
	

extern "C" {

	void load(const char* code) {
		isRunning = false;
		auto instructions = InstructionParser().parseCode(code);

		mima.free();

		if (instructions.empty()) {
			std::cout << "Failed to load!" << std::endl;
			return;
		}

		mima.load(instructions);
	}

	void step() {
		if (isRunning) {
			std::cout << "Already running!" << std::endl;
			return;
		}
		if (mima.canStep()) {
			mima.step();
		}
		else {
			std::cout << "Can't Step!" << std::endl;
		}
	}

	void stop() {
		isRunning = false;
	}

	void run() {
		if (!mima.isLoaded()) {
			std::cout << "Mima not loaded!" << std::endl;
			return;
		}

		isRunning = true;

		while (isRunning && mima.canStep()) {
			mima.step();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}
