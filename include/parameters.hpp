#pragma once

#include <cstdint>

typedef uint16_t word_index_t; // Defines the maximum input word count
typedef uint8_t character_index_t; // Defines the maximum character count in a word
typedef uint8_t operation_index_t; // Defines the maximum operation count
typedef uint8_t hash_value_t; // Defines the value of each hash

namespace setting
{
    const operation_index_t default_operation_search_depth = 1; // The default depth to search for solutions

    const operation_index_t character_read_opcount_cost = 0; // The recorded opcount of accessing a character in a word
    const operation_index_t constant_read_opcount_cost = 0; // The recorded opcount of accessing a constant value
    const operation_index_t operation_opcount_cost = 1; // The recorded opcount of performing an operation
};