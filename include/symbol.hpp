#pragma once

#include "parameters.hpp"

#include <string>
#include <vector>

class Symbol
{
private:
    std::string symbol;
    std::vector<hash_value_t> value;
    bool valid_solution;
    operation_index_t opcount;
    operation_index_t tightness;
public:
    Symbol();
    Symbol(const std::string symbol, std::vector<hash_value_t> value, const word_index_t word_count, const bool valid_solution, const operation_index_t opcount, const operation_index_t tightness);

    std::string get_symbol() const;
    bool is_valid_solution() const;
    hash_value_t get_value(const character_index_t index) const;
    operation_index_t get_opcount() const;
    operation_index_t get_tightness() const;

    std::string to_string() const;
};