#pragma once

#include <cstdint>

typedef uint16_t word_index_t; // Defines the maximum input word count
typedef uint8_t character_index_t; // Defines the maximum character count in a word
typedef uint8_t operation_index_t; // Defines the maximum operation count
typedef uint8_t hash_value_t; // Defines the value of each hash

namespace setting
{
    static const operation_index_t default_operation_search_depth = 3;
};