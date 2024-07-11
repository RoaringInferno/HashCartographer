#include "symbol.hpp"

#include <bitset>

bool check_is_valid_solution(const hash_value_t *value, const word_index_t word_count)
{
    std::bitset<266> unique;
    for (word_index_t i = 0; i < word_count; i++)
    {
        if (unique[value[i]])
        {
            return false;
        }
        unique[value[i]] = true;
    }
    return true;
}

Symbol::Symbol(const std::string symbol, hash_value_t *value, const word_index_t word_count) :
    symbol(symbol),
    value(value),
    valid_solution(check_is_valid_solution(value, word_count))
{}

Symbol::Symbol(const Symbol &other) :
    symbol(other.symbol),
    value(other.value),
    valid_solution(other.valid_solution)
{}

Symbol::~Symbol() { delete[] value; }


std::string Symbol::get_symbol() const { return this->symbol; }

bool Symbol::is_valid_solution() const { return this->valid_solution; }

hash_value_t Symbol::get_value(const character_index_t index) const { return this->value[index]; }
