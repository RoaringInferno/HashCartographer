#pragma once

#include "symbols.hpp"
#include "hash_function.hpp"

struct MetaData
{
    const Symbols symbols;
    const unsigned int word_count;
    std::vector<SingleOpHashFunction> solutions;
};

void single_layer_hash_function_search(MetaData& meta_data);