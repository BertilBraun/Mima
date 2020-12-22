#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "Instructions.h"
#include "Util.h"
#include <optional>

class InstructionParser {
private:
    struct JumpLoad : public instruction {
        std::string element;
        int idx;
        std::string type;
        JumpLoad(const std::string& element, int idx, const std::string& type) :
            element(element), idx(idx), type(type) {}

        // Inherited via instruction
        virtual void run(Mima& mima) override { std::cout << "ERROR" << std::endl; };
    };

    std::map<std::string, int> jumpLocations;
    std::vector<instruction*> instructions;
    std::vector<JumpLoad*> jumpLoads;

public:
    std::optional<std::vector<instruction*>> parse(const std::string& filePath) {

        std::string line;
        std::ifstream file(filePath);

        if (!file.is_open()) {
            std::cout << "Couldn't open the provided File: " << filePath<< std::endl;
            return std::nullopt;
        }

        std::vector<std::pair<std::string, int>> lines;
        for (int lineCount = 1; std::getline(file, line); lineCount++) {
            trim(line);
            line = line.substr(0, line.find("//", 0));
            if (line != "")
                lines.push_back({ line, lineCount });
        }

        for (auto pair : lines) {
            parseLocation(pair.first, pair.second);
            if (pair.first == "")
                continue;

            auto instruction = parseLine(pair.first, pair.second);
            if (!instruction.has_value())
                return std::nullopt;
            instructions.push_back(*instruction);
        }

        for (auto jumpLoad : jumpLoads) {
            if (jumpLocations.find(toLower(jumpLoad->element)) == jumpLocations.end()) {
                std::cout << "Jump location could not be found! " << jumpLoad->element << std::endl;
                return std::nullopt;
            }
            if (jumpLoad->type == "JMP")
                instructions[jumpLoad->idx] = new JMP(jumpLocations[jumpLoad->element]);
            else if (jumpLoad->type == "JMN")
                instructions[jumpLoad->idx] = new JMN(jumpLocations[jumpLoad->element]);
        }

        std::cout << "Parsing complete! " << instructions.size() << " instructions" << std::endl;
        return instructions;
    }

private:
    void parseLocation(std::string& line, int lineNumber) {
        // Parse jump locations and clean lines
        trim(line);

        std::vector<std::string> elements = split(line, ':');

        if (elements.size() == 1 && line.back() == ':')
            line = "";
        else {
            line = elements.back();
            elements.pop_back();
        }

        for (std::string element : elements) {
            element = toLower(element);
            if (jumpLocations.find(element) != jumpLocations.end()) {
                std::cout << "Error on line: " << lineNumber << " Jump loaction was already defined: " << element << std::endl;
                return;
            }

            jumpLocations[element] = (int)instructions.size() - 1;
        }
    }

    std::optional<instruction*> parseLine(const std::string& line, int lineNumber) {

        auto error = [lineNumber, line](const char* msg) {
            std::cout << "Error on line: " << line << " : " << lineNumber << " with message: " << msg << std::endl;
            return std::nullopt;
        };

        std::vector<std::string> elements = split(line, ' ');
        if (elements.size() > 2)
            return error("Too many arguments provided!");

        std::string instructionCode = toLower(elements[0]);

        try
        {
            if (instructionCode == "ldc") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'LDC'! 1 expected!");
                return new LDC(parseInt(elements[1]));
            }
            else if (instructionCode == "ldv") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'LDV'! 1 expected!");
                return new LDV(parseInt(elements[1]));
            }
            else if (instructionCode == "stv") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'STV'! 1 expected!");
                return new STV(parseInt(elements[1]));
            }
            else if (instructionCode == "add") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'ADD'! 1 expected!");
                return new ADD(parseInt(elements[1]));
            }
            else if (instructionCode == "and") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'AND'! 1 expected!");
                return new AND(parseInt(elements[1]));
            }
            else if (instructionCode == "or") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'OR'! 1 expected!");
                return new OR(parseInt(elements[1]));
            }
            else if (instructionCode == "xor") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'XOR'! 1 expected!");
                return new XOR(parseInt(elements[1]));
            }
            else if (instructionCode == "eql") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'EQL'! 1 expected!");
                return new EQL(parseInt(elements[1]));
            }
            else if (instructionCode == "jmp") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'JMP'! 1 expected!");
                jumpLoads.push_back(new JumpLoad(toLower(elements[1]), (int)instructions.size(), "JMP"));
                return jumpLoads.back();
            }
            else if (instructionCode == "jmn") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'JMN'! 1 expected!");
                jumpLoads.push_back(new JumpLoad(toLower(elements[1]), (int)instructions.size(), "JMN"));
                return jumpLoads.back();
            }
            else if (instructionCode == "halt") {
                if (elements.size() != 1)
                    return error("Not the correct amount of parameters for 'HALT'! 0 expected!");
                return new HALT();
            }
            else if (instructionCode == "not") {
                if (elements.size() != 1)
                    return error("Not the correct amount of parameters for 'NOT'! 0 expected!");
                return new NOT();
            }
            else if (instructionCode == "rar") {
                if (elements.size() != 1)
                    return error("Not the correct amount of parameters for 'RAR'! 0 expected!");
                return new RAR();
            }
            else if (instructionCode == "ldiv") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'LDIV'! 1 expected!");
                return new LDIV(parseInt(elements[1]));
            }
            else if (instructionCode == "stiv") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'STIV'! 1 expected!");
                return new STIV(parseInt(elements[1]));
            }
            else if (instructionCode == "printakku") {
                if (elements.size() != 1)
                    return error("Not the correct amount of parameters for 'PRINTAKKU'! 0 expected!");
                return new PRINTAKKU();
            }
            else if (instructionCode == "print") {
                if (elements.size() != 2)
                    return error("Not the correct amount of parameters for 'PRINT'! 1 expected!");
                return new PRINT(parseInt(elements[1]));
            }
            else
                return error("Command not recognized!");
        }
        catch (const std::exception& e)
        {
            return error(e.what());
        }
    }

    int parseInt(const std::string& s) {
        try
        {
            if (s.find("0x") != std::string::npos)
                return std::stoi(s.substr(2, s.length() - 2), nullptr, 16);
            if (s.back() == 'b')
                return std::stoi(s.substr(0, s.length() - 1), nullptr, 2);
            else
                return std::stoi(s);
        }
        catch (const std::exception&)
        {
            throw std::exception(("Can't parse parameter: " + s).c_str());
        }
    }
};
