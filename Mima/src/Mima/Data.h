#pragma once

// Wrapper for a 24bit signed integer
struct value {
    value() : value(0) {}
    value(int v) : v(v) {}
    int v : 24;
};

// Wrapper for a 20bit signed integer
struct addr {
    addr() : addr(0) {}
    addr(int v) : v(v) {}
    unsigned int v : 20;
};
