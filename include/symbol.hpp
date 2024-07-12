#pragma once

#include "parameters.hpp"

#include <string>

class Symbol
{
private:
    std::string symbol;
    std::vector<hash_value_t> value;
    bool valid_solution;
public:
    Symbol();
    Symbol(const std::string symbol, std::vector<hash_value_t> value, const word_index_t word_count);
    Symbol(const std::string symbol, std::vector<hash_value_t> value, const word_index_t word_count, const bool valid_solution);

    std::string get_symbol() const;
    bool is_valid_solution() const;
    hash_value_t get_value(const character_index_t index) const;
};