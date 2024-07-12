#include "symbol.hpp"

#include <iostream>
#include <bitset>

bool check_is_valid_solution(const std::vector<hash_value_t> value, const word_index_t word_count)
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
    std::cout << "Valid Solution:";
    for (word_index_t i = 0; i < word_count; i++)
    {
        std::cout << " " << std::to_string(value[i]);
    }
    std::cout << std::endl;
    return true;
}

Symbol::Symbol() :
    symbol("X"),
    value(),
    valid_solution(false)
{
}

Symbol::Symbol(const std::string symbol, std::vector<hash_value_t> value, const word_index_t word_count) : symbol(symbol),
                                                                                               value(value),
                                                                                               valid_solution(check_is_valid_solution(value, word_count))
{}

Symbol::Symbol(const std::string symbol, std::vector<hash_value_t> value, const word_index_t word_count, const bool valid_solution) :
    symbol(symbol),
    value(value),
    valid_solution(valid_solution)
{
    if (valid_solution)
    {
        std::cout << "Valid Solution: " << symbol << " has hash table:";
    }
    else
    {
        return;
    }
    for (word_index_t i = 0; i < word_count; i++)
    {
        std::cout << " " << std::to_string(value[i]);
    }
    std::cout << std::endl;
}


std::string Symbol::get_symbol() const { return this->symbol; }

bool Symbol::is_valid_solution() const { return this->valid_solution; }

hash_value_t Symbol::get_value(const character_index_t index) const { return this->value[index]; }
