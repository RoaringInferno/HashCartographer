#pragma once

#include "parameters.hpp"

#include <string>
#include <bitset>

struct Args
{
    std::string* args;
    word_index_t count;

    // Options
    std::bitset<256> flags;
    operation_index_t depth = setting::default_operation_search_depth;
};

void parse_args(Args& args, int argc, char** argv);