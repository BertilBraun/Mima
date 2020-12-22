#pragma once

#include "Mima.h"
#include "Data.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>

struct instruction {
    virtual void run(Mima& mima) = 0;
};

struct LDC : public instruction {
    const value c;
    LDC(int c) : c(c) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.Akku = c;
    }
};

struct LDV : public instruction {
    const addr a;
    LDV(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.Akku = mima.M[a.v];
    }
};

struct STV : public instruction {
    const addr a;
    STV(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.M[a.v] = mima.Akku;
    }
};

struct ADD : public instruction {
    const addr a;
    ADD(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.Akku.v += mima.M[a.v].v;
    }
};

struct AND : public instruction {
    const addr a;
    AND(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.Akku.v &= mima.M[a.v].v;
    }
};

struct OR : public instruction {
    const addr a;
    OR(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.Akku.v |= mima.M[a.v].v;
    }
};

struct XOR : public instruction {
    const addr a;
    XOR(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.Akku.v ^= mima.M[a.v].v;
    }
};

struct EQL : public instruction {
    const addr a;
    EQL(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        if (mima.Akku.v == mima.M[a.v].v)
            mima.Akku.v = -1;
        else
            mima.Akku.v = 0;
    }
};

struct JMP : public instruction {
    const addr a;
    JMP(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.Ir.v = a.v;
    }
};

struct JMN : public instruction {
    const addr a;
    JMN(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        if (mima.Akku.v < 0)
            mima.Ir.v = a.v;
    }
};

struct HALT : public instruction {
    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.Ir.v = mima.instructions.size();
    }
};

struct NOT : public instruction {
    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.Akku.v = ~mima.Akku.v;
    }
};

struct RAR : public instruction {
    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        int carry = mima.Akku.v & 1;
        mima.Akku.v = ((mima.Akku.v >> 1) & ~(3 << 23)) | (carry << 23);
    }
};

struct LDIV : public instruction {
    const addr a;
    LDIV(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.Akku = mima.M[mima.M[a.v].v];
    }
};

struct STIV : public instruction {
    const addr a;
    STIV(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        mima.M[mima.M[a.v].v] = mima.Akku;
    }
};

static std::string getHexRepr(int v, int lengthInBits) {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(lengthInBits / 4) << std::hex << v;
    return stream.str();
}

static std::string getBinRepr(int v) {
    std::stringstream stream;
    std::bitset<24> x(v);
    stream << x << "b";
    return stream.str();
}

struct PRINTAKKU : public instruction {
    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        std::cout << "Akku:   " << " bin: " << getBinRepr(mima.Akku.v) << " hex: " << getHexRepr(mima.Akku.v, 24) << " dez: " << mima.Akku.v << std::endl;
    }
};

struct PRINT : public instruction {
    const addr a;
    PRINT(int a) : a(a) {}

    // Inherited via instruction
    virtual void run(Mima& mima) override
    {
        std::cout << getHexRepr(a.v, 20) << " bin: " << getBinRepr(mima.M[a.v].v) << ": hex: " << getHexRepr(mima.M[a.v].v, 24) << " dez: " << mima.M[a.v].v << std::endl;
    }
};

// FUTURE Improvement possibilities
// SUB
// INC
// DEC
// CALL