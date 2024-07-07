#pragma once

#include "operations.hpp"
#include "symbols.hpp"

#include <vector>

struct HashOperation
{
    Operation operation;
    Symbol rhs, lhs;
    uint8_t value;

    void evaluate(const Symbols& symbols, const unsigned int word);
};

class SingleOpHashFunction
{
public:
    SingleOpHashFunction(const unsigned int word_count);

    HashOperation operation;
    uint8_t* hashes;
    const unsigned int word_count;

    bool evaluate(const Symbols& symbols); // Returns true if the hash function is a valid solution
};