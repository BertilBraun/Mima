#pragma once

#include "Data.h"

#include <vector>

struct instruction;

class Mima {
public:
    value Akku;
    value Ir;
    value* M = nullptr; 

    std::vector<instruction*> instructions;

public:	
	void load(const std::vector<instruction*>& instructions) {
		if (M != nullptr)
			delete[] M;
        this->instructions = instructions;
        this->M = new value[1 << 20];
		this->Ir = value();
		this->Akku = value();
	}
	
	bool isLoaded() {
		return M != nullptr && instructions.size() != 0;
	}

	void free();

    void step();
    bool canStep();
};
