#include "hash_function.hpp"

#include <bitset>

void HashOperation::evaluate(const Symbols &symbols, const unsigned int word)
{
    value = execute(this->operation, symbols.pull_symbol(word, lhs), symbols.pull_symbol(word, rhs));
}

SingleOpHashFunction::SingleOpHashFunction(const unsigned int word_count) : word_count(word_count), hashes(new uint8_t[word_count])
{
}

bool SingleOpHashFunction::evaluate(const Symbols &symbols)
{
    std::bitset<256> hash_values;
    bool valid = true;
    for (unsigned int i = 0; i < word_count; i++)
    {
        operation.evaluate(symbols, i);
        hashes[i] = operation.value;
        if (hash_values.test(operation.value)) valid = false; else hash_values.set(operation.value);
    }
    return valid;
}
