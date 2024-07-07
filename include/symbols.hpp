#pragma once

#include "input_parsing.hpp"

#include <vector>

struct Symbol
{
    bool is_constant;

    unsigned int character;
    uint8_t value;
};

class Symbols
{
    uint8_t** symbols;

    const unsigned int word_count;
    unsigned int min_word_length;
public:
    Symbols(Args& args);
    ~Symbols();

    uint8_t get_character(const unsigned int word, const unsigned int index) const;

    uint8_t pull_symbol(const unsigned int word, Symbol& symbol) const;

    unsigned int get_word_count() const { return word_count; }
    unsigned int get_min_word_length() const { return min_word_length; }
};