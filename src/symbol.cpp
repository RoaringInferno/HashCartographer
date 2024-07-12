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
    return true;
}

Symbol::Symbol() :
    symbol("X"),
    value(),
    valid_solution(false),
    opcount(0)
{}

hash_value_t calc_tightness(const std::vector<hash_value_t> value, const word_index_t word_count)
{
    hash_value_t max, min;
    for (word_index_t i = 0; i < word_count; i++)
    {
        if (i == 0)
        {
            max = value[i];
            min = value[i];
        }
        else
        {
            if (value[i] > max)
            {
                max = value[i];
            }
            if (value[i] < min)
            {
                min = value[i];
            }
        }
    }
    return max - min;
}

Symbol::Symbol(const std::string symbol, std::vector<hash_value_t> value, const word_index_t word_count, const bool valid_solution, const operation_index_t opcount) :
    symbol(symbol),
    value(value),
    valid_solution(valid_solution),
    opcount(opcount)
{
    /*
    if (valid_solution)
    {
        std::cout << "Valid Solution: " << this->to_string() << std::endl;
    }
    */
}


std::string Symbol::get_symbol() const { return this->symbol; }

bool Symbol::is_valid_solution() const { return this->valid_solution; }

hash_value_t Symbol::get_value(const character_index_t index) const { return this->value[index]; }

operation_index_t Symbol::get_opcount() const { return this->opcount; }

std::string Symbol::to_string() const
{
    std::string str = this->symbol + " with opcount " + std::to_string(this->opcount) + " has hash table:";
    for (hash_value_t val : this->value)
    {
        str += " " + std::to_string(val);
    }
    return str;
}