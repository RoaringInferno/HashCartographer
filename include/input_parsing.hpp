#pragma once

#include <string>

struct Args
{
    std::string* args;
    unsigned int count;
};

void parse_args(Args& args, int argc, char** argv);